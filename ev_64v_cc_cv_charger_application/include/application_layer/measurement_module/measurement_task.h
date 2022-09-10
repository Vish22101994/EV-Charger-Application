/*
 * measurement_task.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef APPLICATION_LAYER_MEASUREMENT_MODULE_MEASUREMENT_TASK_H_
#define APPLICATION_LAYER_MEASUREMENT_MODULE_MEASUREMENT_TASK_H_

#include "adc_mdl.h"


/*MACROS*/

/*based on the Configuration, When there is no Battery Connected ,
 * Charger will take measurement in low frequency
 *  */
#define MEASUREMENT_LOW_FREQUENCY_EVENT_TIME          (5)  /*50*timer_period : 5*1second  =  5 Sec*/


/*Charger measurement Parameters structure*/
typedef struct measurement_param_tst_tag
{
	unsigned short charger_terminal_voltage_cv;
	unsigned short charger_output_current_ca;

}measurement_param_tst;


/*measurment task state enum list */
typedef enum measurement_task_state_te_tag
{

	MEASUREMENT_TASK_START = 0,
	MEASUREMENT_TASK_MEASURE_INIT,
	MEASUREMENT_TASK_MEASURE_INIT_SUCCESS,
	MEASUREMENT_TASK_MEASUREMENT,
	MEASUREMENT_FAULT_STATE,
	MESAUREMENT_TASK_MEASUREMENT_INIT_FAILED = 0xFF

}measurement_task_state_te_tag;

/*Public Function*/
extern void os_measurement_task(void* pvParameter);

/*Public Variable*/
extern measurement_task_state_te_tag measurement_task_state;
extern measurement_param_tst measurement_param_gst;

#endif /* APPLICATION_LAYER_MEASUREMENT_MODULE_MEASUREMENT_TASK_H_ */
