/********************************************************************************************************
 * @file    pm_app_config.h
 *
 * @brief   This is the header file for Telink RISC-V MCU
 *
 * @author  Driver Group
 * @date    2019
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

#define WAKEUP_PAD       GPIO_PA0
#if defined(MCU_CORE_TL751X)
#define LPC_WAKEUP_PAD   LPC_INPUT_PG1
#else
#define LPC_WAKEUP_PAD   LPC_INPUT_PB1
#endif
#define CURRENT_TEST     1
#define CRC_OK           1
#define MDEC_MATCH_VALUE 0x02

#if defined(MCU_CORE_B91)
    #define PM_TICK_STIMER PM_TICK_STIMER_16M
#endif

#define CORE_SINGLE 1
#define CORE_MULTI  2
#if defined(MCU_CORE_TL751X) || defined(MCU_CORE_TL322X)
    #define CORE_MODE CORE_MULTI
#else
    #define CORE_MODE CORE_SINGLE
#endif

#define N22_TEST                   0
#define DSP_TEST                   0
#define DSP_FW_DOWNLOAD_FLASH_ADDR 0x20040000
#define N22_FW_DOWNLOAD_FLASH_ADDR 0x20080000

#if defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL7518) || defined(MCU_CORE_TL751X) || defined(MCU_CORE_TL322X)
    #define PM_SET_DVDD_MODE 01
#endif

/**
 * @note To enter sleep using COMPARATOR Wake mode, the voltage difference between the input level and the configured wake level
 *       needs to be greater than 100mV. If the input level is particularly close to the wake level, the chip will not sleep properly
 *       due to the unstable state of the LPC, resulting in a crash.
 */

/* SUSPEND MODE */
//[eagle]The A0 version of the suspend execution process is abnormal and the program restarts.
#define SUSPEND_PAD_WAKEUP        10
#define SUSPEND_32K_RC_WAKEUP     11
#define SUSPEND_32K_XTAL_WAKEUP   12
#define SUSPEND_COMPARATOR_WAKEUP 13
#define SUSPEND_CORE_USB_WAKEUP   14
#if defined(MCU_CORE_B91)
    #define SUSPEND_MDEC_WAKEUP   15
#endif
#if defined(MCU_CORE_B92)
    // CTB mode : For internal testing only, this function is not available externally
    #define SUSPEND_CTB_WAKEUP    16
//#define SUSPEND_CORE_GPIO_WAKEUP        17
#endif
#if defined(MCU_CORE_TL751X)
    #define SUSPEND_WT_WAKEUP     18
#endif

/* DEEP SLEEP MODE */
#define DEEP_PAD_WAKEUP           20
#define DEEP_32K_RC_WAKEUP        21
#define DEEP_32K_XTAL_WAKEUP      22
#define DEEP_COMPARATOR_WAKEUP    23
#if defined(MCU_CORE_B91)
    #define DEEP_MDEC_WAKEUP      24
#endif
#if defined(MCU_CORE_B92)
    // CTB mode : For internal testing only, this function is not available externally
    #define DEEP_CTB_WAKEUP       25
#endif
#if defined(MCU_CORE_TL751X)
    #define DEEP_WT_WAKEUP        26
#endif


/* DEEP SLEEP WITH RETENTION MODE */
#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL322X)
    #define DEEP_RET32K_PAD_WAKEUP          30
    #define DEEP_RET32K_32K_RC_WAKEUP       31
    #define DEEP_RET32K_32K_XTAL_WAKEUP     32
    #define DEEP_RET32K_COMPARATOR_WAKEUP   33
#endif
#if defined(MCU_CORE_B91)
    #define DEEP_RET32K_MDEC_WAKEUP         34
#endif
#if defined(MCU_CORE_B92)
    // CTB mode : For internal testing only, this function is not available externally
    #define DEEP_RET32K_CTB_WAKEUP          35
#endif

#if defined(MCU_CORE_B91) || defined(MCU_CORE_B92) || defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL322X)
    #define DEEP_RET64K_PAD_WAKEUP          40
    #define DEEP_RET64K_32K_RC_WAKEUP       41
    #define DEEP_RET64K_32K_XTAL_WAKEUP     42
    #define DEEP_RET64K_COMPARATOR_WAKEUP   43
#endif
#if defined(MCU_CORE_B91)
    #define DEEP_RET64K_MDEC_WAKEUP         44
#endif
#if defined(MCU_CORE_B92)
    // CTB mode : For internal testing only, this function is not available externally
    #define DEEP_RET64K_CTB_WAKEUP          45
#endif

#if defined(MCU_CORE_B92) || defined(MCU_CORE_TL321X)
    #define DEEP_RET96K_PAD_WAKEUP          50
    #define DEEP_RET96K_32K_RC_WAKEUP       51
    #define DEEP_RET96K_32K_XTAL_WAKEUP     52
    #define DEEP_RET96K_COMPARATOR_WAKEUP   53
#endif
#if defined(MCU_CORE_B92)
    // CTB mode : For internal testing only, this function is not available externally
    #define DEEP_RET96K_CTB_WAKEUP          54
#endif

#if defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL751X) || defined(MCU_CORE_TL322X)
    #define DEEP_RET128K_PAD_WAKEUP         60
    #define DEEP_RET128K_32K_RC_WAKEUP      61
    #define DEEP_RET128K_32K_XTAL_WAKEUP    62
    #define DEEP_RET128K_COMPARATOR_WAKEUP  63
#endif
#if defined(MCU_CORE_TL751X)
    #define DEEP_RET128K_WT_WAKEUP          64
#endif

#if defined(MCU_CORE_TL721X) || defined(MCU_CORE_TL751X) || defined(MCU_CORE_TL322X)
    #define DEEP_RET256K_PAD_WAKEUP         70
    #define DEEP_RET256K_32K_RC_WAKEUP      71
    #define DEEP_RET256K_32K_XTAL_WAKEUP    72
    #define DEEP_RET256K_COMPARATOR_WAKEUP  73
#endif
#if defined(MCU_CORE_TL751X)
    #define DEEP_RET256K_WT_WAKEUP          74
#endif

#if defined(MCU_CORE_TL751X) || defined(MCU_CORE_TL322X)
    #define DEEP_RET384K_PAD_WAKEUP         80
    #define DEEP_RET384K_32K_RC_WAKEUP      81
    #define DEEP_RET384K_32K_XTAL_WAKEUP    82
    #define DEEP_RET384K_COMPARATOR_WAKEUP  83
#endif
#if defined(MCU_CORE_TL751X)
    #define DEEP_RET384K_WT_WAKEUP          84
#endif

#define PM_MODE                             DEEP_PAD_WAKEUP

#if defined(MCU_CORE_B91)
    #if (PM_MODE == SUSPEND_32K_XTAL_WAKEUP || PM_MODE == DEEP_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP)
        #define PULL_WAKEUP_SRC_PD0         GPIO_PIN_UP_DOWN_FLOAT
        #define PULL_WAKEUP_SRC_PD1         GPIO_PIN_UP_DOWN_FLOAT
    #endif
#elif defined(MCU_CORE_B92)
    #if (PM_MODE == SUSPEND_32K_XTAL_WAKEUP || PM_MODE == DEEP_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET96K_32K_XTAL_WAKEUP)
        #define PULL_WAKEUP_SRC_PD0         GPIO_PIN_UP_DOWN_FLOAT
        #define PULL_WAKEUP_SRC_PD1         GPIO_PIN_UP_DOWN_FLOAT
    #endif
#elif defined(MCU_CORE_TL721X)
    #if (PM_MODE == SUSPEND_32K_XTAL_WAKEUP || PM_MODE == DEEP_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET128K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET256K_32K_XTAL_WAKEUP)
        #define PULL_WAKEUP_SRC_PD2         GPIO_PIN_UP_DOWN_FLOAT
        #define PULL_WAKEUP_SRC_PD3         GPIO_PIN_UP_DOWN_FLOAT
    #endif
#elif defined(MCU_CORE_TL321X)
    #if (PM_MODE == SUSPEND_32K_XTAL_WAKEUP || PM_MODE == DEEP_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET96K_32K_XTAL_WAKEUP)
        #define PULL_WAKEUP_SRC_PC2         GPIO_PIN_UP_DOWN_FLOAT
        #define PULL_WAKEUP_SRC_PC3         GPIO_PIN_UP_DOWN_FLOAT
    #endif
#elif defined(MCU_CORE_TL322X)
    #if (PM_MODE == SUSPEND_32K_XTAL_WAKEUP || PM_MODE == DEEP_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET128K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET256K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET384K_32K_XTAL_WAKEUP)
//        #define PULL_WAKEUP_SRC_PC2         GPIO_PIN_UP_DOWN_FLOAT
//        #define PULL_WAKEUP_SRC_PC3         GPIO_PIN_UP_DOWN_FLOAT
    #endif
#endif

#include "driver.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
