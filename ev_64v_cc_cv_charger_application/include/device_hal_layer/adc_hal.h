/*
 * adc_hal.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef DEVICE_HAL_LAYER_ADC_HAL_H_
#define DEVICE_HAL_LAYER_ADC_HAL_H_



typedef enum adc_hal_grp_channel_tag
{
	ADC_GRP_CHANNEL0 = 0,
	ADC_GRP_CHANNEL1,

	MAX_ADC_GRP_CHANNEL
}adc_hal_grp_channel;


typedef enum adc_hal_grp_instance_tag
{
	ADC0_GROUP0,

}adc_hal_grp_instance;


/*Public functions*/
extern unsigned char adc_hal_init(void);
extern unsigned char adc_hal_start_conversion_it(void);
extern unsigned char adc_hal_read_data(adc_hal_grp_instance  adc_grp_te, unsigned short *raw_adc_val_arrptr);

#endif /* DEVICE_HAL_LAYER_ADC_HAL_H_ */
