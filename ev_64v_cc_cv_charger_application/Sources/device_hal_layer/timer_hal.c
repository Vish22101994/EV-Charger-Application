/*
 * timer_hal.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#include "timer_hal.h"





/*Public Function declaration*/

unsigned char timer_hal_init(void);

void timer_hal_channel_start_it(void);

void timer_channel_0_callback_event(void);

void timer_channel_1_callback_event(void);





/*********************************************************************
 *  VISHAL
 *  Description : initialize the timer peripheral for getting different temporal events by different timer channel
 *
 ******************************************************************** */
unsigned char timer_hal_init(void)
{
	unsigned char return_error = 0;
	/*Initialize the Timer Base Peripheral, by passing the required structure handle or parameter, depending upon the
	 * SDK or HAL libraries
	 * For this Project : For generating time based event a single timer instance has been used
	 * 					  TIM0: consist of multiple independent timer channel [CH0-CHn] [with 16 counter register]
	 * 					  CH0, CH1 :  for generating 1seconds and 100milisecond time base event [contineous mode]
	 * Following configuration for timer peripheral:
	 * [Input Clock Source 			:  @MCU_SYS_CLOCK_SOURCE_FREQ   {refer : target_config.h}]
	 * Input clock prescaler        :  @TIMER_INPUT_CLOCK_PRESCALER {refer : target_config.h}
	 * Counter clock prescaler      :  @TIMER_BASE_COUNTER_CLOCK_PRESCALER {refer : target_config.h}
	 * callbacks[MAX_CHANNEL]       :  timer_channel_0_callback_event, timer_channel_1_callback_event
	 * 								   these two callbacks will be passed as an argument
	 * */
	/*Remove the  below Commented code , once this application integrated with target based project*/

//	return_error =  sdk_timer_init_api(timer_instance_t instance,&timer_base_config);


	return return_error;



}



/*********************************************************************
 *  VISHAL
 *  Description : start the timer channel (Interrupt flavour API's), by calling timer_channel_start_it
 *
 ******************************************************************** */
void timer_hal_channel_start_it(void)
{


		/* Start Timer Channel 0,
		 * Starting the timer channel (interrupt based) by passsing number of ticks as basic unit
		 * [SDK and HAL libraries are not included in this project]
		 * Ticks    : as  unsigned long value :@TIM0_CH0_1S_TICKS {refer : timer_hal.h}
		 * Instance :  TIM 0
		 * CHANNEL  :  CH0
		 * */

		/*Remove the  below Commented code , once this application integrated with target based project*/

//		sdk_start_timer_channel_it(timer_instance, channel0, ticks);

		/* Start Timer Channel 1,
		 * Starting the timer channel (interrupt based) by passsing number of ticks as basic unit
		 * [SDK and HAL libraries are not included in this project]
		 * Ticks    : as  unsigned long value  : @TIM0_CH1_100MS_TICKS {refer : timer_hal.h}
		 * Instance :  TIM 0
		 * CHANNEL  :  CH1
		 * */

		/*Remove the  below Commented code , once this application integrated with target based project*/

//		sdk_start_timer_channel_it(timer_instance, channel1, ticks);


}

/*********************************************************************
 *  VISHAL
 *  Description : timer channel 0 callback event function will be called, if the configured timeperiod has been elapsed
 *
 ******************************************************************** */
void timer_channel_0_callback_event(void)   /*1 second callback*/
{


	static unsigned char timer_counter_0_v = 0;

	if(system_boot_state_list_ge = SYSTEM_BOOT_OS_RUNNING)
	{

		/*Charger SM task fault state Recovery Counter increment*/
		if(charger_sm_task_flt_recovery_en_flag)
		{
			charger_sm_task_flt_recovery_counter++;
		}


		/*Gate Driver task fault state recovery counter increment*/
		if(gate_driver_task_flt_recovery_en_flag)
		{
			gate_driver_task_flt_recovery_counter++;
		}



		/*Triggering Measurement at Low Frequency*/
		 if(charger_sm_task_mode_ge == CHARGER_SM_TASK_IDLE_MODE)
		 {
			if(timer_counter_0_v == MEASUREMENT_LOW_FREQUENCY_EVENT_TIME)
			{
				/*Reset the Timer Count*/
				timer_counter_0_v = 0;

				os_measurement_queue_payload_typst os_measure_queue_payload_t0;
				os_measure_queue_payload_t.event = OS_LOW_FREQ_MEASURE_EVENT;

				/*Send the Low Frequency Measurement Event*/
				xQueueSendFromISR(os_measurement_queue_handler_ge, &os_measure_queue_payload_t0, NULL);
			} else
			{
				/*Increment the Counter at 1 second Frequency*/
				timer_counter_0_v++;
			}

		 } else
		 {
			 /*Reset the Timer Count*/
			 timer_counter_0_v = 0;
		 }
	}

}



/*********************************************************************
 *  VISHAL
 *  Description : timer channel 1 callback event function will be called, if the configured timeperiod has been elapsed
 *
 *
 ******************************************************************** */
void timer_channel_1_callback_event(void)  /*100ms Callback*/
{


	if(system_boot_state_list_ge = SYSTEM_BOOT_OS_RUNNING)
	{

		 /*Triggering Charger State Timer delay*/
		 if(charger_sm_state_timer_flag)
		 {

			 if(charger_sm_timer_state_g.charger_sm_timer_counter > charger_sm_timer_state_g.charger_sm_timer_set_time)
			 {
				 /*Set, Timeout is elapse*/
				 charger_sm_timer_state_g.charger_sm_tout_state = CHARGER_TIMEOUT_STATE_SET;
			 } else
			 {
				 charger_sm_timer_state_g.charger_sm_timer_counter++;
			 }

		 }


		/*Triggering Measuremnt task at High Frequency*/
		 if(   charger_sm_task_mode_ge == CHARGER_SM_TASK_CC_MODE
			|| charger_sm_task_mode_ge == CHARGER_SM_TASK_CV_MODE
		 )
		 {
			os_measurement_queue_payload_typst os_measure_queue_payload_t1;
			os_measure_queue_payload_t.event = OS_HIGH_FREQ_MEASURE_EVENT;

			/*Send the High Frequency Measurement Event*/
			xQueueSendFromISR(os_measurement_queue_handler_ge, &os_measure_queue_payload_t1, NULL);

		 }
	}


}


