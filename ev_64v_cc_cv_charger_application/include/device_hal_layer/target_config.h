/*
 * target_config.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef DEVICE_HAL_LAYER_TARGET_CONFIG_H_
#define DEVICE_HAL_LAYER_TARGET_CONFIG_H_



#define MCU_SYS_CLOCK_SOURCE_FREQ      (8000000U)  /*Selected target will be configured for this clock frequency*/

#define TIMER_INPUT_CLOCK_PRESCALER                 (1)
#define TIMER_BASE_COUNTER_CLOCK_PRESCALER          (128)   /*(0:MIN - 255:MAX)*/


#endif /* DEVICE_HAL_LAYER_TARGET_CONFIG_H_ */
