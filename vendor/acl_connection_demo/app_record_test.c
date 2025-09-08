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
#include "app_codec.h"
#include "app_record_test.h"
#define FRAME_MS         20
#define FRAME_US         (FRAME_MS * 1000)
#define SAMPLE_RATE_HZ   8000
#define BYTES_PER_SAMPLE 2
#define CHANNELS         1
#define FRAME_BYTES      (SAMPLE_RATE_HZ/1000 * FRAME_MS * BYTES_PER_SAMPLE * CHANNELS) // 320
#define HDR_BYTES 4
#define PKT_BYTES (HDR_BYTES + FRAME_BYTES)   // 4 + 320 = 324

static uint8_t tx_pkt[PKT_BYTES] __attribute__((aligned(4)));

static inline void make_pkt_320(const uint8_t *pcm320){
    tx_pkt[0] = 'P';
    tx_pkt[1] = 'C';
    tx_pkt[2] = (uint8_t)(FRAME_BYTES & 0xFF);
    tx_pkt[3] = (uint8_t)(FRAME_BYTES >> 8);
    memcpy(&tx_pkt[4], pcm320, FRAME_BYTES);
}


#define RING_BYTES (sizeof(AUDIO_BUFF))

static uintptr_t prev_off = 0;

static inline uint32_t ring_delta(uintptr_t prev, uintptr_t now, uint32_t size){
    return (now >= prev) ? (now - prev) : (size - (prev - now));
}

static void pump_one_frame_if_ready(void){
    uintptr_t base = (uintptr_t)AUDIO_BUFF;
    uintptr_t w    = (uintptr_t)audio_get_rx_dma_wptr(RX_DMA_CHN);
    uintptr_t off  = (w - base) % RING_BYTES;

    if (ring_delta(prev_off, off, RING_BYTES) < FRAME_BYTES) return;

    uint32_t head_free = RING_BYTES - (prev_off % RING_BYTES);
    const uint8_t *p1  = (const uint8_t*)AUDIO_BUFF + (prev_off % RING_BYTES);

    uint8_t frame[FRAME_BYTES];
    if (head_free >= FRAME_BYTES){
        memcpy(frame, p1, FRAME_BYTES);
    }else{
        memcpy(frame, p1, head_free);
        memcpy(frame + head_free, (const uint8_t*)AUDIO_BUFF, FRAME_BYTES - head_free);
    }
    prev_off = (prev_off + FRAME_BYTES) % RING_BYTES;

    make_pkt_320(frame);
}



