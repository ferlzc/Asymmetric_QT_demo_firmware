#include "pwm.h"
#include "board.h"
#include "MCIMX7D_M4.h"

void PWM_Init() {
    //            Repeat once         | Prescaler              | Clock hf = 24Mhz           | Set at rollover    |
//    PWM2->PWMCR = PWM_PWMCR_REPEAT(1) | PWM_PWMCR_PRESCALER(0) | PWM_PWMCR_CLKSRC(2) | PWM_PWMCR_POUTC(0);

    PWM2->PWMCR = PWM_PWMCR_REPEAT(1) | PWM_PWMCR_PRESCALER(0xfff) | PWM_PWMCR_CLKSRC(2) | PWM_PWMCR_POUTC(0);
    PWM2->PWMSAR = 1; //First sample

    //Check errors
    if (PWM2->PWMSR && PWM_PWMSR_FWE_MASK)
        PWM2->PWMSR = PWM_PWMSR_FWE_MASK;
    if (PWM2->PWMSR && PWM_PWMSR_ROV_MASK)
        PWM2->PWMSR = PWM_PWMSR_ROV_MASK;
    if (PWM2->PWMSR && PWM_PWMSR_CMP_MASK)
        PWM2->PWMSR = PWM_PWMSR_CMP_MASK;

    //Set period
    PWM2->PWMPR = 2058;

}

void PWM_Enable() {
    PWM2->PWMCR = PWM2->PWMCR | PWM_PWMCR_EN_MASK;
}

void PWM_Disable() {
    PWM2->PWMCR = PWM2->PWMCR & ~PWM_PWMCR_EN_MASK;
}

void PWM_SetWidth(uint16_t width) {
    if (width <= 65534) {
    //if (width <= 1024) {
        PWM2->PWMSAR = width;
    }
}

void PWM_SetPeriod(uint16_t period) {
    if (period <= 65534) {
    //if (period <= 1024) {
        PWM2->PWMPR = period;
    }
}