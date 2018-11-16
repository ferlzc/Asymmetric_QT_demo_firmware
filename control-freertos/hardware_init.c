/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 */

#include "board.h"
#include "pin_mux.h"
#include "gpio_pins.h"
#include "ccm_imx7d.h"

void hardware_init(void)
{
    /* Board specific RDC settings */
    BOARD_RdcInit();

    /* Board specific clock settings */
    BOARD_ClockInit();

    /* initialize debug uart */
    dbg_uart_init();

    /* Configure GPIOS */
//    configure_platform_gpio();

    /* In this demo, we need to grasp board GPT exclusively */
//    RDC_SetPdapAccess(RDC, BOARD_GPTA_RDC_PDAP, 3 << (BOARD_DOMAIN_ID * 2), false, false);

    /* In this demo, we need to access RDC SEMAPHORE1 on this board */
//    RDC_SetPdapAccess(RDC, rdcPdapSemaphore1, 0xFF, false, false);

    /* Enable RDC SEMAPHORE GATE needed in this demo */
    CCM_ControlGate(CCM, ccmCcgrGateSema1, ccmClockNeededRunWait);

    /* RDC MU*/
    RDC_SetPdapAccess(RDC, BOARD_MU_RDC_PDAP, 3 << (BOARD_DOMAIN_ID * 2), false, false);

    /* Enable clock gate for MU*/
    CCM_ControlGate(CCM, BOARD_MU_CCM_CCGR, ccmClockNeededRun);

    /* I2C4 */

    /* In this example, we need to grasp board I2C exclusively */
    RDC_SetPdapAccess(RDC, BOARD_I2C_RDC_PDAP, 3 << (BOARD_DOMAIN_ID * 2), false, false);

    /* Select I2C clock derived from OSC clock(24M) */
    CCM_UpdateRoot(CCM, BOARD_I2C_CCM_ROOT, ccmRootmuxI2cOsc24m, 0, 0);
    /* Enable I2C clock */
    CCM_EnableRoot(CCM, BOARD_I2C_CCM_ROOT);
    CCM_ControlGate(CCM, BOARD_I2C_CCM_CCGR, ccmClockNeededRunWait);

    /* I2C Pin setting */
    configure_i2c_pins(BOARD_I2C_BASEADDR);

}

/*******************************************************************************
 * EOF
 ******************************************************************************/
