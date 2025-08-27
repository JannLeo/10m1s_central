/********************************************************************************************************
 * @file    app_conn_test.c
 *
 * @brief   BLE connection test module source file
 *
 * @author  BLE GROUP
 * @date    06,2025
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"

/* Constants */
#define PWM_CMD_OUT_DP_H         61
#define SEND_RGB_SIGNAL_TIMES    3
#define MAX_TEST_DATA_SIZE       20
#define FIFO_THRESHOLD           2

/* Global Variables */
static u16 feature_dle_mode[MAX_TARGET_MAC_NUM];
static u32 feature_test_tick = 0;
u16 dataLen = MAX_TEST_DATA_SIZE;
u16 array_pos = 0;
bool pwm_dir_flag = true;
static u32 send_num = 0;

/* Test Data Patterns */
static uint8_t app_test_data[3][MAX_TEST_DATA_SIZE] = {
    { [0 ... (MAX_TEST_DATA_SIZE-1)] = 0x00 },  // Pattern 0: All zeros
    { [0 ... (MAX_TEST_DATA_SIZE-1)] = 0x01 },  // Pattern 1: All 0x01
    { [0 ... (MAX_TEST_DATA_SIZE-1)] = 0x02 },  // Pattern 2: All 0x02
};

/* Connection Flags */
volatile bool xor_flag = false;
volatile bool conn_flag = false;
extern ble_sts_t blc_ll_clear_central_tx_fifo(u16 connHandle);
/**
 * @brief Check if all target devices are connected
 * @return true if all devices connected, false otherwise
 */
bool judge_conn_state(void) {
    for (int i = 0; i < MAX_TARGET_MAC_NUM; i++) {
        if (conn_dev_list[i].conn_state == false) {
            test_target_mac_num = i;
            return false;
        }
    }
    return true;
}

/**
 * @brief Central event callback for ACL connection
 * @return 0 on success
 */
_attribute_ram_code_
int app_acl_central_post_event_callback(void) {
    if (conn_flag && xor_flag) {
        gpio_set_high_level(TEST_GPIO);
        
        /* Process all connected devices */
        for (int i = 0; i < MAX_TARGET_MAC_NUM; i++) {
            if (conn_dev_list[i].conn_state == 1) {
                /* Check and clear TX FIFO if needed */
                int fifo_count = blc_ll_getTxFifoNumber(conn_dev_list[i].conn_handle);
                if (fifo_count >= FIFO_THRESHOLD) {
                    blc_ll_clear_central_tx_fifo(conn_dev_list[i].conn_handle);
                }
                
                /* Send test data pattern */
                ble_sts_t status = blc_gatt_pushWriteCommand(
                    conn_dev_list[i].conn_handle, 
                    PWM_CMD_OUT_DP_H, 
                    app_test_data[array_pos], 
                    dataLen
                );
            }
        }
        
        /* Update pattern position */
        send_num++;
        if (send_num % SEND_RGB_SIGNAL_TIMES == 0) {
            array_pos = (array_pos + 1) % 3;
            send_num = 0;
        }

        xor_flag = false;
        gpio_set_low_level(TEST_GPIO);
    }
    else {
        gpio_set_low_level(TEST_GPIO);
        xor_flag = true;
    }
    
    return 0;
}

/**
 * @brief Main loop for connection test
 */
void app_conn_test_mainloop(void) {
    if (!conn_flag && judge_conn_state()) {
        conn_flag = true;
    }
}
