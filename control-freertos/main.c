/*
 * Copyright (c) 2018, Emc Logic
 */

#include "rpmsg/rpmsg_rtos.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "string.h"
#include "board.h"
#include "mu_imx.h"
#include "gpio_pins.h"
#include "MCIMX7D_M4.h"
#include "debug_console_imx.h"
#include "i2c_imx.h"

#define APP_TASK_STACK_SIZE 256
#define MPU6050_addr 0x68

char app_buf[512]; /* Each RPMSG buffer can carry less than 512 payload */

typedef struct _i2c_state {
    const uint8_t*    cmdBuff;         /*!< The buffer of I2C command. */
    const uint8_t*    txBuff;          /*!< The buffer of data being sent.*/
    uint8_t*          rxBuff;          /*!< The buffer of received data. */
    volatile uint32_t cmdSize;         /*!< The remaining number of commands to be transmitted. */
    volatile uint32_t txSize;          /*!< The remaining number of bytes to be transmitted. */
    volatile uint32_t rxSize;          /*!< The remaining number of bytes to be received. */
    volatile bool     isBusy;          /*!< True if there is an active transmission. */
    volatile uint32_t operateDir;      /*!< Overall I2C bus operating direction. */
    volatile uint32_t currentDir;      /*!< Current Data transfer direction. */
    volatile uint32_t currentMode;     /*!< Current I2C Bus role of this module. */
} i2c_state_t;

/* I2C runtime state structure */
static i2c_state_t i2cState;

static uint8_t txBuffer[5];
static uint8_t rxBuffer[7];
static uint8_t cmdBuffer[5];

static void report_abs(void);
static void I2C_XFER_Config(i2c_init_config_t* initConfig);
static bool I2C_XFER_Write(const uint8_t* cmdBuff, uint32_t cmdSize, const uint8_t* txBuffer, uint32_t txSize);
static bool I2C_XFER_Read(const uint8_t* cmdBuff, uint32_t cmdSize, uint8_t* rxBuffer, uint32_t rxSize);
static bool I2C_XFER_IsBusy(void);

/*
 * APP decided interrupt priority
 */
#define APP_MU_IRQ_PRIORITY 3

/* Globals */

/*!
 * @brief Inter-core communication Task
 */
static void CommTask(void *pvParameters)
{
    int i = 0;
    struct remote_device *rdev = NULL;
    struct rpmsg_channel *app_chnl = NULL;
    int result;
    void *rx_buf;
    int len;
    unsigned long src;
    char send[50];

    PRINTF("[RPMSG] - Init as Remote\r\n");
    result = rpmsg_rtos_init(0 /*REMOTE_CPU_ID*/, &rdev, RPMSG_MASTER, &app_chnl);
    assert(result == 0);
    PRINTF("[RPMSG] - Name service handshake is done, M4 has setup a rpmsg channel [%d ---> %d]\r\n", app_chnl->src, app_chnl->dst);
    while(1)
    {

    // Get RPMsg rx buffer with message
    result = rpmsg_rtos_recv_nocopy(app_chnl->rp_ept, &rx_buf, &len, &src, 0xFFFFFFFF);
    assert(result == 0);

    // Copy string from RPMsg rx buffer
    assert(len < sizeof(app_buf));
    memcpy(app_buf, rx_buf, len);
    app_buf[len] = 0; // End string by '\0'

    PRINTF("<--- %s\r\n", app_buf);

    switch(i){
        case 0:
        strcpy(send,"x:1,y:8,z:5\n");
        break;
        case 1:
        strcpy(send,"x:4,y:6,z:7\n");
        break;
        case 2:
        strcpy(send,"x:2,y:2,z:-3\n");
        break;
        case 3:
        strcpy(send,"x:3,y:1,z:-2\n");
        break;
        case 4:
        strcpy(send,"x:4,y:2,z:-1\n");
        break;
        case 5:
        strcpy(send,"x:3,y:3,z:2\n");
        break;
        case 6:
        strcpy(send,"x:6,y:3,z:3\n");
        break;
        case 7:
        strcpy(send,"x:5,y:6,z:8\n");
        break;
        case 8:
        strcpy(send,"x:4,y:7,z:1\n");
        break;
    }
    i++;
    if( i > 8) i=0;

    PRINTF("---> %s\r\n", send);
    rpmsg_send(app_chnl, send, strlen(send));

/*
    switch (app_buf[0])
    {
    case 'a':
        strcpy(send,"x:10,y:20,z:30");
        rpmsg_send(app_chnl, send, strlen(send));
        break;

    case 'g':
        strcpy(send,"x:30,y:50,z:40");
        rpmsg_send(app_chnl, send, strlen(send));
        break;
    default:
            PRINTF("[COM] - Error \r\n");
    }
 */

    // Release held RPMsg rx buffer
    result = rpmsg_rtos_recv_nocopy_free(app_chnl->rp_ept, rx_buf);
    assert(result == 0);
    }
}

int main(void)
{
     uint8_t i;

    /* Setup I2C init structure. */
    i2c_init_config_t i2cInitConfig = {
        .baudRate     = 400000u,
        .slaveAddress = 0x00
    };

    /* Initialize board specified hardware. */
    hardware_init();

    /* Get current module clock frequency. */
    i2cInitConfig.clockRate = get_i2c_clock_freq(BOARD_I2C_BASEADDR);

/*
    PRINTF("\n\r++++++++++++++++ I2C Send/Receive interrupt Example ++++++++++++++++\n\r");
    PRINTF("This example will configure on board accelerometer through I2C Bus\n\r");
    PRINTF("and read 10 samples back to see if the accelerometer is configured successfully. \n\r\n\r");

    PRINTF("[1].Initialize the I2C module with initialize structure. \n\r");
    I2C_XFER_Config(&i2cInitConfig);

    I2C_Enable(BOARD_I2C_BASEADDR);

     PRINTF("[3].WHO_AM_I\n\r");
    //cmdBuffer[0] = MPU6050_addr << 1 | 0x01;
    //cmdBuffer[0] = MPU6050_addr << 1;
    cmdBuffer[0] = MPU6050_addr << 1;
    cmdBuffer[1] = 0x75;
    cmdBuffer[2] = (MPU6050_addr << 1) + 1;
    PRINTF("cmdBuffer[0]: %d\n\r", cmdBuffer[0]);
    PRINTF("cmdBuffer[1]: %d\n\r", cmdBuffer[1]);
    PRINTF("cmdBuffer[2]: %d \n\r",cmdBuffer[2]);
    while (I2C_XFER_IsBusy());
    PRINTF("rxBuffer[0]: %d\n\r", rxBuffer[0]);
    if (0xC7 == rxBuffer[0])
        PRINTF("OK\n\r");
    else
        PRINTF("ERROR\n\r");

    PRINTF("\n\rExample finished!!!\n\r");
    */

    /*
     * Prepare for the MU Interrupt
     *  MU must be initialized before rpmsg init is called
     */
    MU_Init(BOARD_MU_BASE_ADDR);
    NVIC_SetPriority(BOARD_MU_IRQ_NUM, APP_MU_IRQ_PRIORITY);
    NVIC_EnableIRQ(BOARD_MU_IRQ_NUM);

    xTaskCreate(CommTask, "Communication Task", APP_TASK_STACK_SIZE,
                    NULL, tskIDLE_PRIORITY + 1, NULL);

    PRINTF("Open AMP Demo V1.0\r\n");

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Should never reach this point. */
    while (true);
}

/* IRQ Handlers */

/*
 * MU Interrrupt ISR
 */
void BOARD_MU_HANDLER(void)
{
    /*
     * calls into rpmsg_handler provided by middleware
     */
    rpmsg_handler();
}

static void I2C_XFER_Config(i2c_init_config_t* initConfig)
{
    /* Initialize I2C state structure content. */
    i2cState.cmdBuff = 0;
    i2cState.txBuff = 0;
    i2cState.rxBuff = 0;
    i2cState.cmdSize = 0;
    i2cState.txSize = 0;
    i2cState.rxSize = 0;
    i2cState.isBusy = false;
    i2cState.operateDir = i2cDirectionReceive;
    i2cState.currentDir = i2cDirectionReceive;
    i2cState.currentMode = i2cModeSlave;

    /* Initialize I2C baud rate, mode, transfer direction and slave address. */
    I2C_Init(BOARD_I2C_BASEADDR, initConfig);

    /* Set I2C Interrupt priority */
    NVIC_SetPriority(BOARD_I2C_IRQ_NUM, 3);

    /* Call core API to enable the IRQ. */
    NVIC_EnableIRQ(BOARD_I2C_IRQ_NUM);

    /* Finally, enable the I2C module */
    I2C_Enable(BOARD_I2C_BASEADDR);
}

static bool I2C_XFER_Write(const uint8_t* cmdBuff, uint32_t cmdSize,
                      const uint8_t* txBuffer, uint32_t txSize)
{
    if ((i2cState.isBusy) || (0 == txSize))
        return false;

    /* Initialize i2c transfer struct */
    i2cState.cmdBuff = cmdBuff;
    i2cState.cmdSize = cmdSize;
    i2cState.txBuff = txBuffer;
    i2cState.txSize = txSize;
    i2cState.isBusy = true;
    i2cState.operateDir = i2cDirectionTransmit;

    /* Clear I2C interrupt flag to avoid spurious interrupt */
    I2C_ClearStatusFlag(BOARD_I2C_BASEADDR, i2cStatusInterrupt);

    if (I2C_GetStatusFlag(BOARD_I2C_BASEADDR, i2cStatusBusBusy))
    {
        /* Reset i2c transfer state. */
        i2cState.operateDir = i2cDirectionReceive;
        i2cState.isBusy = false;
        return false;
    }

    /* Set I2C work under Tx mode */
    I2C_SetDirMode(BOARD_I2C_BASEADDR, i2cDirectionTransmit);
    i2cState.currentDir = i2cDirectionTransmit;

    /* Switch to Master Mode and Send Start Signal. */
    I2C_SetWorkMode(BOARD_I2C_BASEADDR, i2cModeMaster);
    i2cState.currentMode = i2cModeMaster;

    if (0 != cmdSize)
    {
        I2C_WriteByte(BOARD_I2C_BASEADDR, *i2cState.cmdBuff);
        i2cState.cmdBuff++;
        i2cState.cmdSize--;
    }
    else
    {
        I2C_WriteByte(BOARD_I2C_BASEADDR, *i2cState.txBuff);
        i2cState.txBuff++;
        i2cState.txSize--;
    }

    /* Enable I2C interrupt, subsequent data transfer will be handled in ISR. */
    I2C_SetIntCmd(BOARD_I2C_BASEADDR, true);

    return true;
}

static bool I2C_XFER_Read(const uint8_t* cmdBuff, uint32_t cmdSize,
                     uint8_t* rxBuffer, uint32_t rxSize)
{
    if ((i2cState.isBusy) || (0 == rxSize))
        return false;

    /* Initialize i2c transfer struct */
    i2cState.cmdBuff = cmdBuff;
    i2cState.cmdSize = cmdSize;
    i2cState.rxBuff = rxBuffer;
    i2cState.rxSize = rxSize;
    i2cState.isBusy = true;
    i2cState.operateDir = i2cDirectionReceive;

    /* Clear I2C interrupt flag to avoid spurious interrupt */
    I2C_ClearStatusFlag(BOARD_I2C_BASEADDR, i2cStatusInterrupt);

    if (I2C_GetStatusFlag(BOARD_I2C_BASEADDR, i2cStatusBusBusy))
    {
        /* Reset i2c transfer state. */
        i2cState.operateDir = i2cDirectionReceive;
        i2cState.isBusy = false;
        return false;
    }

    /* Set I2C work under Tx mode */
    I2C_SetDirMode(BOARD_I2C_BASEADDR, i2cDirectionTransmit);
    i2cState.currentDir = i2cDirectionTransmit;

    /* Switch to Master Mode and Send Start Signal. */
    I2C_SetWorkMode(BOARD_I2C_BASEADDR, i2cModeMaster);
    i2cState.currentMode = i2cModeMaster;

    /* Is there command to be sent before receive data? */
    if (0 != i2cState.cmdSize)
    {
        if (1 == i2cState.cmdSize)
            I2C_SendRepeatStart(BOARD_I2C_BASEADDR);
        I2C_WriteByte(BOARD_I2C_BASEADDR, *i2cState.cmdBuff);
        i2cState.cmdBuff++;
        i2cState.cmdSize--;
    }
    else
    {
        /* Change to receive state. */
        I2C_SetDirMode(BOARD_I2C_BASEADDR, i2cDirectionReceive);
        i2cState.currentDir = i2cDirectionReceive;

        if (1 == rxSize)
            /* Send Nack */
            I2C_SetAckBit(BOARD_I2C_BASEADDR, false);
        else
            /* Send Ack */
            I2C_SetAckBit(BOARD_I2C_BASEADDR, true);
        /* dummy read to clock in 1st byte */
        I2C_ReadByte(BOARD_I2C_BASEADDR);
    }

    /* Enable I2C interrupt, subsequent data transfer will be handled in ISR. */
    I2C_SetIntCmd(BOARD_I2C_BASEADDR, true);

    return true;
}

static bool I2C_XFER_IsBusy(void)
{
    return i2cState.isBusy;
}

void BOARD_I2C_HANDLER(void)
{
    /* Clear interrupt flag. */
    I2C_ClearStatusFlag(BOARD_I2C_BASEADDR, i2cStatusInterrupt);

    /* Exit the ISR if no transfer is happening for this instance. */
    if (!i2cState.isBusy)
        return;

    if (i2cModeMaster == i2cState.currentMode)
    {
        if (i2cDirectionTransmit == i2cState.currentDir)
        {
            if ((I2C_GetStatusFlag(BOARD_I2C_BASEADDR, i2cStatusReceivedAck)) ||
                ((0 == i2cState.txSize) && (0 == i2cState.cmdSize)))
            {
                if ((i2cDirectionTransmit == i2cState.operateDir) ||
                    (I2C_GetStatusFlag(BOARD_I2C_BASEADDR, i2cStatusReceivedAck)))
                {
                    /* Switch to Slave mode and Generate a Stop Signal. */
                    I2C_SetWorkMode(BOARD_I2C_BASEADDR, i2cModeSlave);
                    i2cState.currentMode = i2cModeSlave;

                    /* Switch back to Rx direction. */
                    I2C_SetDirMode(BOARD_I2C_BASEADDR, i2cDirectionReceive);
                    i2cState.currentDir = i2cDirectionReceive;

                    /* Close I2C interrupt. */
                    I2C_SetIntCmd(BOARD_I2C_BASEADDR, false);
                    /* Release I2C Bus. */
                    i2cState.isBusy = false;
                }
                else
                {
                    /* Switch back to Rx direction. */
                    I2C_SetDirMode(BOARD_I2C_BASEADDR, i2cDirectionReceive);
                    i2cState.currentDir = i2cDirectionReceive;

                    if (1 == i2cState.rxSize)
                        /* Send Nack */
                        I2C_SetAckBit(BOARD_I2C_BASEADDR, false);
                    else
                        /* Send Ack */
                        I2C_SetAckBit(BOARD_I2C_BASEADDR, true);
                    /* dummy read to clock in 1st byte */
                    *i2cState.rxBuff = I2C_ReadByte(BOARD_I2C_BASEADDR);
                }
            }
            else
            {
                if (0 != i2cState.cmdSize)
                {
                    if ((1 == i2cState.cmdSize) && (i2cDirectionReceive == i2cState.operateDir))
                        I2C_SendRepeatStart(BOARD_I2C_BASEADDR);
                    I2C_WriteByte(BOARD_I2C_BASEADDR, *i2cState.cmdBuff);
                    i2cState.cmdBuff++;
                    i2cState.cmdSize--;
                }
                else
                {
                    I2C_WriteByte(BOARD_I2C_BASEADDR, *i2cState.txBuff);
                    i2cState.txBuff++;
                    i2cState.txSize--;
                }
            }
        }
        else
        {
            /* Normal read operation. */
            if (2 == i2cState.rxSize)
                /* Send Nack */
                I2C_SetAckBit(BOARD_I2C_BASEADDR, false);
            else
                /* Send Nack */
                I2C_SetAckBit(BOARD_I2C_BASEADDR, true);

            if (1 == i2cState.rxSize)
            {
                /* Switch back to Tx direction to avoid additional I2C bus read. */
                I2C_SetDirMode(BOARD_I2C_BASEADDR, i2cDirectionTransmit);
                i2cState.currentDir = i2cDirectionTransmit;
            }
            *i2cState.rxBuff = I2C_ReadByte(BOARD_I2C_BASEADDR);
            i2cState.rxBuff++;
            i2cState.rxSize--;

            /* receive finished. */
            if (0 == i2cState.rxSize)
            {
                /* Switch to Slave mode and Generate a Stop Signal. */
                I2C_SetWorkMode(BOARD_I2C_BASEADDR, i2cModeSlave);
                i2cState.currentMode = i2cModeSlave;

                /* Switch back to Rx direction. */
                I2C_SetDirMode(BOARD_I2C_BASEADDR, i2cDirectionReceive);
                i2cState.currentDir = i2cDirectionReceive;

                /* Close I2C interrupt. */
                I2C_SetIntCmd(BOARD_I2C_BASEADDR, false);
                /* Release I2C Bus. */
                i2cState.isBusy = false;
            }
        }
    }
}

