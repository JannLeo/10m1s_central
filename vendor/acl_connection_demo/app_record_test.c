/********************************************************************************************************
 * @file    app_record_test.c
 *
 * @brief   BLE connection test module source file
 *
 * @author  BLE GROUP
 * @date    08,2025
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
/********************************************************************************************************
 * @file    app_record_test.c
 *********************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "app_codec.h"
#include "app_record_test.h"
/*********** UART DMA for PCM TX (1,000,000 bps) ***********/
#include "uart.h"
#include "dma.h"
#define FRAME_MS         20
#define FRAME_US         (FRAME_MS * 1000)
#define SAMPLE_RATE_HZ   8000
#define BYTES_PER_SAMPLE 2
#define CHANNELS         1
#define FRAME_BYTES      (SAMPLE_RATE_HZ/1000 * FRAME_MS * BYTES_PER_SAMPLE * CHANNELS) // 320
#define HDR_BYTES 4
#define PKT_BYTES (HDR_BYTES + FRAME_BYTES)   // 4 + 320 = 324

// TLKAPI 文本模式：每字节会变成 " xx"，FIFO 有限，这里保守点每块发 <=80B
#define TLKAPI_CHUNK_BYTES 80
extern int _write(int fd, const unsigned char *buf, int size);

// 每条 FIFO item 的最大原始数据长度：FIFO_SIZE - 4 字节头
#ifndef TLKAPI_DEBUG_FIFO_SIZE
#include "tlkapi_debug.h"
#endif
#define TLKAPI_BIN_CHUNK  (TLKAPI_DEBUG_FIFO_SIZE - 4)  // 默认 284B（若你没改宏）



static volatile unsigned char uart_dma_send_flag = 1; // 发送完成标志（TXDONE置1）

// _attribute_ram_code_sec_noinline_
// void uart1_irq_handler(void){
//     // 根据你 SDK 的 API 改写：判断 TXDONE 并清中断
//     if(uart_get_irq_status(UART_MODULE_SEL, UART_TXDONE_IRQ_STATUS)){
//         uart_dma_send_flag = 1;
//         uart_clr_irq_status(UART_MODULE_SEL, UART_TXDONE_IRQ_STATUS);
//     }
    
//     // 如需：清其它可能的状态位，避免重复进中断
// }
// PLIC_ISR_REGISTER(uart1_irq_handler, IRQ_UART1)
// void uart_pcm_tx_init_1m(void)
// {
//     unsigned short div;
//     unsigned char bwpc;

//     // 1) 设置 UART 引脚（TX/RX）
//     uart_set_pin(UART_MODULE_SEL, UART_TX_PIN, UART_RX_PIN);

//     // 2) 计算波特率分频器（1000000 bps）
//     uart_cal_div_and_bwpc(1000000, sys_clk.pclk*1000*1000, &div, &bwpc);
//     uart_init(UART_MODULE_SEL, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

//     // 4) 配置 TX DMA
//     uart_set_tx_dma_config(UART_MODULE_SEL, UART_TX_DMA_CHN);
//     uart_clr_irq_status(UART_MODULE_SEL, UART_TXDONE_IRQ_STATUS);
//     uart_set_irq_mask(UART_MODULE_SEL, UART_TXDONE_MASK);  // 启用 TXDONE 中断

//     // 5) 设置 UART 中断优先级
//     if (UART_MODULE_SEL == UART0) {
//         plic_interrupt_enable(IRQ_UART0);
//         plic_set_priority(IRQ_UART0, 2);
//     } else if (UART_MODULE_SEL == UART1) {
//         plic_interrupt_enable(IRQ_UART1);
//         plic_set_priority(IRQ_UART1, 2);
//     }

//     core_interrupt_enable();  // 启用全局中断

//     uart_dma_send_flag = 1;  // 初始化标志位
// }


static uint8_t tx_pkt[PKT_BYTES] __attribute__((aligned(4)));

static inline void make_pkt_320(const uint8_t *pcm320){
    tx_pkt[0] = 'P';
    tx_pkt[1] = 'C';
    tx_pkt[2] = (uint8_t)(FRAME_BYTES & 0xFF);
    tx_pkt[3] = (uint8_t)(FRAME_BYTES >> 8);
    memcpy(&tx_pkt[4], pcm320, FRAME_BYTES);
}

static inline void uart_send_frame_dma(void){
    unsigned int timeout = 1000;  // 超时设置为 1000 次循环
    // 等上一帧发完
    while (!uart_dma_send_flag && timeout--) {
        // 如果超时，打印调试信息
        if (timeout == 0) {
            printf("UART DMA 超时\n");
            return;  // 超时退出
        }
    }
    uart_dma_send_flag = 0;
    // uart_send_dma(UART_MODULE_SEL, tx_pkt, PKT_BYTES);  // 发送数据
}


// #define RING_BYTES (sizeof(AUDIO_BUFF))

// static uintptr_t prev_off = 0;

// static inline uint32_t ring_delta(uintptr_t prev, uintptr_t now, uint32_t size){
//     return (now >= prev) ? (now - prev) : (size - (prev - now));
// }

// static void pump_one_frame_if_ready(void){
//     uintptr_t base = (uintptr_t)AUDIO_BUFF;
//     uintptr_t w    = (uintptr_t)audio_get_rx_dma_wptr(RX_DMA_CHN);
//     uintptr_t off  = (w - base) % RING_BYTES;

//     if (ring_delta(prev_off, off, RING_BYTES) < FRAME_BYTES) return;

//     uint32_t head_free = RING_BYTES - (prev_off % RING_BYTES);
//     const uint8_t *p1  = (const uint8_t*)AUDIO_BUFF + (prev_off % RING_BYTES);

//     uint8_t frame[FRAME_BYTES];
//     if (head_free >= FRAME_BYTES){
//         memcpy(frame, p1, FRAME_BYTES);
//     }else{
//         memcpy(frame, p1, head_free);
//         memcpy(frame + head_free, (const uint8_t*)AUDIO_BUFF, FRAME_BYTES - head_free);
//     }
//     prev_off = (prev_off + FRAME_BYTES) % RING_BYTES;

//     make_pkt_320(frame);
//     uart_send_frame_dma();
// }


// void main_loop_record_test(void){
//     static unsigned int t_next = 0;
//     if (t_next == 0){
//         t_next = clock_time();

//         // 关键：第一次对齐到当前写指针，避免一上来把环里旧数据倒出去
//         uintptr_t base = (uintptr_t)AUDIO_BUFF;
//         prev_off = ((uintptr_t)audio_get_rx_dma_wptr(RX_DMA_CHN) - base) % RING_BYTES;
//     }

//     while (clock_time_exceed(t_next, FRAME_MS * 1000)){
//         t_next += FRAME_MS * 1000;
//         pump_one_frame_if_ready();
//         gpio_toggle(LED1);  // 每次循环时切换 LED 状态，用于调试

//     }

// }

