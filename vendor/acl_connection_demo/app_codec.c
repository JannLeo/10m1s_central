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
#include "common.h"
#include "audio_common.h"
#define AUDIO_BUFF_SIZE 4096 * 2 /* In order to support codec data fade-in process, define enough buff */
//When the codec data bit width is selected as 16bit, the following buf is used,
//when the data bit width is selected as 20bit,
//you can define a buf with the same size of the signed int type for use.
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE >> 1] __attribute__((aligned(4)));
    #if defined(MCU_CORE_TL321X)
sdm_pin_config_t sdm_pin_config = {
    .sdm0_p_pin = GPIO_FC_PD5,
    .sdm0_n_pin = GPIO_FC_PE2,
    .sdm1_p_pin = GPIO_FC_PE3,
    .sdm1_n_pin = GPIO_FC_PA0, //Both the SDM and printf print functions use the PA0 pin. If the SDM function is used, modify the pin used for DEBUG_INFO_TX_PIN in printf.h.
};
    #endif

    #if  (AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) 

        #define SAMPLE_RATE AUDIO_8K
        #define DATA_WIDTH  CODEC_BIT_16_DATA
        #define RX_FIFO_NUM FIFO0
        #define TX_FIFO_NUM FIFO0 // TX Hardware is fixed to FIFO and cannot be modified.
        #define RX_DMA_CHN  DMA0
        #define TX_DMA_CHN  DMA1
        #if ((AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) || (AUDIO_MODE == DMA_IRQ_TEST))
            #define INPUT_SRC  AMIC_STREAM0_MONO_L
            #define OUTPUT_SRC SDM_MONO
        #endif

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
    #endif

void user_init_codec(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);

    audio_init();
    #if ((AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT) )
            /****setting the amic bias pin****/
            #if defined(MCU_CORE_TL321X)
    audio_set_amic_bias_pin(GPIO_PB4);
            #endif
    /****stream0 line in/amic/dmic init****/
    audio_codec_stream0_input_init(&audio_codec_stream0_input);
    /****line output init****/

    // audio_codec_stream_output_init(&audio_stream_output);

    /****rx tx dma init****/
    audio_rx_dma_chain_init(audio_codec_stream0_input.fifo_chn, audio_codec_stream0_input.dma_num, (unsigned short *)audio_codec_stream0_input.data_buf, audio_codec_stream0_input.data_buf_size);
    // audio_tx_dma_chain_init(TX_FIFO_NUM, audio_stream_output.dma_num, (unsigned short *)audio_stream_output.data_buf, audio_stream_output.data_buf_size);
        /****audio starts run****/
        #if (AUDIO_CLR_CODEC_POP == 1)
    audio_mic_mute_en();                                             /* Step1 - mute audio*/
        #endif
    audio_codec_stream0_input_en(audio_codec_stream0_input.dma_num); /* Step2 - enable audio codec */
        #if (AUDIO_CLR_CODEC_POP == 1)
    /* Please make sure the amic capacitor on the hardware has been changed to 10nF */
    audio_codec_clr_input_pop(20);                                 /* Step3 - Clear codec input pop and dis mute audio */
        #endif

    audio_codec_input_path_en(audio_codec_stream0_input.fifo_chn); /* Step4 - enable codec input path, codec data come in */
    audio_stream0_fade_dig_gain(CODEC_IN_D_GAIN_m6_DB);

        #if (AUDIO_CODEC_FADE_IN == 1)
    /* Collect enough codec data and make it fade in.
     * A delay of 5ms is used here in order to allow the codec to generate enough data for the fade-in process.
     * audio_sample_rate_value[audio_codec_stream0_input.sample_rate] / 1000 : number of data contained in 1ms at a given sample rate.
     * (AUDIO_MONO+1) : channel type, if AUDIO_MONO, * 1; if AUDIO_STEREO, * 2.
     * (audio_codec_stream0_input.data_width + 1) * 2 : data bit width, if 16bit, *2; if 20 or 24bit, *4.
     * (5) : 5ms - fade-in time.
    */
    extern unsigned int audio_sample_rate_value[AUDIO_ASCL_RATE_SIZE];
    while (((audio_get_rx_dma_wptr(RX_DMA_CHN)) - (unsigned int)audio_codec_stream0_input.data_buf) < audio_sample_rate_value[audio_codec_stream0_input.sample_rate] / 1000 * (AUDIO_MONO + 1) * (audio_codec_stream0_input.data_width + 1) * 2 * (5))
        ;
    /* 0ms ~ 5ms : fade-in process */
    audio_linear_fade_in_config(AUDIO_MONO, audio_codec_stream0_input.data_width, audio_codec_stream0_input.sample_rate, (char *)audio_codec_stream0_input.data_buf, 0, 5);
        #endif
    audio_set_sdm_pin(&sdm_pin_config);
    // audio_codec_stream_output_en(audio_stream_output.dma_num);
    #endif

}

volatile unsigned int dma_irq_ptr_test[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
void main_loop_codec(void)
{
    gpio_toggle(LED1);
    delay_ms(200);
    dma_irq_ptr_test[0] = audio_get_rx_dma_wptr(RX_DMA_CHN) - (unsigned int)AUDIO_BUFF; //dma_irq_ptr_test[0]=dma_irq_ptr_test[1]
    dma_irq_ptr_test[1] = audio_get_rx_wptr(RX_FIFO_NUM);
    printf("dma_irq_ptr_test[0]=%d, dma_irq_ptr_test[1]=%d\r\n", dma_irq_ptr_test[0], dma_irq_ptr_test[1]);
}

