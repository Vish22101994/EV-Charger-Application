/*
 * charger_sm_task.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "charger_sm_task.h"


/*Public Variable*/
charger_sm_state_tst charger_sm_state_gst;


charger_sm_task_mode_te charger_sm_task_mode_ge = CHARGER_SM_TASK_NOT_INIT_MODE;

charger_sm_task_mode_te charger_sm_task_temp_mode_ge =  CHARGER_SM_TASK_NOT_INIT_MODE;

volatile unsigned char charger_sm_state_timer_flag = 0;

volatile unsigned char charger_sm_task_flt_recovery_en_flag = 0;
volatile unsigned int  charger_sm_task_flt_recovery_counter = 0;

/*Initialize the Timer timeout state */
charger_sm_timer_state charger_sm_timer_state_g = {
		{
			CHARGER_TIMEOUT_STATE_CLEAR,
			0,
			0,

		}
};

/*Public Function definition*/
void os_charger_sm_task(void* pvParameter);

void os_charger_sm_update_mode(void);

void os_charger_sm_reset_timer(void);

void os_charger_sm_set_delay(unsigned int set_time)

/*********************************************************************
 *  VISHAL
 *  Description : Charger output state task function, Maitains the state of the charger output state
 *  			  Based on votlage and current thresholds defined to acheive CC_CV charging
 *  			  following below charger state machine maintains the constant current and constant voltage mode of the battery charging stage
 *
 ******************************************************************** */
void os_charger_sm_task(void* pvParameter)
{

	unsigned char error_code = 0;


	system_boot_state_list_ge =  SYSTEM_BOOT_POST_OS_INIT;

	/*POST OS Initialization*/
	error_code = system_boot_post_os_initialization();


	if(!error_code)
	{
		/*Post Initialization Successfull*/
		system_boot_state_list_ge = SYSTEM_BOOT_OS_RUNNING;
	} else
	{
		/*BOOT System POST Initialization Failed*/
		system_boot_state_list_ge = SYSTEM_BOOT_POST_OS_FAILED;

		/*suspend the tasks or scheduler or put into diagnostic mode
		 * TODO:
		 * */
	}

	/*Block the Further execution of the Charger State Task till the measurement initialization is not completed*/
	if(measurement_task_state != MEASUREMENT_TASK_MEASURE_INIT_SUCCESS)
	{

		vTaskDelay(10);
	}

	/*Initialize the charger state variable*/
	charger_sm_state_gst.current_state  = CHARGER_IDLE_STATE;
	charger_sm_state_gst.previous_state = CHARGER_IDLE_STATE;

	while(1)
	{
		unsigned char state;
		charger_sm_state_list_te nState = CHARGER_ERROR_STATE
		state = charger_sm_state_gst.current_state;

		/*Update the charger sm task mode based on voltage and current*/
		os_charger_sm_update_mode();

		if(charger_sm_task_mode_ge < CHARGER_SM_FAULT_MODE)
		{
			if(charger_sm_task_temp_mode_ge ==  CHARGER_SM_TASK_CC_MODE)
			{

				charger_sm_task_mode_ge = CHARGER_SM_TASK_CC_MODE;

			}
			else if(charger_sm_task_temp_mode_ge == CHARGER_SM_TASK_CV_MODE)
			{

				charger_sm_task_mode_ge = CHARGER_SM_TASK_CV_MODE;

			} else
			{
				charger_sm_task_mode_ge = CHARGER_SM_TASK_IDLE_MODE;
			}

		} else if(charger_sm_task_mode_ge == CHARGER_SM_FAULT_MODE)
		{
			charger_sm_task_flt_recovery_en_flag = 1;
			if(charger_sm_task_flt_recovery_counter > CHARGER_SM_FAULT_STATE_RECOVERY_THRES)
			{
				/*Clear the flt recovery flag*/
				charger_sm_task_flt_recovery_en_flag = 0;

				/*Clear the Counter value */
				charger_sm_task_flt_recovery_counter = 0;

				/*Automatic recovery from the fault*/
				charger_sm_task_mode_ge = CHARGER_SM_TASK_IDLE_MODE;
			}
		}

		switch(state)
		{
			case CHARGER_IDLE_STATE:
			{
				if(charger_sm_task_mode_ge == CHARGER_SM_TASK_CC_MODE)
				{
					nState =  CHARGER_CC_ENABLE_STATE;

				} else if(charger_sm_task_mode_ge ==  CHARGER_SM_TASK_CV_MODE)
				{
					nState = CHARGER_CV_ENABLE_STATE;

				} else
				{
					nState = CHARGER_IDLE_STATE;
				}
			}
			break;
			case CHARGER_CC_ENABLE_STATE:
			{
				/*Trigger the event, to turn on gate driver signal */
				gate_driver_state_gst.request_mode = GATE_DRIVER_TASK_CURRENT_ENABLE_MODE;

				/*Set timer delay*/
				os_charger_sm_set_delay(4);  /*400ms :  can be changed as per the requirement */

				nState =  CHARGER_ENABLE_CHECK_STATE;
			}
			break;
			case CHARGER_CV_ENABLE_STATE:
			{
				/*Trigger the event, to turn on gate driver signal */
				gate_driver_state_gst.request_mode = GATE_DRIVER_TASK_VOLTAGE_ENABLE_MODE;

				/*Set timer delay*/
				os_charger_sm_set_delay(4);  /*400ms :  can be changed as per the requirement */

				nState =  CHARGER_ENABLE_CHECK_STATE;
			}
			break;
			case CHARGER_ENABLE_CHECK_STATE:
			{
				if(gate_driver_state_gst.current_state == GATE_DRIVER_TASK_STABILIZING_STATE)
				{
					/*Clear the Timer state*/
					os_charger_sm_reset_timer();

					if(charger_sm_task_mode_ge == CHARGER_SM_TASK_CC_MODE)
					{
						nState =  CHARGER_CC_STATE;

					} else if(charger_sm_task_mode_ge ==  CHARGER_SM_TASK_CV_MODE)
					{
						nState = CHARGER_CV_STATE;

					} else
					{
						nState = CHARGER_OFF_MODE_STATE;
					}
				}
				else if(charger_sm_timer_state_g.charger_sm_tout_state == CHARGER_TIMEOUT_STATE_SET)
				{
					/*Clear the timer State*/
					os_charger_sm_reset_timer();

					nState = CHARGER_FAULT_STATE;

				} else
				{
					nState = CHARGER_ENABLE_CHECK_STATE;
				}
			}
			break;
			case CHARGER_CC_STATE:
			{
				if(gate_driver_state_gst.current_state == GATE_DRIVER_TASK_STABILIZING_STATE)
				{
					if(charger_sm_task_mode_ge == CHARGER_SM_TASK_CC_MODE)
					{
						nState =  CHARGER_CC_STATE;
					} else if(charger_sm_task_mode_ge == CHARGER_SM_TASK_CV_MODE)
					{
						/*Trigger the event, to turn on gate driver signal */
						gate_driver_state_gst.request_mode = GATE_DRIVER_TASK_VOLTAGE_ENABLE_MODE;

						/*Once the CV mode starts and previously the CC mode was running , direct jump to the CV mode state*/
						nState = CHARGER_CV_STATE
					} else
					{
						nState = CHARGER_OFF_MODE_STATE;
					}
				} else
				{
					nState =  CHARGER_FAULT_STATE;
				}

			}
			break;
			case CHARGER_CV_STATE:
			{
				if(gate_driver_state_gst.current_state == GATE_DRIVER_TASK_STABILIZING_STATE)
				{
					if(charger_sm_task_mode_ge == CHARGER_SM_TASK_CV_MODE)
					{
						nState =  CHARGER_CV_STATE;
					} else if(charger_sm_task_mode_ge == CHARGER_SM_TASK_CC_MODE)
					{
						/*Trigger the event, to turn on gate driver signal */
						gate_driver_state_gst.request_mode = GATE_DRIVER_TASK_CURRENT_ENABLE_MODE;

						/*Needs a clarity, but anyways if such case happens , CV mode to CC mode jump will happen*/
						nState = CHARGER_CV_STATE
					} else
					{
						nState = CHARGER_OFF_MODE_STATE;
					}
				} else
				{
					nState =  CHARGER_FAULT_STATE;
				}

			}
			break;
			case CHARGER_OFF_MODE_STATE:
			{
				/*TODO :  OFF mode activity we can perform
				 * */
				/*Trigger the event, to turn off gate driver signal , Since the charger has detected no Battery is conected*/
				gate_driver_state_gst.request_mode = GATE_DRIVER_TASK_IDLE_MODE;

				nState  = CHARGER_IDLE_STATE;
			}
			break;
			case CHARGER_FAULT_STATE:
			{

				/*As for now this fault will  be non recoverable*/
				charger_sm_task_mode_ge =  CHARGER_FAULT_STATE;

				/*Trigger the event, to turn off gate driver signal , Since the charger has detected no Battery is conected*/
				gate_driver_state_gst.request_mode = GATE_DRIVER_TASK_IDLE_MODE;

				nState  = CHARGER_IDLE_STATE;
			}
			break;
			case CHARGER_ERROR_STATE:
			{
				/*TODO*/
			}
			break;
			default:
			{
				/*TODO*/
			}



		}


		/*Critical Condition Handling
		 * TODO: needs to be done {Important}
		 * */



		charger_sm_state_gst.current_state = nState;

		vTaskDelay(100);
	}

}



/*********************************************************************
 *  VISHAL
 *  Description : based on the voltage and current , charger state machine task mode will be updated here
 *
 ******************************************************************** */

void os_charger_sm_update_mode(void)
{
		static unsigned char charge_complete_flag =  0;

		/*CC Mode Detection */
		if(measurement_param_gst.charger_terminal_voltage_cv < CHARGER_CC_UPPER_VOLTAGE_THRESHOLD_CV
		   && measurement_param_gst.charger_terminal_voltage_cv >= CHARGER_CC_LOWER_VOLTAGE_THRESHOLD_CV)
		{
				/*Constant Current mode is detected*/

				/*Trigger Mode Change to CC Mode*/
				charger_sm_task_temp_mode_ge = CHARGER_SM_TASK_CC_MODE;

		}
		/*CV Mode Detection*/
		else if(measurement_param_gst.charger_terminal_voltage_cv >= CHARGER_CC_UPPER_VOLTAGE_THRESHOLD_CV
				&& measurement_param_gst.charger_terminal_voltage_cv < CHARGER_PEAK_UPPER_VOLTAGE_THRESHOLD_CV)
		{
				/*Constant Voltage Mode is detected */

			if(measurement_param_gst.charger_output_current_ca < CHARGER_CV_LOWER_CURRENT_THRESHOLD_CA)
			{
				/*Charging Completed once the current reaches to lower threshold value in CV Mode*/
				charge_complete_flag = 1;
			}
			else
			{
				charge_complete_flag = 0;

			}


			if(!charge_complete_flag)
			{
				/*Trigger Mode change to CV Mode*/
				charger_sm_task_temp_mode_ge =  CHARGER_SM_TASK_CV_MODE;
			}
			else
			{
				/*Trigger Mode change to Idle mode, since the CHarging is completed*/
				charger_sm_task_temp_mode_ge = CHARGER_SM_TASK_IDLE_MODE;
			}


		}
		/*FAULT or IDLE mode*/
		else
		{
				/*Clear the Charge complete flag*/
				charge_complete_flag = 0;
				/*Trigger mode change to IDLE */
	     		charger_sm_task_temp_mode_ge =  CCHARGER_SM_TASK_IDLE_MODE;

		}


}



/*********************************************************************
 *  VISHAL
 *  Description : Reset the charger State timer state, to work us properly
 *
 ******************************************************************** */

void os_charger_sm_reset_timer(void)
{
	/*Reset the charger sm state timer flag*/
	charger_sm_state_timer_flag= 0;

	charger_sm_timer_state_g.charger_sm_timer_counter = 0;
	charger_sm_timer_state_g.charger_sm_tout_state = CHARGER_TIMEOUT_STATE_CLEAR;
	charger_sm_timer_state_g.charger_sm_timer_set_time = 0;

}



/*********************************************************************
 *  VISHAL
 *  Description : Set the Timer Set point for timer Delay, set time in 100ms unit
 *
 ******************************************************************** */
void os_charger_sm_set_delay(unsigned int set_time)
{
	/*Set the timer flag , for delay*/
	charger_sm_state_timer_flag = 1;

	charger_sm_timer_state_g.charger_sm_timer_set_time = set_time;
	charger_sm_timer_state_g.charger_sm_timer_counter = 0;
	charger_sm_timer_state_g.charger_sm_tout_state = CHARGER_TIMEOUT_STATE_CLEAR;

}
