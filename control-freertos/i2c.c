#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "debug_console_imx.h"
#include "i2c.h"

#define MPU6050_addr 0x68

static uint8_t txBuffer[5];
static uint8_t rxBuffer[7];
static uint8_t cmdBuffer[5];

static bool I2C_MasterSendDataPolling(I2C_Type *base,
                                      const uint8_t *cmdBuff,
                                      uint32_t cmdSize,
                                      const uint8_t *txBuff,
                                      uint32_t txSize);
static bool I2C_MasterReceiveDataPolling(I2C_Type *base,
                                         const uint8_t *cmdBuff,
                                         uint32_t cmdSize,
                                         uint8_t *rxBuff,
                                         uint32_t rxSize);

void i2c_setup(void)
{

    uint8_t i;

    /* Setup I2C init structure. */
    i2c_init_config_t i2cInitConfig = {
        .baudRate = 400000u,
        .slaveAddress = 0x00};

    /* Initialize board specified hardware. */
//    hardware_init();

    /* Get current module clock frequency. */
    i2cInitConfig.clockRate = get_i2c_clock_freq(BOARD_I2C_BASEADDR);

    PRINTF("\n\r++++++++++++++++ I2C Send/Receive polling Example ++++++++++++++++\n\r");
    PRINTF("This example will configure on board accelerometer through I2C Bus\n\r");
    PRINTF("and read 10 samples back to see if the accelerometer is configured successfully. \n\r");

    PRINTF("[1].Initialize the I2C module with initialize structure. \n\r");
    I2C_Init(BOARD_I2C_BASEADDR, &i2cInitConfig);

    /* Finally, enable the I2C module */
    I2C_Enable(BOARD_I2C_BASEADDR);

/*
    PRINTF("[2].Set PWR_MGMT_1\n\r");
    cmdBuffer[0] = MPU6050_addr << 1;
    cmdBuffer[1] = 0x6B;
    txBuffer[0] = 0x00;
    PRINTF("cmdBuffer[0]: %d\n\r", cmdBuffer[0]);
    PRINTF("cmdBuffer[1]: %d\n\r", cmdBuffer[1]);
    I2C_MasterSendDataPolling(BOARD_I2C_BASEADDR, cmdBuffer, 2, txBuffer, 1);

*/

    PRINTF("[3].WHO_AM_I\n\r");
    //cmdBuffer[0] = MPU6050_addr << 1 | 0x01;
    //cmdBuffer[0] = MPU6050_addr << 1;
    cmdBuffer[0] = (MPU6050_addr << 1) + 1;
    cmdBuffer[1] = 0x75;
    PRINTF("cmdBuffer[0]: %d\n\r", cmdBuffer[0]);
    PRINTF("cmdBuffer[1]: %d\n\r", cmdBuffer[1]);
    I2C_MasterReceiveDataPolling(BOARD_I2C_BASEADDR, cmdBuffer, 2, rxBuffer, 2);
    PRINTF("rxBuffer[0]: %d \n\r",rxBuffer[0]);
    PRINTF("rxBuffer[1]: %d \n\r",rxBuffer[1]);
    PRINTF("rxBuffer[2]: %d \n\r",rxBuffer[2]);
    PRINTF("rxBuffer[3]: %d \n\r",rxBuffer[3]);
    PRINTF("rxBuffer[4]: %d \n\r",rxBuffer[4]);
    PRINTF("rxBuffer[5]: %d \n\r",rxBuffer[5]);
    
    PRINTF("\n\rExample finished!!!\n\r");
}

void report_abs(void)
{
    int16_t x, y, z;
    float Ax, Ay, Az;

    cmdBuffer[0] = MPU6050_addr << 1;
    cmdBuffer[1] = 0x01;
    cmdBuffer[2] = (MPU6050_addr << 1) + 1;
    I2C_MasterReceiveDataPolling(BOARD_I2C_BASEADDR, cmdBuffer, 3, rxBuffer, 6);

    x = ((rxBuffer[0] << 8) & 0xff00) | rxBuffer[1];
    y = ((rxBuffer[2] << 8) & 0xff00) | rxBuffer[3];
    z = ((rxBuffer[4] << 8) & 0xff00) | rxBuffer[5];
    x = (int16_t)(x) >> 2;
    y = (int16_t)(y) >> 2;
    z = (int16_t)(z) >> 2;

    Ax = x / (4.0 * 1024); //For full scale range 2g mode.
    Ay = y / (4.0 * 1024);
    Az = z / (4.0 * 1024);
    PRINTF("2G MODE: X=%6.3fg Y=%6.3fg Z=%6.3fg\n\r", Ax, Ay, Az);
}

static bool I2C_MasterSendDataPolling(I2C_Type *base,
                                      const uint8_t *cmdBuff,
                                      uint32_t cmdSize,
                                      const uint8_t *txBuff,
                                      uint32_t txSize)
{
    if (I2C_GetStatusFlag(base, i2cStatusBusBusy))
        return false;

    /* Set I2C work under Tx mode */
    I2C_SetDirMode(base, i2cDirectionTransmit);

    /* Switch to Master Mode and Send Start Signal. */
    I2C_SetWorkMode(base, i2cModeMaster);

    /* Send first byte */
    if (0 != cmdSize)
    {
        I2C_WriteByte(base, *cmdBuff++);
        cmdSize--;
    }
    else
    {
        I2C_WriteByte(base, *txBuff++);
        txSize--;
    }

    while (1)
    {
        /* Wait I2C transmission status flag assert.  */
        while (!I2C_GetStatusFlag(base, i2cStatusInterrupt));

        /* Clear I2C transmission status flag. */
        I2C_ClearStatusFlag(base, i2cStatusInterrupt);

        /* Transmit complete. */
        if ((I2C_GetStatusFlag(base, i2cStatusReceivedAck)) ||
            ((0 == txSize) && (0 == cmdSize)))
        {
            /* Switch to Slave mode and Generate a Stop Signal. */
            I2C_SetWorkMode(base, i2cModeSlave);

            /* Switch back to Rx direction. */
            I2C_SetDirMode(base, i2cDirectionReceive);
            return true;
        }
        else
        {
            if (0 != cmdSize)
            {
                I2C_WriteByte(base, *cmdBuff++);
                cmdSize--;
            }
            else
            {
                I2C_WriteByte(base, *txBuff++);
                txSize--;
            }
        }
    }
}

static bool I2C_MasterReceiveDataPolling(I2C_Type *base,
                                         const uint8_t *cmdBuff,
                                         uint32_t cmdSize,
                                         uint8_t *rxBuff,
                                         uint32_t rxSize)
{
    uint32_t currentDir = i2cDirectionReceive;

    /* Clear I2C interrupt flag to avoid spurious interrupt */
    I2C_ClearStatusFlag(base, i2cStatusInterrupt);

    if (I2C_GetStatusFlag(base, i2cStatusBusBusy))
    {
        return false;
    }

    /* Set I2C work under Tx mode */
    I2C_SetDirMode(base, i2cDirectionTransmit);

    /* Switch to Master Mode and Send Start Signal. */
    I2C_SetWorkMode(base, i2cModeMaster);

    if (0 != cmdSize)
    {
        currentDir = i2cDirectionTransmit;
        if (1 == cmdSize)
        {
            I2C_SendRepeatStart(base);
        }
        I2C_WriteByte(base, *cmdBuff++);
        cmdSize--;
    }
    else
    {
        /* Change to receive state. */
        I2C_SetDirMode(base, i2cDirectionReceive);

        if (1 == rxSize)
            /* Send Nack */
            I2C_SetAckBit(base, false);
        else
            /* Send Ack */
            I2C_SetAckBit(base, true);
        /* dummy read to clock in 1st byte */
        *rxBuff = I2C_ReadByte(base);
    }

    while (1)
    {
        /* Wait I2C transmission status flag assert.  */
        while (!I2C_GetStatusFlag(base, i2cStatusInterrupt))
            ;

        /* Clear I2C transmission status flag. */
        I2C_ClearStatusFlag(base, i2cStatusInterrupt);

        if (i2cDirectionTransmit == currentDir)
        {
            if (0 < cmdSize)
            {
                if (I2C_GetStatusFlag(base, i2cStatusReceivedAck))
                {
                    /* Switch to Slave mode and Generate a Stop Signal. */
                    I2C_SetWorkMode(base, i2cModeSlave);

                    /* Switch back to Rx direction. */
                    I2C_SetDirMode(base, i2cDirectionReceive);
                    return false;
                }
                else
                {
                    if (1 == cmdSize)
                        I2C_SendRepeatStart(base);
                    I2C_WriteByte(base, *cmdBuff++);
                    cmdSize--;
                }
            }
            else
            {
                /* Change to receive state. */
                I2C_SetDirMode(base, i2cDirectionReceive);
                currentDir = i2cDirectionReceive;

                if (1 == rxSize)
                    /* Send Nack */
                    I2C_SetAckBit(base, false);
                else
                    /* Send Ack */
                    I2C_SetAckBit(base, true);
                /* dummy read to clock in 1st byte */
                *rxBuff = I2C_ReadByte(base);
            }
        }
        else
        {
            /* Normal read operation. */
            if (2 == rxSize)
                /* Send Nack */
                I2C_SetAckBit(base, false);
            else
                /* Send Nack */
                I2C_SetAckBit(base, true);

            if (1 == rxSize)
                /* Switch back to Tx direction to avoid additional I2C bus read. */
                I2C_SetDirMode(base, i2cDirectionTransmit);
            *rxBuff = I2C_ReadByte(base);
            rxBuff++;
            rxSize--;

            /* receive finished. */
            if (0 == rxSize)
            {
                /* Switch to Slave mode and Generate a Stop Signal. */
                I2C_SetWorkMode(base, i2cModeSlave);

                /* Switch back to Rx direction. */
                I2C_SetDirMode(base, i2cDirectionReceive);

                return true;
            }
        }
    }
}
