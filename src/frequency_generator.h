// Function generator Sin
// (C) Helmut Weber
//
// Provided as is as example
// Free for everyone
//
// 50 Hz - 200000Hz:     frequency error ~ 1%
// from 17 Hz - 250000   possible
 
 
// based on:
// https://github.com/krzychb/dac-cosine
 
// clk_8m_div and frequency_step are computed to give the best result for the wished frequency
 
 
 
/* DAC Cosine Generator Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
 
#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc.h"
 
 
#include "driver/dac.h"
 
/* Declare global sine waveform parameters
 * so they may be then accessed and changed from debugger
 * over an JTAG interface
 */
 
// Esp32 @ 240 MHz:
 
// f=125.6*step/(div+1)            [div=0..7] [calc: (div+1)]
 
// step0:
// 0=15.61Hz 1=--
 
// step 1:
// 0 = 125.6 Hz 1 = 62.50 Hz 2 = 41.67Hz 3=31.35Hz 4=25.00Hz 5=20.92Hz 6=17.86Hz 7=15.63Hz
 
// step 2:125,6
 
// 0= 250.0 Hz 1=125Hz 2=83.68 Hz 3=62.5 Hz 4=50.1 Hz 5=41.74Hz  6=35.74 Hz  7=31.35Hz
 
 // step 10
 // 0 = 1.248 kHz 1 = 625.4 Hz 2 = 417.4 Hz (noisy) 3 = 31.33 Hz 4 = 250.6  5 = 208.3 Hz  6=178.9 7=156.3
 
 // step 100:
 // 0: 12.53 kHz  1 = 6.266 kHz 2=4.173kHz 3= 3.127 kHz 4=2.505 kHz 6=1.789 kHz 7=1.565KHz
 
 
// step 500:
// 0: 62.54 kHz (noisy) 1 = 31.33 kHz 2 = 20.86 kHz  5 = 10.5 kHz   6 =20.88 KhZ  7=
 
// step 1000:
// 0 = 125 kHz, 1 = 62.11 kHz, 5 = 20.73 kHz    ==>>> 10 = 42.02 kHz 100
 
// step 2000
// 0: 249.4 kHz(unten verzerrt)  1 = 125.8 kHz (noisy)  5 = 41.5 kHz (noisy)
 
int clk_8m_div = 1;      // RTC 8M clock divider (division is by clk_8m_div+1, i.e. 0 means 8MHz frequency)
int frequency_step = 7;  // Frequency step for CW generator
int scale = 1;           // 50% of the full scale // scale 0 == fill scale = bad signal (no longer a sine wave)
int offset;              // leave it default / 0 = no any offset
int invert = 2;          // invert MSB to get sine waveform  // 3: invert 180°
 
 
/*
 * Enable cosine waveform generator on a DAC channel
 */
void dac_cosine_enable(dac_channel_t channel)
{
    // Enable tone generator common to both channels
    SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
    switch(channel) {
        case DAC_CHANNEL_1:
            // Enable / connect tone tone generator on / to this channel
            SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
            // Invert MSB, otherwise part of waveform will have inverted
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 2, SENS_DAC_INV1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN2_M);
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, 2, SENS_DAC_INV2_S);
            break;
        default :
           printf("Channel %d\n", channel);
    }
}
 
 
/*
 * Set frequency of internal CW generator common to both DAC channels
 *
 * clk_8m_div: 0b000 - 0b111
 * frequency_step: range 0x0001 - 0xFFFF
 *
 */
void dac_frequency_set(int clk_8m_div, int frequency_step)
{
    REG_SET_FIELD(RTC_CNTL_CLK_CONF_REG, RTC_CNTL_CK8M_DIV_SEL, clk_8m_div);
    SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL1_REG, SENS_SW_FSTEP, frequency_step, SENS_SW_FSTEP_S);
}
 
 
/*
 * Scale output of a DAC channel using two bit pattern:
 *
 * - 00: no scale
 * - 01: scale to 1/2
 * - 10: scale to 1/4
 * - 11: scale to 1/8
 *
 */
void dac_scale_set(dac_channel_t channel, int scale)
{
    switch(channel) {
        case DAC_CHANNEL_1:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE1, scale, SENS_DAC_SCALE1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE2, scale, SENS_DAC_SCALE2_S);
            break;
        default :
           printf("Channel %d\n", channel);
    }
}
 
 
/*
 * Offset output of a DAC channel
 *
 * Range 0x00 - 0xFF
 *
 */
void dac_offset_set(dac_channel_t channel, int offset)
{
    switch(channel) {
        case DAC_CHANNEL_1:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC1, offset, SENS_DAC_DC1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC2, offset, SENS_DAC_DC2_S);
            break;
        default :
           printf("Channel %d\n", channel);
    }
}
 
 
/*
 * Invert output pattern of a DAC channel
 *
 * - 00: does not invert any bits,
 * - 01: inverts all bits,
 * - 10: inverts MSB,
 * - 11: inverts all bits except for MSB
 *
 */
void dac_invert_set(dac_channel_t channel, int invert)
{
    switch(channel) {
        case DAC_CHANNEL_1:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, invert, SENS_DAC_INV1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, invert, SENS_DAC_INV2_S);
            break;
        default :
           printf("Channel %d\n", channel);
    }
}
 
/*
 * Main task that let you test CW parameters in action
 *
*/
void dactask(void* arg)
{
double f, f_target, delta, delta_min=9999999.0;
int step_target=0, divi_target=0;
 
  //From experiments:  f=125.6*step/(div+1)
 
    // this frequency is wanted:
    f_target=127000.0;                             // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
     
 
    // check all combinations of step iand divi to get the best guess:
    for (int step=1;step<2000; step++) {
      for (int divi=0; divi<8; divi++) {
        f=125.6*(double)step/(double)(divi+1);
        delta= abs((f_target-f));
        if (delta  < delta_min) {delta_min=delta; step_target=step; divi_target=divi; }
      }
    }
   
    clk_8m_div=divi_target;
    frequency_step=step_target;
   
    printf("divi=%d step=%d\n",divi_target, step_target);
    while(1){
 
        // frequency setting is common to both channels
        dac_frequency_set(clk_8m_div, frequency_step);
 
        /* Tune parameters of channel 2 only
         * to see and compare changes against channel 1
         */
        dac_scale_set(DAC_CHANNEL_2, scale);
        dac_offset_set(DAC_CHANNEL_2, offset);
        dac_invert_set(DAC_CHANNEL_2, invert);
 
        float frequency = RTC_FAST_CLK_FREQ_APPROX / (1 + clk_8m_div) * (float) frequency_step / 65536;
        printf("THEORIE:   clk_8m_div: %d, frequency step: %d, frequency: %.0f Hz\n", clk_8m_div, frequency_step, frequency);
        printf("PRACTICAL: frequency: %.0f Hz\n", 125.6*(float)frequency_step /(1 + (float)clk_8m_div) );
       
        printf("DAC2 scale: %d, offset %d, invert: %d\n", scale, offset, invert);
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}
 
 
/*
 * Generate a sine waveform on both DAC channels:
 *
 * DAC_CHANNEL_1 - GPIO25
 * DAC_CHANNEL_2 - GPIO26
 *
 * Connect scope to both GPIO25 and GPIO26
 * to observe the waveform changes
 * in response to the parameter change
*/
void app_main()
{
    dac_cosine_enable(DAC_CHANNEL_1);
    dac_cosine_enable(DAC_CHANNEL_2);
 
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
 
    xTaskCreate(dactask, "dactask", 1024*3, NULL, 10, NULL);
}
 