#ifndef PWM_H_
#define PWM_H_

#include "device_imx.h"
#include "pin_mux.h"

void PWM_Init();

void PWM_Enable();

void PWM_Disable();

void PWM_SetWidth(uint16_t width);
#endif /* PWM_H_ */