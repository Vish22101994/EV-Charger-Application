/*
 * timer_hal.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef DEVICE_HAL_LAYER_TIMER_HAL_H_
#define DEVICE_HAL_LAYER_TIMER_HAL_H_

#include "target_config.h"
#include "os_mdl.h"
#include "system_boot_start_app.h"
#include "charger_sm_task.h"
#include "gate_driver_task.h"



/*MACRO Definition*/

#define TIMER_INPUT_CLOCK_FREQ              (MCU_SYS_CLOCK_SOURCE_FREQ/TIMER_INPUT_CLOCK_PRESCALER)
#define TIMER_BASE_COUNTER_FREQ             (TIMER_INPUT_CLOCK_FREQ/TIMER_BASE_COUNTER_CLOCK_PRESCALER)



#define TIM0_CH0_1S_TICKS                   (1000*TIMER_BASE_COUNTER_FREQ)  /*Number of ticks to generate 1s timer event*/
#define TIME_CH1_100MS_TICKS                (100*TIMER_BASE_COUNTER_FREQ)   /*Number of ticks to generate 100ms timer event*/





/*Public Functions*/
extern unsigned char timer_hal_init(void);
extern void timer_hal_channel_start_it(void);

#endif /* DEVICE_HAL_LAYER_TIMER_HAL_H_ */
