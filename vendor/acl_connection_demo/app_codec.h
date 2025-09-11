/********************************************************************************************************
 * @file    audio_app_config_1v2.h
 *
 * @brief   This is the header file for Telink RISC-V MCU
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once


#if defined(__cplusplus)
extern "C"
{
#endif

#include "driver.h"

#define AUDIO_BUFF_SIZE 2048 * 2 /* In order to support codec data fade-in process, define enough buff */
extern signed short AUDIO_BUFF[AUDIO_BUFF_SIZE >> 1];

#define SAMPLE_RATE AUDIO_8K
#define DATA_WIDTH  CODEC_BIT_16_DATA
#define RX_FIFO_NUM FIFO0
#define TX_FIFO_NUM FIFO0 // TX Hardware is fixed to FIFO and cannot be modified.
#define RX_DMA_CHN  DMA4
#define TX_DMA_CHN  DMA5
#if ((AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == DMA_IRQ_TEST))
    #define INPUT_SRC  AMIC_STREAM0_MONO_L
    #define OUTPUT_SRC SDM_MONO
 #endif
#define LED1 GPIO_PD0

/* audio codec case */
#define LINE_INPUT_TO_BUF_TO_LINEOUT 1 //line_in->buff->SDM out
#define AMIC_INPUT_TO_BUF_TO_LINEOUT 2 //amic_in->buff->SDM out
#define DMIC_INPUT_TO_BUF_TO_LINEOUT 3 //dmic_in->buff->SDM out
#define BUFFER_TO_LINEOUT            4 //buff to line_out
#define DMA_IRQ_TEST                 5 //DMA llp_irq
/* audio i2s case */
#define I2S_TO_EXT_CODEC_WM    6  //I2S input->buff->I2S out for WM8731 external codec
#define I2S_OUTPUT_DOUBLE_BUFF 7  //I2S(I2S0/I2S1/I2S2) outputs double buff data with double tx FIFO.
#define I2S2_TDM_MODE_TEST     8  //I2S2 TDM mode
#define I2S_TO_2LANE_RX        9  //2lane data rx
#define I2S_TO_2LANE_TX        10 //2lane data tx
#define I2S_TO_TARGET          11 //Reach target tick value to enable i2s
#define I2S_ASCl_TEST          12 //I2S Sample rate conversion ( up-sampling only)
#define AUDIO_FIFO_IRQ_TEST    13 //FIFO threshold irq

/* audio mix case */
#define DMIC_INPUT_TO_BUF_TO_I2S 15 //dmic_in->buff->I2S out

#define AUDIO_MODE               AMIC_INPUT_TO_BUF_TO_LINEOUT

#if ((AUDIO_MODE == LINE_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == DMIC_INPUT_TO_BUF_TO_LINEOUT))
/* Comment this macro if not used. */
//#define    AUDIO_CODEC_POWER_TEST        1 //codec power off, power on and reinit loop test
#endif

/* There are 4 steps in order to clear codec pop:
 * Step1 - mute audio;
 * Step2 - enable audio codec;
 * Step3 - Clear codec input pop and dis mute audio;
 * Step4 - enable codec input path, codec data come in.
 */
#define AUDIO_CLR_CODEC_POP 1

/* Collect enough codec data and make it fade in. */
#define AUDIO_CODEC_FADE_IN 0


void user_init_codec(void);

void main_loop_codec(void);
#if defined(__cplusplus)
}
#endif
