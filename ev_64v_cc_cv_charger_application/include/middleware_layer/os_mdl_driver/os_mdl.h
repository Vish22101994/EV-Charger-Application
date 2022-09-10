/*
 * os_mdl.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef MIDDLEWARE_LAYER_OS_MDL_DRIVER_OS_MDL_H_
#define MIDDLEWARE_LAYER_OS_MDL_DRIVER_OS_MDL_H_

#include "task.h"
#include "queue.h"



/*Defining MACROS*/

/*TASK STACK SIZE MACROS
 *
 *Size :  (in words)
 *Note :  Once the application is completely finished, do the stack analysis and
 *		  Set the Appropriate stack size to optimize the RAM usage
 **/
#define OS_MEASUREMENT_TASK_STACK_SIZE       (500)
#define OS_PROTECTION_TASK_STACK_SIZE        (500)
#define OS_CHARGER_TASK_STACK_SIZE           (500)
#define OS_GATE_DRIVER_TASK_STACK_SIZE       (500)


/*QUEUE size*/
#define OS_MEASUREMENT_QUEUE_SIZE             (10)


/*Task priority MACROS Definition*/
#define OS_MEASUREMENT_TASK_PRIROTY       (osPriorityNormal)
#define OS_PROTECTION_TASK_PRIROTY        (osPriorityNormal)
#define OS_CHARGER_TASK_PRIROTY           (osPriorityHigh)
#define OS_GATE_DRIVER_TASK_PRIROTY       (osPriorityHigh)

typedef enum os_measruement_queue_event_list_typd_tag
{
	OS_LOW_FREQ_MEASURE_EVENT  = 1,
	OS_HIGH_FREQ_MEASURE_EVENT = 2,

}os_measruement_queue_event_list_typd;


/*RTOS queue payload typedef structure*/
typedef struct os_measurement_queue_payload_typst_tag
{
	os_measruement_queue_event_list_typd event;

}os_measurement_queue_payload_typst;


/*RTOS task priority typedef enum
 *Note : Numerically higher priority will have logically higher priority in this RTOS
 * */
typedef enum os_priority_typd_tag
{
  osPriorityIdle       = 0,          // idle (lowest)
  osPriorityLow           ,          // low
  osPriorityBelowNormal   ,          // below normal
  osPriorityNormal        ,          // (default)
  osPriorityAboveNormal   ,          // above normal
  osPriorityHigh          ,          // high
  osPriorityAboveHigh      ,          //above high (highest)
} os_priority_typd;



extern unsigned char os_system_init(void);
extern void os_scheduler_start(void);

#endif /* MIDDLEWARE_LAYER_OS_MDL_DRIVER_OS_MDL_H_ */
