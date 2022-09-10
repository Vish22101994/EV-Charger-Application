/*
 * measurement_task.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "measurement_task.h"




/*Public Variable*/

measurement_task_state_te_tag measurement_task_state =  MEASUREMENT_TASK_START;

os_measurement_queue_payload_typst os_measurement_queue_payload_gst;

measurement_param_tst measurement_param_gst;



/*Public Function definition*/
void os_measurement_task(void* pvParameter);


/*********************************************************************
 *  VISHAL
 *  Description : os_measurement_task , handling the scheduled measurements
 *  			  This Time triggered measurement task will execute in two different frequency handled by timer events
 *  			  1. When no battery is connected :  Low Frequency measurement will be scheduled
 *  			  2. When charger is connected and detected : High Frequency measurement will be scheduled
 *
 ******************************************************************** */
void os_measurement_task(void* pvParameter)
{

	unsigned char ret_err = 0;

	/*Block the Further execution of the Measuremnt Task till the proper BOOT sequence is not completed*/
	if(system_boot_state_list_ge != SYSTEM_BOOT_OS_RUNNING)
	{
		vTaskDelay(10);
	}


	/*Initializing the Measurement */
	measurement_task_state =  MEASUREMENT_TASK_MEASURE_INIT;

	/*Measurement Task */
	ret_err =  adc_mdl_read_feedback_votlage_current();

	if(!ret_err)
	{
		/*Measurement Initialization Successfull*/
		measurement_task_state =  MEASUREMENT_TASK_MEASURE_INIT_SUCCESS;
	} else
	{

		/*Measurement Initialization failed*/
		measurement_task_state = MESAUREMENT_TASK_MEASUREMENT_INIT_FAILED;

		/*TODO: Diagnosis Logic can be implemented to do regain the Measurement state
		 *      Suspend the task, for the time being
		 * */
	}


	while(1)
	{

			if(xQueueReceive(os_measurement_queue_handler_ge, &os_measurement_queue_payload_gst, portMAX_DELAY))
			{

				switch(os_measurement_queue_payload_gst.event)
				{

					case OS_LOW_FREQ_MEASURE_EVENT:
					case OS_HIGH_FREQ_MEASURE_EVENT:
					{

							/*Measurement task Measuring*/
							measurement_task_state =  MEASUREMENT_TASK_MEASUREMENT;

							/*Read the Feedback voltage and current & Update the Global structure*/
							adc_mdl_read_feedback_votlage_current();


					}
					break;
					default:
					{
						 /*TODO*/
					}

				}

			}
	}

}

