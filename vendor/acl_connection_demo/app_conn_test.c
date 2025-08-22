/********************************************************************************************************
 * @file    app_conn_text.c
 *
 * @brief   This is the source file for BLE SDK
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


// #include "app_config.h"
#include "app.h"
// #include "app_buffer.h"
// #include "app_ui.h"

static u16 feature_dle_mode[DEVICE_CHAR_INFO_MAX_NUM];
static u32 feature_test_tick = 0;
u16 dataLen = 20;
u16 array_pos = 0;
bool pwm_dir_flag = true;
u32 send_num = 0;
#define PWM_CMD_OUT_DP_H 61
static uint8_t app_test_data[5][20] = {
    { [0 ... 19] = 0x00 },  // 第0行：所有字节为0x00
    { [0 ... 19] = 0x01 },   // 第1行：所有字节为0x01
    { [0 ... 19] = 0x02 },
    { [0 ... 19] = 0x03 },
    { [0 ... 19] = 0x04 },
};

volatile bool xor_flag = 0;
volatile bool conn_flag = false;
bool judge_conn_State(){
    for(int i=0;i<MAX_TARGET_MAC_NUM;i++){
        if(conn_dev_list[i].conn_state == false){
            test_target_mac_num = i;
            return false;
        }
    }
    return true;
}
_attribute_ram_code_
int app_acl_central_post_event_callback(void){
    if(conn_flag && xor_flag){
        gpio_set_high_level(TEST_GPIO);
        
        extern ble_sts_t blc_ll_clear_central_tx_fifo(u16 connHandle);
        for(int i = 0; i < DEVICE_CHAR_INFO_MAX_NUM; i++) {
            if(conn_dev_list[i].conn_state == 1) {
                
                int n = blc_ll_getTxFifoNumber(conn_dev_list[i].conn_handle);
                if(n >= 2){
                    blc_ll_clear_central_tx_fifo(conn_dev_list[i].conn_handle);
                }
                ble_sts_t status = blc_gatt_pushWriteCommand(conn_dev_list[i].conn_handle, PWM_CMD_OUT_DP_H, app_test_data[array_pos], dataLen);
            }
        }
        pwm_dir_flag ? (array_pos++) : (array_pos--);
        
        if (pwm_dir_flag && array_pos >= 4) {
            pwm_dir_flag = false;   // 到达顶部，改为递减
        } else if (!pwm_dir_flag && array_pos == 0) {
            pwm_dir_flag = true;    // 到达底部，改为递增
        }

        xor_flag = false;
        gpio_set_low_level(TEST_GPIO);


    }
    else{
        gpio_set_low_level(TEST_GPIO);
        xor_flag = true;
    }
	return 0;
}

void app_conn_test_mainloop(void)
{
    u16 connHandle;
    if(!conn_flag && judge_conn_State()){
        conn_flag = true;
    }

    return;
    
}
