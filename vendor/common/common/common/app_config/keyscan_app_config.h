/********************************************************************************************************
 * @file    keyscan_app_config.h
 *
 * @brief   This is the header file for Telink RISC-V MCU
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#if defined(MCU_CORE_TL322X)
    #define KEYSCAN_TEST_SUSPEND 0

    #define SOURCE_32K_RC        0
    #define SOURCE_32K_XTAL      1

    #define KEYSCAN_32K_SOURCE   SOURCE_32K_RC

    #define KEYSCAN_NDMA         0
    #define KEYSCAN_DMA          1
    #define KEYSCAN_DMA_LLP      2

    #define KEYSCAN_MODE         KEYSCAN_NDMA
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
