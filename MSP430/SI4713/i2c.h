#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
// Example Commands ************************************************************
//******************************************************************************

// #define SLAVE_ADDR  0x63
#define MAX_BUFFER_SIZE     20

//******************************************************************************
// General I2C State Machine ***************************************************
//******************************************************************************

typedef enum I2C_ModeEnum{
    IDLE_MODE,
    NACK_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    SWITCH_TO_RX_MODE,
    SWITHC_TO_TX_MODE,
    TIMEOUT_MODE
} I2C_Mode;

/* Used to track the state of the software state machine*/
extern I2C_Mode MasterMode;

/* The Register Address/Command to use*/
extern uint8_t TransmitRegAddr;

/* ReceiveBuffer: Buffer used to receive data in the ISR
 * RXByteCtr: Number of bytes left to receive
 * ReceiveIndex: The index of the next byte to be received in ReceiveBuffer
 * TransmitBuffer: Buffer used to transmit data in the ISR
 * TXByteCtr: Number of bytes left to transfer
 * TransmitIndex: The index of the next byte to be transmitted in TransmitBuffer
 * */
extern uint8_t ReceiveBuffer[MAX_BUFFER_SIZE];
extern uint8_t RXByteCtr;
extern uint8_t ReceiveIndex;
extern uint8_t TransmitBuffer[MAX_BUFFER_SIZE];
extern uint8_t TXByteCtr;
extern uint8_t TransmitIndex;

/* I2C Write and Read Functions */

/* For slave device with dev_addr, writes the data specified in *reg_data
 *
 * dev_addr: The slave device address.
 *           Example: SLAVE_ADDR
 * reg_addr: The register or command to send to the slave.
 *           Example: CMD_TYPE_0_MASTER
 * *reg_data: The buffer to write
 *           Example: MasterType0
 * count: The length of *reg_data
 *           Example: TYPE_0_LENGTH
 *  */
I2C_Mode I2C_Master_Write(uint8_t dev_addr, uint8_t *reg_data, uint8_t count);

/* For slave device with dev_addr, read the data specified in slaves reg_addr.
 * The received data is available in ReceiveBuffer
 *
 * dev_addr: The slave device address.
 *           Example: SLAVE_ADDR
 * reg_addr: The register or command to send to the slave.
 *           Example: CMD_TYPE_0_SLAVE
 * count: The length of data to read
 *           Example: TYPE_0_LENGTH
 *  */
I2C_Mode I2C_Master_Read(uint8_t dev_addr, uint8_t *dest, uint8_t count);
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);
void initGPIO();
void initClockTo8MHz();
void initI2C();


#ifdef __cplusplus
}
#endif

#endif // I2C_H
