/*
 * timer_mdl.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_TIMER_MDL_H_
#define MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_TIMER_MDL_H_

#include "timer_hal.h"







extern unsigned char timer_mdl_init(void);
extern void timer_mdl_channel_start_it(void);

#endif /* MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_TIMER_MDL_H_ */
