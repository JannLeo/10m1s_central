/********************************************************************************************************
 * @file    lin_app_config.h
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

#if defined(MCU_CORE_TL322X)
#define PULL_WAKEUP_SRC_PA2 GPIO_PIN_UP_DOWN_FLOAT
#define PULL_WAKEUP_SRC_PA3 GPIO_PIN_UP_DOWN_FLOAT

#define PULL_WAKEUP_SRC_PB2 GPIO_PIN_UP_DOWN_FLOAT
#define PULL_WAKEUP_SRC_PB3 GPIO_PIN_UP_DOWN_FLOAT
#endif

#include "driver.h"

#if defined(MCU_CORE_TL322X)
    #define LIN0_TX_PIN GPIO_FC_PA2
    #define LIN0_RX_PIN GPIO_FC_PA3

    #define LIN1_TX_PIN GPIO_FC_PB2
    #define LIN1_RX_PIN GPIO_FC_PB3

#endif

#define LIN0_TEST_TX_PIN    LIN0_TX_PIN
#define LIN0_TEST_RX_PIN    LIN0_RX_PIN

#define LIN1_TEST_TX_PIN    LIN1_TX_PIN
#define LIN1_TEST_RX_PIN    LIN1_RX_PIN

#define LIN_TEST_BAUDRATE   20000 /*1k~20k*/
#define LIN_TEST_TIMEBASE   LIN_TIME_BASE_5MS
#define LIN_TEST_BREAK_LEN  LIN_BREAK13
#define LIN_TEST_BREAK1_LEN LIN_BREAK1_1


#define LIN_TEST1           1 /*Test LIN Master Node*/
#define LIN_TEST2           2 /*Test LIN Master Node - using hardware schedule*/
#define LIN_TEST3           3 /*Test LIN Slave Node*/
#define LIN_TEST4           4 /*Test LIN Slave Node - using filter PID*/
#define LIN_TEST5           5 /*Test LIN0 Master Node, LIN1 Slave Node*/

#define LIN_TEST_MODE       LIN_TEST5


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
