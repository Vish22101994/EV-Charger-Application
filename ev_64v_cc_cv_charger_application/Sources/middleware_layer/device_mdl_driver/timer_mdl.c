/*
 * timer_mdl.c
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */


#include "timer_mdl.h"




/*Public Function  definition*/
unsigned char timer_mdl_init(void);
void timer_mdl_channel_start_it(void);


/*********************************************************************
 *  VISHAL
 *  Description : Initialize the timer peripheral of target device (Wrapper function)
 *
 ******************************************************************** */

unsigned char timer_mdl_init(void)
{

	unsigned char return_error = 0;

	return_error =  timer_hal_init();

	return return_error;

}


/*********************************************************************
 *  VISHAL
 *  Description : Starting timer channel to generate time based interrupt events by MCU timer peripheral
 *
 ******************************************************************** */

void timer_mdl_channel_start_it(void)
{

	unsigned char return_error = 0;

	return_error =  timer_hal_channel_start_it();

	return return_error;

}


