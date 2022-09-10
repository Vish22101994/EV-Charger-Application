/*
 * pwm_mdl.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "pwm_mdl.h"





/*Public Function Definition*/

unsigned char pwm_mdl_init(pwm_mdl_target_e pwm_mdl_target);
unsigned char pwm_mdl_deinit(pwm_mdl_target_e pwm_mdl_target);
unsigned char pwm_mdl_change_duty_cycle(pwm_mdl_target_e pwm_mdl_target, unsigned short pwm_duty_cycle);


/*********************************************************************
 *  VISHAL
 *  Description : pwm_mdl_init will configure and starts the PWM Signal on the configured channel
 *
 ******************************************************************** */

unsigned char pwm_mdl_init(pwm_mdl_target_e pwm_mdl_target)
{

	unsigned char return_error = 0;

	switch((pwm_mdl_tim_pwm_instance)pwm_mdl_target)
	{
	case TIM_PWM_INST1:
	{

		/*Initialize the TIM1 base instance to PWM  and start PWM signal on configured channel with given below configuration*/
		/* pass the following configuration {taking randomly , will be decided as per the schematics}
		 * Select the TIM1 channel for generating PWM Signal {configure as ouput compare mode}
		 * Channel Type : Edge Aligned
		 * period : 5000 {Period of output signal}
		 * duty   : 2500 {PWM signal initially generate 0.5  duty cycle pwm signal}
		 * deadtime : not needed
		 * timebase : timer TIM1 base handle {SDK will do configuration setting for timer peripheral using this timer handle}
		 * Complementary channel :  not needed
		 * */

		return_error += sdk_pwm_init(&tim_pwm_instance, &tim_pwm_config);    /*Dummy Function call to the SDK Layer {UNDEFINED}*/

	}
	break;
	default:
	{
		/*TODO*/
	}


	}

}



/*********************************************************************
 *  VISHAL
 *  Description : Stop ALL THE CHannelS from producing the PWM Signal
 *
 ******************************************************************** */

unsigned char pwm_mdl_deinit(pwm_mdl_target_e pwm_mdl_target)
{

	unsigned char return_error = 0;

	switch((pwm_mdl_tim_pwm_instance)pwm_mdl_target)
	{
	case TIM_PWM_INST1:
	{
		/*STOP the PWM signal from the output channel of TIM 1*/

		return_error += sdk_pwm_deinit(&tim_pwm_instance);    /*Dummy Function call to the SDK Layer {UNDEFINED}*/

	}
	break;
	default:
	{
		/*TODO*/
	}


	}

}


/*********************************************************************
 *  VISHAL
 *  Description : To update the duty cycle of PWM signal {Input duty cycle will be in percentage 0%-100% will be changed to TICKS internal by this api}
 *
 ******************************************************************** */

unsigned char pwm_mdl_change_duty_cycle(pwm_mdl_target_e pwm_mdl_target, unsigned short pwm_duty_cycle)
{

	unsigned char return_error = 0;

	unsigned int tim_base_period = get_timer_base_period(); /*Dummy Function call or peripheral register read for extracting the PERIOD value set during PWM init*/

	unsigned char tim_pwm_instance = (pwm_mdl_tim_pwm_instance)pwm_mdl_target;

	/* Convert to Ticks*/
	pwm_duty_cycle = (tim_base_period * pwm_duty_cycle) / 100;


	return_error = sdk_pwm_update_duty_cycle(tim_pwm_instance,pwm_duty_cycle);  /*Dummy Functional Call to change the duty cycle on fly*/


	return return_error;
}


