/*
 * pwm_mdl.h
 *
 *  Created on: 08-Sep-2022
 *      Author: Vishal
 */

#ifndef MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_PWM_MDL_H_
#define MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_PWM_MDL_H_


#include "Cpu.h"



/*Typdeef enum decelaration*/

typedef enum pwm_mdl_tim_pwm_instance_tag
{
	TIM_PWM_INST1 = 1,

	FTM_PWM_MAX_INST
}pwm_mdl_tim_pwm_instance;


typedef enum pwm_mdl_target_tag
{
	PWM_OP_GATE_DRIVER = FTM_PWM_INST1,

}pwm_mdl_target_e;



/*Public Function*/

extern unsigned char pwm_mdl_init(pwm_mdl_target_e pwm_mdl_target);
extern unsigned char pwm_mdl_deinit(pwm_mdl_target_e pwm_mdl_target);
extern unsigned char pwm_mdl_change_duty_cycle(pwm_mdl_target_e pwm_mdl_target, unsigned short pwm_duty_cycle);



#endif /* MIDDLEWARE_LAYER_DEVICE_MDL_DRIVER_PWM_MDL_H_ */
