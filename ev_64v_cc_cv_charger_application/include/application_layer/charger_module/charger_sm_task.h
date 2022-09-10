/*
 * charger_sm_task.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef APPLICATION_LAYER_CHARGER_MODULE_CHARGER_SM_TASK_H_
#define APPLICATION_LAYER_CHARGER_MODULE_CHARGER_SM_TASK_H_

#include "measurement_task.h"
#include "gate_driver_task.h"
#include "system_boot_start_app.h"

/*MACROS*/

#define CHARGER_CC_UPPER_VOLTAGE_THRESHOLD_CV       (7200)    /*As given requirement : 73V peak votlage at FULL Charge State, take 72v for safety side*/
#define CHARGER_CV_LOWER_CURRENT_THRESHOLD_CA       (200)     /*Assumed C/20  :  40 / 20 ->  2Amps {here 1c =  40Amps}*/
#define CHARGER_CC_LOWER_VOLTAGE_THRESHOLD_CV       (4000)    /*Below this charger will not detect the load, will not give the output*/
#define CHARGER_PEAK_UPPER_VOLTAGE_THRESHOLD_CV     (7300)    /*Critical Upper Voltage threshold value*/

#define CHARGER_SM_FAULT_STATE_RECOVERY_THRES        (10)    /*10 seconds: time unit :  1 second*/

typedef enum charger_sm_tout_state_te_tag
{
	CHARGER_TIMEOUT_STATE_SET     = 0,
	CHARGER_TIMEOUT_STATE_CLEAR,

}charger_sm_tout_state_te;


typedef struct charger_sm_timer_state_tst
{

	volatile charger_sm_tout_state_te charger_sm_tout_state;
	unsigned int charger_sm_timer_counter;
	unsigned int charger_sm_timer_set_time;

}charger_sm_timer_state;


typedef enum charger_sm_task_mode_te_tag
{
	CHARGER_SM_TASK_NOT_INIT_MODE,
	CHARGER_SM_TASK_IDLE_MODE,
	CHARGER_SM_TASK_CC_MODE,
	CHARGER_SM_TASK_CV_MODE,
	CHARGER_SM_FAULT_MODE,
	CHARGER_SM_TASK_ERROR_MODE,

}charger_sm_task_mode_te;


typedef enum charger_sm_state_list_te_tag
{
	CHARGER_IDLE_STATE = 0,
	CHARGER_CC_ENABLE_STATE,
	CHARGER_CC_STATE,
	CHARGER_CV_ENABLE_STATE,
	CHARGER_ENABLE_CHECK_STATE,
	CHARGER_CV_STATE,
	CHARGER_OFF_MODE_STATE,
	CHARGER_FAULT_STATE,
	CHARGER_ERROR_STATE,

}charger_sm_state_list_te;


typedef struct charger_sm_state_tst_tag
{
	charger_sm_state_list_te previous_state;
	charger_sm_state_list_te current_state;

}charger_sm_state_tst;



/*Public Functions*/
extern void os_charger_sm_task(void* pvParameter);

/*Public Variable*/
extern charger_sm_state_tst charger_sm_state_gst;
extern charger_sm_task_mode_te charger_sm_task_mode_ge;
extern charger_sm_timer_state charger_sm_timer_state_g;
extern volatile unsigned char charger_sm_state_timer_flag;
extern volatile unsigned char charger_sm_task_flt_recovery_en_flag = 0;
extern volatile unsigned int  charger_sm_task_flt_recovery_counter = 0;
#endif /* APPLICATION_LAYER_CHARGER_MODULE_CHARGER_SM_TASK_H_ */
