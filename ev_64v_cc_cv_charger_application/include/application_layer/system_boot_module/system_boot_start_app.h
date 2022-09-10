/*
 * system_boot_start_app.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef APPLICATION_LAYER_SYSTEM_BOOT_MODULE_SYSTEM_BOOT_START_APP_H_
#define APPLICATION_LAYER_SYSTEM_BOOT_MODULE_SYSTEM_BOOT_START_APP_H_

/*Includes*/
#include "Cpu.h"
#include "os_mdl.h"
#include "timer_mdl.h"
#include "adc_mdl.h"


/*Defines*/
#define  CHARGER_BOOT_0_START   system_boot_start


/*INTERRUPTS
 * ----------------------------------------------
 *   PERIPHERAL (IRQ_n)    |         PRIORITY
 * ----------------------------------------------
 *          TIM0           |           4
 * ----------------------------------------------
 *          ADC0 (CH0-CHn) |           5
 * ----------------------------------------------
 * Note*  Numerically low number will refer to the logically high priority interrupt
 * */

/*Boot State of system enums */
typedef enum system_boot_state_list_te_tag
{
	SYSTEM_BOOT_START = 0,
	SYSTEM_BOOT_CORE_INIT,
	SYSTEM_BOOT_POST_OS_INIT,
	SYSTEM_BOOT_OS_RUNNING,
	SYSTEM_BOOT_POST_OS_FAILED,
	SYSTEM_BOOT_CORE_INIT_FAILED,
	SYSTEM_BOOT_OS_SUSPENDED

}system_boot_state_list_te;

/*Function Prototype defination*/
extern unsigned char system_boot_post_os_initialization(void);

/*Public Variable*/
extern system_boot_state_list_te system_boot_state_list_ge;

#endif /* APPLICATION_LAYER_SYSTEM_BOOT_MODULE_SYSTEM_BOOT_START_APP_H_ */
