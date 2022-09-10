/*
 * adc_mdl.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_ADC_MDL_H_
#define MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_ADC_MDL_H_


#include "adc_hal.h"
#include "measurement_task.h"

/*MACROS*/
#define ADC_FAULT_VALUE               ((unsigned short)(0xFFFFU))
#define ADC_12BIT_FULL_RESOLUTION     (4096)              /* 12bit ADC = 2^12 = 4096 */
#define ADC_REF_VOLTAGE_CV            (330)              /*ADC conversion is happening with 3.3v reference*/
#define ADC_RAW_ERROR_OFFSET           (50)



/*Current Measurement Related macros*/
#define FEEDBACK_CURRENT_MEASUREMENT_VOLTAGE_GAIN      (40)  /*Constant : 50*/
#define CURRENT_MESAUREMENT_SHUNT_RESISTANCE           (1)   /*Current measuremet shunt resitance : 1mohms*/

/*Voltage Measurement Related macros*/
#define  FEEDBACK_VOLTAGE_MESUREMENT_VOLTAGE_GAIN       (500)  /*Constant : 20*/



/*Public Functions*/
extern unsigned char adc_mdl_init(void);
extern unsigned char adc_mdl_read_feedback_votlage_current(void);
extern unsigned char adc_mdl_start_converstion(void);


#endif /* MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_ADC_MDL_H_ */
