/*
 * system_boot_start_app.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


/*Include module header file*/
#include "system_boot_start_app.h"


/*Public variable*/
system_boot_state_list_te system_boot_state_list_ge;


void system_boot_start(void);
unsigned char system_boot_post_os_initialization(void);

/*********************************************************************
 *  VISHAL
 *  Description : system boot sequence initiating after the Power On Reset (POR)
 *
 ******************************************************************** */
void system_boot_start(void)
{


	unsigned short return_error_code = 0;

	system_boot_state_list_ge =  SYSTEM_BOOT_START;

	/* 1. Extract the Reason of Power On Reset*/	/*
	 * Create an API for diagnosis of power On reset by checking the
	 * MCU registers @ refer the respective MCU Datasheet
	 *
	 * */
	/*TODO*/


	system_boot_state_list_ge =  SYSTEM_BOOT_CORE_INIT;

	/*2. Interrupt Disabling [For Interrupt Refer @system_boot_start_app.h]
	 *   by passing respective IRQ_n and its priority into the below sdk function
	 *   Before doing any core clock configuration and peripheral initialization, disable interrupts
	 * */
	 sdk_disable_interrupts(param);  /*Dummy Call to SDK or HAL to disable the interrupts*/

	/*3. Configure and initiate the system clock as per the requirement*/
	/* MCU Clock TREE will configured here, by passing the clock source , prescalers to SYSTEM , BUS , CORE clocks
	 * MCU's peripheral clock configuration will also be passed as argument to [ SDK or HAL APIs]
	 * For this Project , below peripheral clock frequency is required by passing appropriate parameters to configure the CLOCK tree of MCU
	 * ADC_PERIPHERAL_CLOCK         :  8 MHZ
	 * TIM0_PERIPHERAL_INPUT_CLOCK  :  8 MHZ
	 * TIM1_PERIPHERAL_INPUT_CLOCK  :  64 MHZ  {This timer peripheral will be used to generate PWM PULSE}
	 * 								   {generation of PWM Frequency will be depend upon the hardware requirement}
	 * */
	return_error_code +=  SDK_clock_config(&clock_config_struct);  /*Dummy function call to the SDK(not defined)*/


	/* 4. MCU Pin Intialization will be done here :
	 * [two adc pin needs to be configured for the Anlaog Input Sensing to the MCU]
	 * [One pin as an Alternate Functionality for PWM signal to drive the gate driver circuit]
	 * Pass the arguments to SDK PIN configuration API {varries SDK to SDK }
	 * AS per the Schematics [or  as per the hardware requirement] set the pin as an analog input pin and PWM pin
	 *
	 * */
	return_error_code += SDK_pin_mux_init(&pin_mux_config);  /*Dummy function call to the SDK(not defined)*/


	/*5. Initialize the MCU device peripheral as per the requirement*/

	/*Timer initialization*/
	return_error_code  += timer_mdl_init();

	/*ADC Intitialization*/
	return_error_code  += adc_mdl_init();


	/*6.  Initialize the Real Time Operating System*/
	return_error_code += os_system_init();



	/*If the core initialization has been completed successfully, then only start the
	 * rtos scheduler */
	if(return_error_code)
	{
		/*Start the RTOS scheduler*/
		os_scheduler_start();

	}
	else
	{
	/*NOTE :  Program will enter into this if boot sequence is failed
	 *        (Program will be stuck in infinte for loop )
	 *
	 *        TODO: diagnosis or alert logic can be implemented
	 *
	 * */

		system_boot_state_list_ge = SYSTEM_BOOT_CORE_INIT_FAILED;
	}

	for(;;)
	{
		/*TODO*/
	}

	return;
}


/*********************************************************************
 *  VISHAL
 *  Description : Doing the Post os Peripheral initialization, after the scheduler starts
 *
 ******************************************************************** */
unsigned char system_boot_post_os_initialization(void)
{


		unsigned char return_error = 0;

		/*Enable all the Interrupt : refer : {@system_boot_start_app.h}
		 * By calling the sdk enable interrupt API (setting the MCU low level register)
		 * */
		sdk_enable_interrupts(param);   /*Dummy Call to the sdk , to enable the interrupts */

		/*Start TIM0 Channels*/
		timer_mdl_channel_start_it();

		/*Start ADC Conversion*/
		return_error += adc_mdl_start_converstion();


		return return_error;
}
