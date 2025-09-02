/********************************************************************************************************
 * @file    app_codec.c
 *
 * @brief   This is the source file for Telink RISC-V MCU
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
// #include "common.h"
// #include "audio_common.h"
#include "app_codec.h"

//When the codec data bit width is selected as 16bit, the following buf is used,
//when the data bit width is selected as 20bit,
//you can define a buf with the same size of the signed int type for use.
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE >> 1] __attribute__((aligned(4)));




audio_codec_stream0_input_t audio_codec_stream0_input =
    {
        .input_src     = INPUT_SRC,
        .sample_rate   = SAMPLE_RATE,
        .data_width    = DATA_WIDTH,
        .fifo_chn      = RX_FIFO_NUM,
        .dma_num       = RX_DMA_CHN,
        .data_buf      = AUDIO_BUFF,
        .data_buf_size = sizeof(AUDIO_BUFF),
};

void user_init_codec(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);

    audio_init();
            /****setting the amic bias pin****/
    audio_set_amic_bias_pin(GPIO_PC2);
    /****stream0 line in/amic/dmic init****/
    audio_codec_stream0_input_init(&audio_codec_stream0_input);
    /****line output init****/

    // audio_codec_stream_output_init(&audio_stream_output);

    /****rx tx dma init****/
    audio_rx_dma_chain_init(audio_codec_stream0_input.fifo_chn, audio_codec_stream0_input.dma_num, (unsigned short *)audio_codec_stream0_input.data_buf, audio_codec_stream0_input.data_buf_size);
    // audio_tx_dma_chain_init(TX_FIFO_NUM, audio_stream_output.dma_num, (unsigned short *)audio_stream_output.data_buf, audio_stream_output.data_buf_size);
        /****audio starts run****/
    audio_mic_mute_en(); 
    audio_codec_stream0_input_en(audio_codec_stream0_input.dma_num); /* Step2 - enable audio codec */
    audio_codec_clr_input_pop(20);
    audio_codec_input_path_en(audio_codec_stream0_input.fifo_chn); /* Step4 - enable codec input path, codec data come in */
    // audio_stream0_fade_dig_gain(CODEC_IN_D_GAIN_m6_DB);
    // audio_set_sdm_pin(&sdm_pin_config);
    // audio_codec_stream_output_en(audio_stream_output.dma_num);
    
    audio_set_adc_pga_gain(CODEC_IN_GAIN_21P0_DB);      // 20 dB
    audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_0_DB); // +6 dB

}

volatile unsigned int dma_irq_ptr_test[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
void main_loop_codec(void)
{
    gpio_toggle(LED1);
    delay_ms(200);
}

