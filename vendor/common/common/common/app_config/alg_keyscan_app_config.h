/********************************************************************************************************
 * @file    alg_keyscan_app_config.h
 *
 * @brief   This is the header file for Telink RISC-V MCU
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "driver.h"

#define KEYSCAN_TEST_1XADC                  0//KEYSCAN_1XADC_MODE
#define KEYSCAN_TEST_2XADC                  1//KEYSCAN_2XADC_MODE
#define KEYSCAN_TEST_1XADC_8K_ONCE          2//KEYSCAN_1XADC_8K_ONCE_MODE
#define KEYSCAN_TEST_2XADC_8K_ONCE          3//KEYSCAN_2XADC_8K_ONCE_MODE
#define KEYSCAN_TEST_2XADC_8K_TWICE_M1      4//KEYSCAN_2XADC_8K_TWICE_M1_MODE
#define KEYSCAN_TEST_2XADC_8K_TWICE_M2      5//KEYSCAN_2XADC_8K_TWICE_M2_MODE

#define KS_TEST_MODE                        KEYSCAN_TEST_2XADC_8K_ONCE

#define KS_DMA_LLP_ENABLE                   1

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
