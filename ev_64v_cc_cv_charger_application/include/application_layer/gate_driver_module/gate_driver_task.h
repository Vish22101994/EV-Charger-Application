/*
 * gate_driver_task.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef APPLICATION_LAYER_GATE_DRIVER_MODULE_GATE_DRIVER_TASK_H_
#define APPLICATION_LAYER_GATE_DRIVER_MODULE_GATE_DRIVER_TASK_H_

/*iNCLUDES*/
#include "pwm_mdl.h"
#include "measurement_task.h"
#include "system_boot_start_app.h"

/*MACRO Definition*/
/*Constant Current Setting for the PID Controller
 * Can Be changed, as per requirement
 * */
#define CHARGER_CC_MODE_CURRENT_SETTING_CA           (2000)   /*CC mode current setting in centi Amps*/

/*Constant Voltage Setting for the PID Controller
 * Can be changed, as per requirement
 * */
#define CHARGER_CV_MODE_VOLTAGE_SETTING_CV           (7200)  /*CV mode current setting in centi Volts*/

/*
 * Following Gate Driver pwm signal Duty cycle can be changed as per the hardware design and tuning parameter
 * */
#define GATE_DRIVER_DEFAULT_DUTY_CYCLE                ((float)(0.5))
#define GATE_DRIVER_UPPER_THRES_DUTY_CYCLE            ((float)(0.9))
#define GATE_DRIVER_LOWER_THRES_DUTY_CYCLE            ((float)(0.2))


/**/
#define GATE_RECOVERY_FAULT_STATE_RECOVERY_THRES           (5)    /*5seconds : time unit :  1 second*/

/*PID Controller tolerance
 * Can be changed, as per requirement
 * */
#define GATE_DRIVER_ERROR_TOLERANCE       ((float)(0.3))

typedef enum gate_driver_mode_te_tag
{
	GATE_DRIVER_TASK_NOT_INIT_MODE,
	GATE_DRIVER_TASK_IDLE_MODE,
	GATE_DRIVER_TASK_VOLTAGE_ENABLE_MODE,
	GATE_DRIVER_TASK_CURRENT_ENABLE_MODE,
	GATE_DRIVER_FAULT_MODE,
	GATE_DRIVER_TASK_ERROR_MODE,

}gate_driver_mode_te;


typedef enum gate_driver_state_list_te_tag
{
	GATE_DRIVER_TASK_IDLE_STATE,
	GATE_DRIVER_TASK_ENABLE_STATE,
	GATE_DRIVER_TASK_STABILIZING_STATE,
	GATE_DRIVER_TASK_DISABLE_STATE,
	GATE_DRIVER_TASK_FAULT_STATE,
	GATE_DRIVER_TASK_ERROR_STATE

}gate_driver_state_list_te;


typedef struct gate_driver_state_tst_tag
{
	gate_driver_state_list_te previous_state;
	gate_driver_state_list_te current_state;
	gate_driver_mode_te request_mode;

}gate_driver_state_tst;


typedef enum gate_driver_pid_controller_list_te_tag
{
	GATE_DRIVER_CC_PID_CONTROLLER,
	GATE_DRIVER_CV_PID_CONTROLLER,

	PID_CONTROLLER_MAX_NUMBER,
}gate_driver_pid_controller_list_te;

typedef struct gate_driver_pid_param_tst_tag
{
	float pid_param_kp;
	float pid_param_ki;
	float pid_param_kd;

}gate_driver_pid_param_tst;






/*Public Function*/
extern void os_gate_driver_task(void* pvParameter);

/*Public Variable*/
extern gate_driver_state_tst gate_driver_state_gst;
extern volatile unsigned char gate_driver_pwm_signal_enabled_flg;
extern volatile unsigned char gate_driver_task_flt_recovery_en_flag;
extern volatile unsigned int  gate_driver_task_flt_recovery_counter;

#endif /* APPLICATION_LAYER_GATE_DRIVER_MODULE_GATE_DRIVER_TASK_H_ */
