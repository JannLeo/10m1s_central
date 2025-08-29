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

#define I2S_DEMO           (1)
#define CODEC_DEMO         (2)
#define ASRC_DEMO          (3)
#define AUDIO_MODE CODEC_DEMO

void user_init_codec(void);
void main_loop_codec(void);
#if defined(__cplusplus)
}
#endif
