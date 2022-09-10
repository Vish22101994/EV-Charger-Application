/*
 * protection_task.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "protection_task.h"











/*Public Function definition*/
void os_protection_task(void* pvParameter);


/*********************************************************************
 *  VISHAL
 *  Description : Handles the critical condition of the charger and initiate the appropriate
 *
 ******************************************************************** */
void os_protection_task(void* pvParameter)
{


	/*Block the Further execution of the gate Driver task till the proper BOOT sequence is not completed*/
	if(system_boot_state_list_ge != SYSTEM_BOOT_OS_RUNNING)
	{
		vTaskDelay(10);
	}


	/*Block the Further execution of the Protection task till the measurement initialization is not completed*/
	if(measurement_task_state != MEASUREMENT_TASK_MEASURE_INIT_SUCCESS)
	{

		vTaskDelay(10);
	}


	while(1)
	{


		/*TODO :
		 * Please nOte protection feature can be handled in this task for future purpose
		 * */


		vTaskDelay(100);
	}

}


