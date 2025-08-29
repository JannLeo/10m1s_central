/********************************************************************************************************
 * @file    rram_app_config.h
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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C"
{
#endif

#define DSP_FW_DOWNLOAD_FLASH_ADDR 0x20040000
#define N22_FW_DOWNLOAD_FLASH_ADDR 0x20080000
#define N22_FW_DOWNLOAD_RRAM_ADDR  0x00540000

#define RRAM_TX_DMA_CHN            DMA0
#define RRAM_RX_DMA_CHN            DMA1

#define RRAM_COMMON                1
#define RRAM_LOAD                  2
#define RRAM_N22_LOAD              3
#define RRAM_MODE                  RRAM_COMMON

#define RRAM_D25F_TEST             1
#define RRAM_N22_TEST_D25F         2
#define RRAM_N22_TEST_N22          3
#define RRAM_COMMON_MODE           RRAM_D25F_TEST


#include "driver.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
