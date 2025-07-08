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
u8 flag = 0;
#define PWM_CMD_OUT_DP_H 61
static uint8_t app_test_data[2][20] = {
    { [0 ... 19] = 0x00 },  // 第0行：所有字节为0x00
    { [0 ... 19] = 0x01 }   // 第1行：所有字节为0x01
};
bool judge_conn_State(){
    for(int i=0;i<ACL_CENTRAL_MAX_NUM;i++){
        if(conn_dev_list[i].conn_state == false){
            test_target_mac_num = i;
            return false;
        }
    }
    return true;
}
void app_conn_test_mainloop(void)
{
    u16 connHandle;
    static u16 send_cnt =0;
    gpio_function_en(TEST_GPIO);
    gpio_output_en(TEST_GPIO);
    gpio_input_dis(TEST_GPIO);       // 禁用输入

    // loop for all device
    static u32 tick_loop= 0;
    if(!judge_conn_State()){
        flag = 1;
        for (u8 i = 0; i < (DEVICE_CHAR_INFO_MAX_NUM); i++) {
            // check connect state
            if (conn_dev_list[i].conn_state == 1) {                                   //connect state
                connHandle = conn_dev_list[i].conn_handle;
                //send data to peer device
                if (i < ACL_CENTRAL_MAX_NUM) {
                    u8  ret_val = BLE_SUCCESS;
                    if(i == 0 && conn_dev_list[ACL_CENTRAL_MAX_NUM - 1].conn_state == 1){
                        gpio_set_high_level(TEST_GPIO);
                    }else{
                        gpio_set_low_level(TEST_GPIO);
                    }
                    //0x0039 is the handle value of the current test ACL Peripheral, which can be modified by the client according to their actual situation
                    ret_val = blc_gatt_pushWriteCommand(connHandle, PWM_CMD_OUT_DP_H, app_test_data[flag], dataLen);
                    // tlkapi_printf(APP_LOG_EN, "[APP][GATT] blc_gatt_pushWriteCommand     connHandle:%04X status:%02X", connHandle, ret_val);

                }
            }
        }
    }
    else if(clock_time_exceed(tick_loop,30*1000)){
        tick_loop = clock_time();
        send_cnt++;
        if (send_cnt%2) {
            flag = 1;  // 填充数据为 1
        } else {
            flag = 0;  // 填充数据为 0
        }
        for (u8 i = 0; i < (DEVICE_CHAR_INFO_MAX_NUM); i++) {
            // check connect state
            // 使用 memset 填充数据
            if (conn_dev_list[i].conn_state == 1) {                                   //connect state
                connHandle = conn_dev_list[i].conn_handle;
                //send data to peer device
                if (i < ACL_CENTRAL_MAX_NUM) {
                    u8  ret_val = BLE_SUCCESS;
                    if(i == 0 && conn_dev_list[ACL_CENTRAL_MAX_NUM - 1].conn_state == 1){
                        gpio_set_high_level(TEST_GPIO);
                    }else{
                        gpio_set_low_level(TEST_GPIO);
                    }
                    //0x0039 is the handle value of the current test ACL Peripheral, which can be modified by the client according to their actual situation
                    ret_val = blc_gatt_pushWriteCommand(connHandle, PWM_CMD_OUT_DP_H, app_test_data[flag], dataLen);
                    if(ret_val != BLE_SUCCESS){
                        tlkapi_printf(APP_LOG_EN, "[APP][TEST] blc_gatt_pushWriteCommand     connHandle:%04X status:%02X \r\n", connHandle, ret_val);
                        gpio_set_high_level(TEST_GPIO);
                        gpio_set_low_level(TEST_GPIO);
                    }
                    

                }
            }
        }
    }
    
    return;
    
}
