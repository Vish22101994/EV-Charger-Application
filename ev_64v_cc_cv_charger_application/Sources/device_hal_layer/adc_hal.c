/*
 * adc_hal.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "adc_hal.h"






unsigned short adc0_grp0_arr[MAX_ADC_GRP_CHANNEL];

volatile adc_hal_adc0_grp0_conv_cplt_flag = 0;

/*Function Definition*/

unsigned char adc_hal_init(void);
unsigned char adc_hal_start_conversion_it(void);
unsigned char adc_hal_read_data(adc_hal_grp_instance  adc_grp_te, unsigned short *raw_adc_val_arrptr);
void adc_hal_adc0_clbk_grp0_v(void * parameter);

/*********************************************************************
 *  VISHAL
 *  Description : MCU ADC peripheral intialization
 *
 ******************************************************************** */
unsigned char adc_hal_init(void)
{

	unsigned char return_error = 0;

	/* ADC peripheral Initialization
	 * For this project, nxp PAL layer (FOR NXP MCU) will be used to do the initialization of the peripheral where the following configuration
	 * PDB0 :  Programmable Delay Block 0 will be used together with ADC0  to achieve the sequencial conversion
	 * 	     	[the process will varies with Different MCU and its support SDKs]
 	 * ADC0 :  ADC0  instance will be used to do the raw conversion of signal (0-3.3v)
 	 * In this Flavour of SDK API, to do the sequencial conversion of different channels
 	 * GRP0 :  CH0 and CH1
	 * and parameter will be passed as an argument through a structure:
	 * Trigger:  Software Trigger
	 * Sample Ticks for conversion :  50
	 * GROUP CHANNEL DELAY :  No DELAY
	 * Input Clock to the peripheral prescaler: 1
	 * Votlage reference for the ADC module :  3.3v to 0v
	 * callback  :  adc_hal_adc0_clbk_grp0_v(void * parameter) will be passsed as function pointer
	 * ADC Resolution : 12bit
	 * ADC_RESULT_ARRAY Pointer : @adc0_grp0_arr[]
	 * */

	/*Remove the  below Commented code , once this application integrated with target based project*/

//	return_error = sdk_adc_init(&adc_config)

	return return_error;

}



/*********************************************************************
 *  VISHAL
 *  Description : starts the adc group , by callling this API
 *
 ******************************************************************** */
unsigned char adc_hal_start_conversion_it(void)
{



	unsigned char return_error = 0;

	/* Start the ADC COnversion by calling a respective API in SDI
	 * Following API will start the conversion for the particular gorup which  has been initialized
	 * Associated pre-trigger will be configured 1 to 1 , before giving software trigger to the PDB input channel to start the conversion
	 * */

	/*Remove the  below Commented code , once this application integrated with target based project*/

//	return_error = SDK_ADC_StartGroupConversion(&adc_pal_0_instance, ADC0_GROUP_0);

	return return_error;
}


/*********************************************************************
 *  VISHAL
 *  Description :  adc read api from the resultbuffer
 *
 ******************************************************************** */
unsigned char adc_hal_read_data(adc_hal_grp_instance  adc_grp_te, unsigned short *raw_adc_val_arrptr)
{
	unsigned char return_error = 0;

	switch (adc_grp_te)
	{
		case ADC0_GROUP0:
		{

			/*Check wether the conversion is completed or not*/
			if (adc_hal_adc0_grp0_conv_cplt_flag)
			{
				/* ADC0 Group0 */
				for (unsigned char idx = 0; idx < MAX_ADC_GRP_CHANNEL; idx++)
				{
					/* Results are directly available in resultBuffer associated with the group at initialization
					 * Once the Conversion is completed this buffer will be filled with result by SDK only
					 *  */
					raw_adc_val_arrptr[idx] = adc0_grp0_arr[idx];
					idx++;
				}

				/* Clear the flags */
				adc_hal_adc0_grp0_conv_cplt_flag = 0;

				/* Start */
				adc_hal_start_conversion_it();

			}
			else
			{
				return_error++;

				/*Start the Conversion Again*/
				adc_hal_start_conversion_it;
			}
		}
		break;
		default:
		{
			/*TODO*/
		}
	}

	return return_error;
}


/*********************************************************************
 *  VISHAL
 *  Description : adc conversion complete callback will be called
 *
 ******************************************************************** */
void adc_hal_adc0_clbk_grp0_v(void * parameter)
{

	adc_hal_adc0_grp0_conv_cplt_flag = 1;

}


