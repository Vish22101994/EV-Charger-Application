/*
 * adc_mdl.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "adc_mdl.h"




static unsigned short read_raw_adc_value_arr[MAX_ADC_GRP_CHANNEL];


/*Function Definition*/
unsigned char adc_mdl_init(void);
unsigned char adc_mdl_read_feedback_votlage_current(void);
unsigned char adc_mdl_start_converstion(void);

/*********************************************************************
 *  VISHAL
 *  Description :  Initialize the AC peripheral on target MCU
 *
 ******************************************************************** */
unsigned char adc_mdl_init(void)
{

	unsigned char return_error = 0;

	return_error = adc_hal_init();

	return return_error;

}


/*********************************************************************
 *  VISHAL
 *  Description : Start ADC Conversion of Different Channel for Target MCU
 *
 ******************************************************************** */
unsigned char adc_mdl_start_converstion(void)
{

  unsigned char return_error = 0;

  return_error =  adc_hal_start_conversion_it();

  return return_error;
}


/*********************************************************************
 *  VISHAL
 *  Description : Read Analog input channel as raw value for target MCU
 *
 ******************************************************************** */
unsigned char adc_mdl_read_feedback_votlage_current(void)
{

	unsigned char  read_error_flag = 0;
	unsigned char  ret_err = 0;
	unsigned short temp_raw_adc;

 	read_error_flag = adc_hal_read_data(ADC0_GROUP0, read_raw_adc_value_arr);

	if(read_error_flag)
	{
		ret_err++;
	} else
	{
		/*Successfull read of ADC Conversion*/
		for(unsigned char channel_index  =  0 ; channel_index < MAX_ADC_GRP_CHANNEL ; channel_index++)
		{
			/*Reset the adc Raw Value */
			temp_raw_adc = 0;

			/*Get the Raw value from the adc raw buffer array*/
			temp_raw_adc =  read_raw_adc_value_arr[channel_index];

			switch(channel_index)
			{

				case ADC_GRP_CHANNEL0:
				{
					/*Current Measurement Reading
					 * Conversion : raw ---> scaled
					 * */
					if(temp_raw_adc < ADC_12BIT_FULL_RESOLUTION - ADC_RAW_ERROR_OFFSET)
					{
						/*TODO :  VISHAL
						 * Proper Unit Conversion into centiAmperes needs to be done*/
						temp_raw_adc =   temp_raw_adc*ADC_REF_VOLTAGE_CV;
						temp_raw_adc =   (temp_raw_adc/ ADC_12BIT_FULL_RESOLUTION);

						temp_raw_adc =   temp_raw_adc*FEEDBACK_CURRENT_MEASUREMENT_VOLTAGE_GAIN;
						temp_raw_adc =   temp_raw_adc/CURRENT_MESAUREMENT_SHUNT_RESISTANCE;

						measurement_param_gst.charger_output_current_ca =  temp_raw_adc; /*Scaled Value*/

					} else
					{
						/*Saturated ADC RAW Value (Will be considered as FAULTY VALUE )*/
						measurement_param_gst.charger_output_current_ca =  ADC_FAULT_VALUE;
					}
				}
				break;
				case ADC_GRP_CHANNEL1:
				{
					/*Voltage Measurement Reading
					 * Conversion : raw ----> scaled
					 * */
					if(temp_raw_adc < ADC_12BIT_FULL_RESOLUTION - ADC_RAW_ERROR_OFFSET)
					{
						/*TODO :  VISHAL
						 * Proper Unit Conversion into centivolts needs to be done*/
						temp_raw_adc =   temp_raw_adc*ADC_REF_VOLTAGE_CV;
						temp_raw_adc =   (temp_raw_adc/ ADC_12BIT_FULL_RESOLUTION);

						temp_raw_adc = temp_raw_adc*FEEDBACK_VOLTAGE_MESUREMENT_VOLTAGE_GAIN;

						measurement_param_gst.charger_terminal_voltage_cv =  temp_raw_adc;  /*Scaled Value*/
					} else
					{
						/*Saturated ADC RAW Value (Will be considered as FAULTY VALUE )*/
						measurement_param_gst.charger_terminal_voltage_cv =  ADC_FAULT_VALUE;
					}
				}
				break;
				default:
				{
					/*TODO*/
				}

			}
		}
	}



	return 0;
}

