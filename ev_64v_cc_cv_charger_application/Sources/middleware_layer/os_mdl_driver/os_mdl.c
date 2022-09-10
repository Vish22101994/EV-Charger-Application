/*
 * os_mdl.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

/*Include*/
#include "os_mdl.h"

/*Application layer header files*/
#include "charger_sm_task.h"
#include "gate_driver_task.h"
#include "measurement_task.h"
#include "protection_task.h"


/*Variable Defination*/

/*Task handler defination*/
TaskHandle_t os_charger_task_handler_ge = NULL;
TaskHandle_t os_measurement_task_handler_ge = NULL;
TaskHandle_t os_protection_task_handler_ge = NULL;
TaskHandle_t os_gate_driver_task_handler_ge = NULL;

/*Queue handler defination*/
QueueHandle_t os_measurement_queue_handler_ge =  NULL;


/*Public Function*/
unsigned char os_system_init(void);
void os_scheduler_start(void);

/*Private Function*/
unsigned char os_create_task(void);
unsigned char os_create_queue(void);


/*********************************************************************
 *  VISHAL
 *  Description : This api will start the scheduler,
 *                * Note :  If no task is created before calling this api, Only IDLE task will run
 *
 ******************************************************************** */
void os_scheduler_start(void)
{
	/*Start the rtos task scheduler*/
	vTaskStartScheduler();

}

/*********************************************************************
 *  VISHAL
 *  Description : os_system_init will create rtos kernel component before starting scheduler
 *
 ******************************************************************** */
unsigned char os_system_init(void)
{

		unsigned char return_error = 0;

		/*Create task*/
		return_error = os_create_task();

		/*Create Queue*/
		return_error = os_create_queue();


		return return_error;
		/*TODO*/
}

/*********************************************************************
 *  VISHAL
 *  Description : by calling this os_create_task api, defined tasks will be created
 *
 ******************************************************************** */
unsigned char os_create_task(void)
{

	unsigned char return_error = 0;

	/* Create the task by calling RTOS API with following inputs :
	 *
	 * */
	/*Task 1 Creation*/
	xTaskCreate(os_measurement_task, 			     	/* function pointer to function that implement the following task */
					"os_measurement_task", 					/* text name for that task :  for debugging purpose */
					OS_MEASUREMENT_TASK_STACK_SIZE, 				/* allocated stack size*/
					NULL, 								/* Passing Parameter to the task*/
					OS_MEASUREMENT_TASK_PRIROTY, 				/*  Pass the defined task priority */
					&os_measurement_task_handler_ge); 			/* task handler for error handling */

	if (os_measurement_task_handler_ge == NULL)
	{
		return_error++;
	}

	/*Task 2 Creation*/
	xTaskCreate(os_charger_sm_task, 			     	/* function pointer to function that implement the following task */
					"os_charger_sm_task", 					/* text name for that task :  for debugging purpose */
					OS_CHARGER_TASK_STACK_SIZE, 				/* allocated stack size*/
					NULL, 								/* Passing Parameter to the task*/
					OS_CHARGER_TASK_PRIROTY, 				/*  Pass the defined task priority */
					&os_charger_task_handler_ge); 			/* task handler for error handling */

	if (os_charger_task_handler_ge == NULL)
	{
		return_error++;
	}

	/*Task 3  Creation*/

	xTaskCreate(os_protection_task, 			     	/* function pointer to function that implement the following task */
					"os_protection_task", 					/* text name for that task :  for debugging purpose */
					OS_PROTECTION_TASK_STACK_SIZE, 				/* allocated stack size*/
					NULL, 								/* Passing Parameter to the task*/
					OS_PROTECTION_TASK_PRIROTY, 				/*  Pass the defined task priority */
					&os_protection_task_handler_ge); 			/* task handler for error handling */

	if (os_protection_task_handler_ge == NULL)
	{
		return_error++;
	}

	/*Task 4 Creation*/

	xTaskCreate(os_gate_driver_task, 			     	/* function pointer to function that implement the following task */
					"os_gate_driver_task", 					/* text name for that task :  for debugging purpose */
					OS_GATE_DRIVER_TASK_STACK_SIZE, 				/* allocated stack size*/
					NULL, 								/* Passing Parameter to the task*/
					OS_GATE_DRIVER_TASK_PRIROTY, 				/*  Pass the defined task priority */
					&os_gate_driver_task_handler_ge); 			/* task handler for error handling */

	if (os_gate_driver_task_handler_ge == NULL)
	{
		return_error++;
	}
	return return_error;
}


/*********************************************************************
 *  VISHAL
 *  Description : os_create_queue will create the queue
 *
 ******************************************************************** */
unsigned char os_create_queue(void)
{

	unsigned char return_error = 0;

	/*queue creation*/
	os_measurement_queue_handler_ge = xQueueCreate(OS_MEASUREMENT_QUEUE_SIZE,     /*Size of queue kernel object*/
									sizeof(os_measurement_queue_payload_typst));  /*size of individual payload*/

	if (os_measurement_queue_handler_ge == NULL)
	{
		return_error++;
	}


	return return_error;
}
