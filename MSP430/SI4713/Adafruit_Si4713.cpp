/* #############################################################################################
* Authors     : Lim Zi Yang, Yao Hejun
* Emails      : e1398962@u.nus.edu, e1113781@u.nus.edu
* Adapted from Adafruit_Si4713.cpp
################################################################################################
*/

#include <msp430.h>
#include <stdio.h>
#include "driverlib.h"
#include "Adafruit_Si4713.h"

extern void ser_print(const char *message);
extern void ser_printa(uint8_t* array, int array_len);

char testbuf[16];

Adafruit_Si4713::Adafruit_Si4713(int8_t resetpin) { _rst = resetpin; }

/**
 * This checks if there is a device at the location.
 */
void Adafruit_Si4713::sendEmpty() {
  uint8_t dummy[1] = {0};
  I2C_Master_Write(SI4710_ADDR1, dummy, 0); // zero-length write, ack
}

bool Adafruit_Si4713::begin() {
  sendEmpty();

  reset();

  powerUp();
  __delay_cycles(4000000);

  // if (!scan_devices())
  //   return false;

  // check for Si4713
  if (getRev() != 13)
      return false;

  return true;
}

bool scan_devices() {
  ser_print("Starting scan\n");

  for (uint8_t address = 0x01; address <= 0x75; address++) {
      EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, address);
      // Try sending a start + write to see if slave responds
      if (EUSCI_B_I2C_masterSendSingleByteWithTimeout(EUSCI_B0_BASE, 0x00, 1000)) {
          char buffer[32];
          sprintf(buffer, "Found I2C device at 0x%x\n", address);
          ser_print(buffer);
          return true;
      }
  }

  return false;
}

void Adafruit_Si4713::reset() {
  // P1DIR |= _rst;
  P1OUT |= _rst;
  __delay_cycles(4000000);
  P1OUT &= ~_rst;
  __delay_cycles(8000);
  P1OUT |= _rst;
  __delay_cycles(4000000);
  ser_print("Reset Complete\n");
}

void Adafruit_Si4713::setProperty(uint16_t property, uint16_t value) {
  _i2ccommand[0] = SI4710_CMD_SET_PROPERTY;
  _i2ccommand[1] = 0;
  _i2ccommand[2] = property >> 8;
  _i2ccommand[3] = property & 0xFF;
  _i2ccommand[4] = value >> 8;
  _i2ccommand[5] = value & 0xFF;
  sendCommand(6);

#ifdef SI4713_CMD_DEBUG
  ser_print("Set Prop ");
  sprintf(testbuf, "%d = %d\n", property, value);
  ser_print(testbuf);
#endif
}

void Adafruit_Si4713::sendCommand(uint8_t len) {
  // Send command
  I2C_Master_Write(SI4710_ADDR1, _i2ccommand, len);
  // Wait for status CTS bit
  uint8_t status[2] = {0, 0};
  while (!(status[0] & SI4710_STATUS_CTS))
    I2C_Master_Read(SI4710_ADDR1, status, 2);
}

void Adafruit_Si4713::setTXpower(uint8_t pwr, uint8_t antcap) {
  _i2ccommand[0] = SI4710_CMD_TX_TUNE_POWER;
  _i2ccommand[1] = 0;
  _i2ccommand[2] = 0;
  _i2ccommand[3] = pwr;
  _i2ccommand[4] = antcap;
  sendCommand(5);
}

void Adafruit_Si4713::tuneFM(uint16_t freqKHz) {
  _i2ccommand[0] = SI4710_CMD_TX_TUNE_FREQ;
  _i2ccommand[1] = 0;
  _i2ccommand[2] = freqKHz >> 8;
  _i2ccommand[3] = freqKHz;
  sendCommand(4);
  while ((getStatus() & 0x81) != 0x81) {
    __delay_cycles(10);
  }
}

void Adafruit_Si4713::powerUp() {
  _i2ccommand[0] = SI4710_CMD_POWER_UP;
  _i2ccommand[1] = 0x12;
  // CTS interrupt disabled
  // GPO2 output disabled
  // Boot normally
  // xtal oscillator ENabled
  // FM transmit
  _i2ccommand[2] = 0x50; // analog input mode
  sendCommand(3);

  // configuration! see page 254
  setProperty(SI4713_PROP_REFCLK_FREQ, 32768); // crystal is 32.768
  setProperty(SI4713_PROP_TX_PREEMPHASIS, 0);  // 74uS pre-emph (USA std)
  setProperty(SI4713_PROP_TX_ACOMP_GAIN, 10);  // max gain?
  // setProperty(SI4713_PROP_TX_ACOMP_ENABLE, 0x02); // turn on limiter, but no
  // dynamic ranging
  setProperty(SI4713_PROP_TX_ACOMP_ENABLE, 0x0); // turn on limiter and AGC
  ser_print("Powerup Complete\n");
}

uint8_t Adafruit_Si4713::getRev() {
  _i2ccommand[0] = SI4710_CMD_GET_REV;
  _i2ccommand[1] = 0;
  sendCommand(2);

  uint8_t pn, resp[9];
  I2C_Master_Read(SI4710_ADDR1, resp, 9);
  pn = resp[1];

#ifdef SI4713_CMD_DEBUG
  uint8_t fw, patch, cmp, chiprev;
  fw = (uint16_t(resp[2]) << 8) | resp[3];
  patch = (uint16_t(resp[4]) << 8) | resp[5];
  cmp = (uint16_t(resp[6]) << 8) | resp[7];
  chiprev = resp[8];

  ser_print("Part # Si47");
  sprintf(testbuf, "%d\n", pn);
  ser_print(testbuf);
  ser_print("-");
  sprintf(testbuf, "%d\n", fw);
  ser_print(testbuf);
  ser_print("Firmware 0x");
  sprintf(testbuf, "%d\n", fw);
  ser_print(testbuf);
  ser_print("Patch 0x");
  sprintf(testbuf, "%d\n", patch);
  ser_print(testbuf);
  ser_print("Chip rev ");
  sprintf(testbuf, "%d\n", chiprev);
  ser_print(testbuf);
#endif

  return pn;
}

uint8_t Adafruit_Si4713::getStatus() {
  uint8_t resp[2] = {SI4710_CMD_GET_INT_STATUS, 0};
  I2C_Master_Write(SI4710_ADDR1, resp, 1);
  I2C_Master_Read(SI4710_ADDR1, resp, 2);
  return resp[0];
}

void Adafruit_Si4713::setGPIOctrl(uint8_t x) {
#ifdef SI4713_CMD_DEBUG
  ser_print("GPIO direction");
#endif
  _i2ccommand[0] = SI4710_CMD_GPO_CTL;
  _i2ccommand[1] = x;
  sendCommand(2);
}

void Adafruit_Si4713::readTuneStatus() {
  _i2ccommand[0] = SI4710_CMD_TX_TUNE_STATUS;
  _i2ccommand[1] = 0x1; // INTACK
  sendCommand(2);

  uint8_t resp[8];
  I2C_Master_Read(SI4710_ADDR1, resp, 8);
  currFreq = (uint16_t(resp[2]) << 8) | resp[3];
  currdBuV = resp[5];
  currAntCap = resp[6];
  currNoiseLevel = resp[7];
}
