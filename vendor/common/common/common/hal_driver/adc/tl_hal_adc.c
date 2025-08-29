/********************************************************************************************************
 * @file    tl_hal_adc.c
 *
 * @brief   This is the source file for TL321X and TL323X
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#if defined(MCU_CORE_TL321X)
#if defined(MCU_CORE_TL321X)
#include "adc.h"
#endif
#if defined(MCU_CORE_TL323X)
#include "sd_adc.h"
#endif
#include "tl_hal_adc.h"
#include "compiler.h"
#include "lib/include/stimer.h"

unsigned char g_transfer_mode = 0;

/**
 * @brief      This function is used to initialize the ADC.
 * @param[in]  mode - transfer_mode
 * @param[in]  chn - the DMA channel
 * @return     none
 */
void tl_hal_adc_init(tl_hal_transfer_mode_e mode,dma_chn_e chn)
{
    g_transfer_mode = mode;
    if(mode == HAL_DMA)
    {

#if defined(MCU_CORE_TL321X)
        adc_set_dma_config(chn);
#elif defined(MCU_CORE_TL323X)
        sd_adc_set_dma_config(chn);
#endif
    }
#if defined(MCU_CORE_TL321X)
    adc_init((adc_chn_cnt_e)mode);
#elif defined(MCU_CORE_TL323X)
    sd_adc_init();
#endif
}


/**
 * @brief This function is used to initialize the ADC for gpio sampling.
 * @param[in]  pin - enum variable of ADC analog positive input IO.
 * @param[in]  prescale - enum variable of ADC pre_scaling factor.
 * @return none
 */
void tl_hal_adc_config(tl_hal_sample_mode_e mode, tl_hal_adc_gpio_input_e pin, tl_hal_adc_pre_scale_e prescale)
{
    if(mode == HAL_GPIO_MODE)
    {
#if defined(MCU_CORE_TL321X)
        adc_gpio_cfg_t adc_gpio_cfg =
            {
                .v_ref = ADC_VREF_1P2V,
                .pre_scale   = (adc_pre_scale_e)prescale,
                .sample_freq = ADC_SAMPLE_FREQ_23K,
                .pin         = (adc_input_pin_def_e)pin,
        };
        adc_gpio_sample_init(ADC_M_CHANNEL,adc_gpio_cfg);
#elif defined(MCU_CORE_TL323X)
        sd_adc_sample_cfg_t sd_adc_gpio_cfg =
        {
             .mode               = GPIO_SAMPLE_DC0,
             .clk_div            = SD_ADC_SAPMPLE_CLK_2M_DIV,
             .downsample_rate    = SD_ADC_DOWNSAMPLE_RATE_128,
             .cfg.gpio.input_p   = (sd_adc_p_input_pin_def_e)pin,
             .cfg.gpio.input_n   = SD_ADC_GNDN,
             .cfg.gpio.p_div     = (sd_adc_gpio_p_chn_div_e)prescale,
             .cfg.gpio.n_div     = (sd_adc_gpio_n_chn_div_e)prescale,
        };
        sd_adc_gpio_sample_init(&sd_adc_gpio_cfg);
        sd_adc_set_rx_fifo_trig_cnt(0);
#endif
    }
}

/**
 * @brief    This function is used to power on/off sar_adc.
 * @param[in] on - 1:on; 0:off.
 * @return   none.
 */
void tl_hal_adc_set_power(bool on)
{
    if(on)
    {
#if defined(MCU_CORE_TL321X)
    adc_power_on();
    delay_us(100); //After adc_power_on () waits for >100us, ADC samples are stable.
#elif defined(MCU_CORE_TL323X)
    sd_adc_power_on(SD_ADC_SAMPLE_MODE);
    sd_adc_sample_start();
#endif
    }else{
#if defined(MCU_CORE_TL321X)
    adc_power_off();
#elif defined(MCU_CORE_TL323X)
    sd_adc_sample_stop();
    sd_adc_power_off(SD_ADC_SAMPLE_MODE);
#endif
    }
}

/**
 * @brief    This function is used to sort and get average code.
 * @param[in] adc_data_buf  - Pointer to data buffer, it must be 4-bytes aligned address
 *                            and the actual buffer size defined by the user needs to be not smaller than the data_byte_len,
 *                            otherwise there may be an out-of-bounds problem.
 * @param[in] sample_cnt - sample number.
 * @return    adc code average.
 */
unsigned int tl_hal_adc_sort_and_get_average_code(unsigned int *sample_buffer, int sample_cnt)
{
    int i, j;
    unsigned int adc_code_average = 0;
    unsigned int temp;

    /**** insert Sort and get average value ******/
    for (i = 1; i < sample_cnt; i++) {
        if (sample_buffer[i] < sample_buffer[i - 1]) {
            temp                     = sample_buffer[i];
            sample_buffer[i] = sample_buffer[i - 1];
            for (j = i - 1; j >= 0 && sample_buffer[j] > temp; j--) {
                sample_buffer[j + 1] = sample_buffer[j];
            }
            sample_buffer[j + 1] = temp;
        }
    }
    //get average value from raw data(abandon 1/4 small and 1/4 big data)
    for (i = sample_cnt >> 2; i < (sample_cnt - (sample_cnt >> 2)); i++) {
        adc_code_average += sample_buffer[i] / (sample_cnt >> 1);
    }
    return adc_code_average;
}

/**
 * @brief      This function serves to get adc voltage value.
 * @param[in]  sample_cnt - sample number.
 * @return     adc_result   - adc voltage value.
 */
unsigned int tl_hal_adc_get_vol(unsigned int sample_cnt)
{
#if defined(MCU_CORE_TL321X)
    unsigned int cnt = 0;
    unsigned int code_average;
    unsigned short m_channel_buffer[sample_cnt] __attribute__((aligned(4)));
    unsigned int temp_buf[sample_cnt] __attribute__((aligned(4)));
    if (g_transfer_mode == HAL_NDMA) {
        adc_start_sample_nodma();
        while (cnt < sample_cnt) {
            int current_sample_num = adc_get_rxfifo_cnt();
            if (current_sample_num > 0) {
                m_channel_buffer[cnt] = adc_get_raw_code();
                if (m_channel_buffer[cnt] & BIT(11)) { //12 bit resolution, BIT(11) is sign bit, 1 means negative voltage in differential_mode
                    m_channel_buffer[cnt] = 0;
                } else {
                    m_channel_buffer[cnt] &= 0x7FF;    //BIT(10..0) is valid adc code
                }
                cnt++;
            }
        }
    }else if (g_transfer_mode == HAL_DMA) {
        adc_start_sample_dma((unsigned short *)m_channel_buffer, sample_cnt << 1);
        /******wait for adc sample finish********/
        while (!adc_get_sample_status_dma());
        /******clear adc sample finished status********/
        adc_clr_irq_status_dma();
    }
    for (unsigned int i = 0; i < sample_cnt; i++) {
        temp_buf[i] = (unsigned int)m_channel_buffer[i];
    }
    code_average = tl_hal_adc_sort_and_get_average_code((unsigned int *)temp_buf,sample_cnt);
    return (unsigned int)adc_calculate_voltage(0, code_average);
#elif defined(MCU_CORE_TL323X)

    unsigned int cnt = 0;
    unsigned int code_average;
    unsigned int sd_adc_sample_buf[sample_cnt] __attribute__((aligned(4)));

    if (g_transfer_mode == HAL_NDMA) {
        while (cnt < sample_cnt) {
            int current_sample_num = sd_adc_get_rxfifo_cnt();
            if (current_sample_num > 0) {
                sd_adc_sample_buf[cnt] = sd_adc_get_raw_code();
                cnt++;
            }
        }
    }else if (g_transfer_mode == HAL_DMA) {
        sd_adc_start_sample_dma((signed int *)sd_adc_sample_buf, sample_cnt<<2);
        /******wait for adc sample finish********/
        while (!sd_adc_get_sample_status_dma());
        /******clear adc sample finished status********/
        sd_adc_clr_irq_status_dma();
    }
    code_average = tl_hal_adc_sort_and_get_average_code(sd_adc_sample_buf,sample_cnt);
    return (unsigned int)sd_adc_calculate_voltage(code_average,SD_ADC_VOLTAGE_MV);
#endif


}
#endif
