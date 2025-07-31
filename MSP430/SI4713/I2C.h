/* #############################################################################################
* Authors     : Lim Zi Yang, Yao Hejun
* Emails      : e1398962@u.nus.edu, e1113781@u.nus.edu
* Adapted from MSP430FR59xx Demo - eUSCI_B0, I2C Master multiple byte TX/RX
################################################################################################
*/

#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

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
 * DO NOT SET COUNT < 2 (IF NOT STOP MISINTERPRETED AS NACK)
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

#ifdef __cplusplus
}
#endif

#endif // I2C_H
