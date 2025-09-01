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
#include "tlkapi_debug.h"  // 需要这个声明

#define FRAME_MS         20
#define FRAME_US         (FRAME_MS * 1000)
#define SAMPLE_RATE_HZ   8000
#define BYTES_PER_SAMPLE 2
#define CHANNELS         1
#define FRAME_BYTES      (SAMPLE_RATE_HZ/1000 * FRAME_MS * BYTES_PER_SAMPLE * CHANNELS) // 320
#define RING_BYTES       (sizeof(AUDIO_BUFF))

// TLKAPI 文本模式：每字节会变成 " xx"，FIFO 有限，这里保守点每块发 <=80B
#define TLKAPI_CHUNK_BYTES 80

static uintptr_t prev_off = 0;

static inline uint32_t ring_delta(uintptr_t prev, uintptr_t now, uint32_t size){
    return (now >= prev) ? (now - prev) : (size - (prev - now));
}

static inline void send_pcm_chunked_via_tlkapi(const uint8_t *p, unsigned n){
    while(n){
        unsigned c = (n > TLKAPI_CHUNK_BYTES) ? TLKAPI_CHUNK_BYTES : n;
        // 第二个参数给个短前缀，或传 "" 最大化数据区
        tlkapi_send_string_data(1, "", (u8*)p, c);
        p += c; n -= c;
    }
}

static void pump_one_frame_if_ready(void){
    uintptr_t base = (uintptr_t)AUDIO_BUFF;
    uintptr_t w    = (uintptr_t)audio_get_rx_dma_wptr(RX_DMA_CHN);
    uintptr_t off  = (w - base) % RING_BYTES;

    uint32_t avail = ring_delta(prev_off, off, RING_BYTES);
    if (avail < FRAME_BYTES) return;

    uint32_t head_free = RING_BYTES - (prev_off % RING_BYTES);
    uint32_t first     = (head_free >= FRAME_BYTES) ? FRAME_BYTES : head_free;

    const uint8_t *p1 = (const uint8_t*)AUDIO_BUFF + (prev_off % RING_BYTES);
    send_pcm_chunked_via_tlkapi(p1, first);

    if (first < FRAME_BYTES){
        const uint8_t *p2 = (const uint8_t*)AUDIO_BUFF;
        send_pcm_chunked_via_tlkapi(p2, FRAME_BYTES - first);
    }
    prev_off = (prev_off + FRAME_BYTES) % RING_BYTES;
}

void main_loop_record_test(void){
    static unsigned int t_next = 0;
    if (t_next == 0){
        t_next = clock_time();
        // 首帧对齐：从当前写指针开始，避免把环里旧数据全倒出去
        uintptr_t base = (uintptr_t)AUDIO_BUFF;
        prev_off = ((uintptr_t)audio_get_rx_dma_wptr(RX_DMA_CHN) - base) % RING_BYTES;
    }
    while (clock_time_exceed(t_next, FRAME_US)){
        t_next += FRAME_US;
        pump_one_frame_if_ready();
    }

    // 关键：主循环里要跑 TLKAPI 的搬运函数，才能真正发出去
    tlkapi_debug_handler();  // 确保在你的主循环里被周期调用
}

