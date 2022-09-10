/*
 * gate_driver_task.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "gate_driver_task.h"


/*Public Variable*/

gate_driver_state_tst gate_driver_state_gst;

gate_driver_mode_te gate_driver_mode_ge = GATE_DRIVER_TASK_NOT_INIT_MODE;

static float err = 0;
static float err_prev = 0;

volatile unsigned char gate_driver_pwm_signal_enabled_flg =  0;

volatile unsigned char gate_driver_task_flt_recovery_en_flag = 0;
volatile unsigned int  gate_driver_task_flt_recovery_counter = 0;

/*Duty Cycle initialized with default value*/
static float dc = GATE_DRIVER_DEFAULT_DUTY_CYCLE;
static float dc_prev = GATE_DRIVER_DEFAULT_DUTY_CYCLE;

/*Private Variable*/

const unsigned short gate_driver_pid_input_reference_arr[PID_CONTROLLER_MAX_NUMBER] = {
		CHARGER_CC_MODE_CURRENT_SETTING_CA,
		CHARGER_CV_MODE_VOLTAGE_SETTING_CV
};

/*Below Initialized value needs to bee tuned,
 * For this project PI controller will be used, such that Kd constant will not be used for error minimization
 * TODO:*/
const gate_driver_pid_param_tst gate_driver_pid_param_gst[PID_CONTROLLER_MAX_NUMBER] = {
		/*PID Controller setting for CC*/
		{
				2.21,   /*Proportional constant*/
				10,     /*Intergral Constant*/
				0   /*Derivative Constant*/
		},
		{
				6.06,  /*Proportional constant*/
				33,    /*Intergral Constant*/
				0  /*Derivative Constant*/
		}
};



/*Public Function definition*/
void os_gate_driver_task(void* pvParameter);

static void pid_control(gate_driver_pid_controller_list_te gate_driver_pid_controller_e, unsigned short actual_value);

/*********************************************************************
 *  VISHAL
 *  Description : os gate driver task controlling the charger votlage and current supply
 *  			  based on which mode the my charger is operating, the gate driver task will maintain the PID controller
 *  			  output to attain the desired setPoint value as configured {STABILIZATION Process}
 *
 ******************************************************************** */
void os_gate_driver_task(void* pvParameter)
{

	/*Block the Further execution of the gate Driver task till the proper BOOT sequence is not completed*/
	if(system_boot_state_list_ge != SYSTEM_BOOT_OS_RUNNING)
	{
		vTaskDelay(10);
	}

	/*Block the Further execution of the gate Driver task till the measurement initialization is not completed*/
	if(measurement_task_state != MEASUREMENT_TASK_MEASURE_INIT_SUCCESS)
	{

		vTaskDelay(10);
	}

	/*Initializing the gate driver state global structure*/
	gate_driver_state_gst.current_state = GATE_DRIVER_TASK_IDLE_STATE;
	gate_driver_state_gst.previous_state = GATE_DRIVER_TASK_IDLE_STATE;
	gate_driver_state_gst.request_mode = GATE_DRIVER_TASK_NOT_INIT_MODE;


	while(1)
	{
		unsigned char state;
		gate_driver_state_list_te nState =  GATE_DRIVER_TASK_ERROR_STATE;
		state = gate_driver_state_gst.current_state;

		/*Update the Gate Driver task mode based on the request*/
		if(gate_driver_mode_ge < GATE_DRIVER_FAULT_MODE)
		{
			gate_driver_mode_ge = gate_driver_state_gst.request_mode;

		}else if(gate_driver_mode_ge == GATE_DRIVER_FAULT_MODE)
		{
			gate_driver_task_flt_recovery_en_flag = 1;
			if(gate_driver_task_flt_recovery_counter > GATE_RECOVERY_FAULT_STATE_RECOVERY_THRES)
			{
				/*Clear the flt recovery flag*/
				gate_driver_task_flt_recovery_en_flag = 0;

				/*Clear the Counter value */
				gate_driver_task_flt_recovery_counter = 0;

				/*Automatic recovery from the fault*/
				gate_driver_mode_ge = GATE_DRIVER_TASK_IDLE_MODE;
			}
		}

		switch(state)
		{
			case GATE_DRIVER_TASK_IDLE_STATE:
			{
				if(   gate_driver_mode_ge == GATE_DRIVER_TASK_VOLTAGE_ENABLE_MODE
				   || gate_driver_mode_ge == GATE_DRIVER_TASK_CURRENT_ENABLE_MODE)
				{
					nState =  GATE_DRIVER_TASK_ENABLE_STATE;
				} else
				{
					nState = GATE_DRIVER_TASK_IDLE_MODE;
				}
			}
			break;
			case GATE_DRIVER_TASK_ENABLE_STATE:
			{
				/*Initiate the PWM with channel with deafult duty cycle defined by configuration in low level driver*/
				if(pwm_mdl_init(PWM_OP_GATE_DRIVER) == 0)
				{
					nState =  GATE_DRIVER_TASK_STABILIZING_STATE;
				}
				else
				{
					nState = GATE_DRIVER_TASK_FAULT_STATE;
				}
			}
			break;
			case GATE_DRIVER_TASK_STABILIZING_STATE:
			{
				if(    gate_driver_mode_ge == GATE_DRIVER_TASK_VOLTAGE_ENABLE_MODE
				    || gate_driver_mode_ge == GATE_DRIVER_TASK_CURRENT_ENABLE_MODE)
				{
					if(gate_driver_mode_ge == GATE_DRIVER_TASK_CURRENT_ENABLE_MODE)
					{
						/*PID controller output update*/
						pid_control(GATE_DRIVER_CC_PID_CONTROLLER,measurement_param_gst.charger_output_current_ca);

					} else if(gate_driver_mode_ge == GATE_DRIVER_TASK_VOLTAGE_ENABLE_MODE)
					{
						/*PID Controller output update*/
						pid_control(GATE_DRIVER_CV_PID_CONTROLLER, measurement_param_gst.charger_terminal_voltage_cv);
					}

					/*Modify the duty cycle if the absolute error is not under the tolerance value*/
					if(abs(err) < GATE_DRIVER_ERROR_TOLERANCE)
					{
						pwm_mdl_change_duty_cycle(PWM_OP_GATE_DRIVER, dc*100);
					}

					/*Change the next State*/
					nState = GATE_DRIVER_TASK_STABILIZING_STATE;

				} else
				{
					nState = GATE_DRIVER_TASK_DISABLE_STATE;
				}
			}
			break;
			case GATE_DRIVER_TASK_DISABLE_STATE:
			{
				/*Initiate the PWM with channel with deafult duty cycle defined by configuration in low level driver*/
				if(pwm_mdl_deinit(PWM_OP_GATE_DRIVER) == 0)
				{
					nState =  GATE_DRIVER_TASK_IDLE_STATE;
				}
				else
				{
					nState = GATE_DRIVER_TASK_FAULT_STATE;
				}
			}
			break;
			case GATE_DRIVER_TASK_FAULT_STATE:
			{
				/*Change the mode to fault mode*/
				gate_driver_mode_ge = GATE_DRIVER_FAULT_MODE

				/*Change the State*/
				nState = GATE_DRIVER_TASK_IDLE_STATE;
			}
			break;
			case GATE_DRIVER_TASK_ERROR_STATE:
			{
				/*TODO*/
			}
			break;
			default:
			{
				/*TODO*/
			}
		}


		gate_driver_state_gst.current_state =  nState;

		vTaskDelay(10);
	}

}







/*********************************************************************
 *  VISHAL
 *  Description : PID Controller , based on kp , ki and kd tuned for the corresponding PID Controller will try to minimize the error
 *
 ******************************************************************** */
static void pid_control(gate_driver_pid_controller_list_te gate_driver_pid_controller_e , unsigned short actual_value)
{


	err = ((float)gate_driver_pid_input_reference_arr[gate_driver_pid_controller_e]/100.0) - ((float)actual_value/100.0);  /*unisgned short (Centi-Volts) -> float(volts)*/

	dc = dc_prev
	     + gate_driver_pid_param_gst[gate_driver_pid_controller_e].pid_param_kp*(err - err_prev)
		 + gate_driver_pid_param_gst[gate_driver_pid_controller_e].pid_param_ki*(err);

	if(dc > GATE_DRIVER_UPPER_THRES_DUTY_CYCLE)
	{
		dc = GATE_DRIVER_UPPER_THRES_DUTY_CYCLE;

	} else if(dc < GATE_DRIVER_LOWER_THRES_DUTY_CYCLE)
	{
		dc = GATE_DRIVER_LOWER_THRES_DUTY_CYCLE;
	}

	err_prev = err;

	dc_prev =  dc;


}










