/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include "gpio_pins.h"
#include "MCIMX7D_M4.h"
//#include "gpio_imx.h"

/* 
* SODIMM                BALL NAME                GPIO                ALT
*    88                 I2C2_SCL           GPIO4_10                  5
*/
gpio_config_t gpioOVI = {
    "OVI",                                 /* name */
    &IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL,       /* muxReg */
    5,                                     /* muxConfig */
    &IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL,       /* padReg */
    IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL_PS(2) | /* padConfig */
        IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL_HYS_MASK,
    GPIO4, /* base */
    10     /* pin */
};
gpio_init_config_t OVI = {
    .pin = 10, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

/* 
* SODIMM                BALL NAME                GPIO                ALT
*    86                 I2C2_SDA                GPIO4_11                  5
*/
gpio_config_t gpioOVO = {
    "OVO",                                 /* name */
    &IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA,       /* muxReg */
    5,                                     /* muxConfig */
    &IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA,       /* padReg */
    IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA_PS(2) | /* padConfig */
        IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA_HYS_MASK,
    GPIO4, /* base */
    11     /* pin */
};
gpio_init_config_t OVO = {
    .pin = 11, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

// SODIMM                BALL NAME                GPIO                ALT
//    90                 I2C1_SCL                GPIO4_8                  5

gpio_config_t gpioOC = {
    "OC",                                 
    &IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL,      
    5,                                    
    &IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL,      
    IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL_HYS_MASK,
    GPIO4, 
    8     
};
gpio_init_config_t OC = {
    .pin = 8, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

// SODIMM                BALL NAME                GPIO                ALT
//    92                 I2C1_SDA                GPIO4_9                  5

gpio_config_t gpioOT = {
    "OT",                                 
    &IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA,      
    5,                                    
    &IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA,      
    IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA_HYS_MASK,
    GPIO4, 
    9     
};
gpio_init_config_t OT = {
    .pin = 9, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

// SODIMM                BALL NAME                GPIO                ALT
//    98                 SD2_RESET_B                GPIO5_11                  5

gpio_config_t gpioTRIGGER = {
    "TRIGGER",                                 
    &IOMUXC_SW_PAD_CTL_PAD_SD2_RESET_B,       
    5,                                     
    &IOMUXC_SW_PAD_CTL_PAD_SD2_RESET_B,       
    IOMUXC_SW_PAD_CTL_PAD_SD2_RESET_B_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_SD2_RESET_B_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_SD2_RESET_B_HYS_MASK,
    GPIO5, 
    11     
};
gpio_init_config_t TRIGGER = {
    .pin = 11, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

// SODIMM                BALL NAME                GPIO                ALT
//    133                 EPDC_GDRL                GPIO2_26                  5
gpio_config_t gpioPEDAL = {
    "PEDAL",                                 
    &IOMUXC_SW_PAD_CTL_PAD_EPDC_GDRL,       
    5,                                     
    &IOMUXC_SW_PAD_CTL_PAD_EPDC_GDRL,      
    IOMUXC_SW_PAD_CTL_PAD_EPDC_GDRL_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_EPDC_GDRL_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_EPDC_GDRL_HYS_MASK,
    GPIO2, 
    26     
};
gpio_init_config_t PEDAL = {
    .pin = 26, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};


// SODIMM                BALL NAME                GPIO                ALT
//    103                 ECSP1_MOSI                GPIO4_17                  5
gpio_config_t gpioKEY = {
    "KEY",                                 
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MOSI,       
    5,                                     
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MOSI,     
    IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MOSI_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MOSI_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_ECSPI1_MOSI_HYS_MASK,
    GPIO4, 
    17     
};
gpio_init_config_t KEY = {
    .pin = 17, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

// SODIMM                BALL NAME                GPIO                ALT
//    101                 ECSPI1_SCLK                GPIO4_16                  5
gpio_config_t gpioAC = {
    "AC",                                 
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SCLK,       
    5,                                     
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SCLK,    
    IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SCLK_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SCLK_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SCLK_HYS_MASK,
    GPIO4, 
    16     
};
gpio_init_config_t AC = {
    .pin = 16, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

// SODIMM                BALL NAME                GPIO                ALT
//    97                 ECSPI1_SS0                GPIO4_19                  5
gpio_config_t gpioDOOR = {
    "DOOR",                                 
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SS0,      
    5,                                     
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SS0,     
    IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SS0_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SS0_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_ECSPI1_SS0_HYS_MASK,
    GPIO4, 
    19     
};
gpio_init_config_t DOOR = {
    .pin = 19, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};

// SODIMM                BALL NAME                GPIO                ALT
//   85                 ECSPI12_MISO                GPIO4_22                  5

gpio_config_t gpioLASEREN = {
    "LASEREN",                                 
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MISO,       
    5,                                     
    &IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MISO,       
    IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MISO_PS(2) | 
        IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MISO_PE_MASK |
        IOMUXC_SW_PAD_CTL_PAD_ECSPI2_MISO_HYS_MASK,
    GPIO4, 
    22     
};
gpio_init_config_t LASEREN = {
    .pin = 22, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};


// TODO - Enable BOMBEN GPIO
/*
// SODIMM                BALL NAME                GPIO                ALT
//   45                 GPIO1_IO01                GPIO1_1                  0

gpio_config_t gpioBOMBEN = {
    "BOMBEN",
    &IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO01,
    0,
    &IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO01,
    IOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO1_IO01_PS(2) |
        IOMUXC_SW_PAD_CTL_GPIO_IO01_PE_MASK |
        IOMUXC_SW_PAD_CTL_GPIO_IO01_HYS_MASK,
    GPIO1,
    1};
gpio_init_config_t BOMBEN = {
    .pin = 1, //pin number
    .direction = gpioDigitalInput,
    .interruptMode = gpioIntRisingEdge
};
*/

void configure_platform_gpio(void)
{

    GPIO_Init(gpioOVI.base, &OVI);
    GPIO_Init(gpioOVO.base, &OVO);
    GPIO_Init(gpioOC.base, &OC);
    GPIO_Init(gpioOT.base, &OT);
    GPIO_Init(gpioTRIGGER.base, &TRIGGER);
    GPIO_Init(gpioPEDAL.base, &PEDAL);
    GPIO_Init(gpioKEY.base, &KEY);
    GPIO_Init(gpioAC.base, &AC);
    GPIO_Init(gpioDOOR.base, &DOOR);
    GPIO_Init(gpioLASEREN.base, &DOOR);

    /* Clear the interrupt state, this operation is necessary, because the GPIO module maybe confuse
       the first rising edge as interrupt*/
    GPIO_ClearStatusFlag(gpioOVI.base, gpioOVI.pin);
    GPIO_ClearStatusFlag(gpioOVO.base, gpioOVO.pin);
    GPIO_ClearStatusFlag(gpioOC.base, gpioOC.pin);
    GPIO_ClearStatusFlag(gpioOT.base, gpioOT.pin);
    GPIO_ClearStatusFlag(gpioTRIGGER.base, gpioTRIGGER.pin);
    GPIO_ClearStatusFlag(gpioPEDAL.base, gpioPEDAL.pin);
    GPIO_ClearStatusFlag(gpioKEY.base, gpioKEY.pin);
    GPIO_ClearStatusFlag(gpioAC.base, gpioAC.pin);
    GPIO_ClearStatusFlag(gpioDOOR.base, gpioDOOR.pin);
    GPIO_ClearStatusFlag(gpioLASEREN.base, gpioLASEREN.pin);

//TODO - CHECK IF NEEDED
/* 
    IOMUXC_I2C2_SCL_SELECT_INPUT = 1;
    IOMUXC_I2C2_SDA_SELECT_INPUT = 1;
    IOMUXC_I2C1_SCL_SELECT_INPUT = 1;
    IOMUXC_I2C1_SDA_SELECT_INPUT = 1;
//TODO    IOMUXC_SD2_RESET_B_SELECT_INPUT = 1;
//TODO    IOMUXC_EPDC_GDRL_SELECT_INPUT = 1;
    IOMUXC_ECSPI1_MOSI_SELECT_INPUT = 1;
//TODO    IOMUXC_ECSPI1_SS0_SELECT_INPUT = 1;
*/

    /* Enable GPIO pin interrupt */
    GPIO_SetPinIntMode(gpioOVI.base, gpioOVI.pin, true);
    GPIO_SetIntEdgeSelect(gpioOVI.base, gpioOVI.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioOVO.base, gpioOVO.pin, true);
    GPIO_SetIntEdgeSelect(gpioOVO.base, gpioOVO.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioOC.base, gpioOC.pin, true);
    GPIO_SetIntEdgeSelect(gpioOC.base, gpioOC.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioOT.base, gpioOT.pin, true);
    GPIO_SetIntEdgeSelect(gpioOT.base, gpioOT.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioTRIGGER.base, gpioTRIGGER.pin, true);
    GPIO_SetIntEdgeSelect(gpioTRIGGER.base, gpioTRIGGER.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioPEDAL.base, gpioPEDAL.pin, true);
    GPIO_SetIntEdgeSelect(gpioPEDAL.base, gpioPEDAL.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioKEY.base, gpioKEY.pin, true);
    GPIO_SetIntEdgeSelect(gpioKEY.base, gpioKEY.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioKEY.base, gpioKEY.pin, true);
    GPIO_SetIntEdgeSelect(gpioKEY.base, gpioKEY.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioAC.base, gpioAC.pin, true);
    GPIO_SetIntEdgeSelect(gpioAC.base, gpioAC.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioDOOR.base, gpioDOOR.pin, true);
    GPIO_SetIntEdgeSelect(gpioDOOR.base, gpioDOOR.pin, true); //Active on any edge

    GPIO_SetPinIntMode(gpioLASEREN.base, gpioLASEREN.pin, true);
    GPIO_SetIntEdgeSelect(gpioLASEREN.base, gpioLASEREN.pin, true); //Active on any edge

}

/*******************************************************************************
 * EOF
 ******************************************************************************/
