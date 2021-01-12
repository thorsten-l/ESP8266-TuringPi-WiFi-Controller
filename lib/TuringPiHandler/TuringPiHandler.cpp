#include <App.hpp>
#include <Arduino.h>
#include <Wire.h>
#include "TuringPiHandler.hpp"

/*
(11950) I2C SCL=5
(11953)     SDA=4
(11965) Found address: 87 (0x57)
(11966) Found address: 92 (0x5C)
(11968) Found address: 111 (0x6F)
(11969) Found 3 device(s).
*/

#define I2C_ADDRESS 0x57
#define I2C_TPI_REGISTER_CONTROL 0xf2
#define I2C_TPI_REGISTER_STATUS 0xf8

#define I2C_READ_INTERVAL 10000

// turing pi 1 slots    -     1     2     3     4     5     6     7
int slot_masks[] = {0x00, 0x02, 0x04, 0x08, 0x10, 0x80, 0x40, 0x20};

TuringPiHandler turingPiHandler;

uint8_t TuringPiHandler::readRegister(uint8_t addr)
{
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS, 1);
  return Wire.read();
}

void TuringPiHandler::writeRegister(uint8_t addr, uint8_t value)
{
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(addr);
  Wire.write(value);
  Wire.endTransmission();
}

void TuringPiHandler::readRegisters()
{
  control = readRegister(I2C_TPI_REGISTER_CONTROL);
  status = readRegister(I2C_TPI_REGISTER_STATUS);
}

void TuringPiHandler::setPower(int slot, bool power_on)
{
  control = readRegister(I2C_TPI_REGISTER_CONTROL);

  uint8_t data;

  if (power_on)
  {
    data = control | slot_masks[slot];
  }
  else
  {
    data = 0xff ^ slot_masks[slot];
    data &= control;
  }

  writeRegister(I2C_TPI_REGISTER_CONTROL, data);
  control = data;
}

bool TuringPiHandler::getPower(int slot)
{
  return (control & slot_masks[slot]) != 0;
}

// 0=unknown, 1=installed, 2=empty
int TuringPiHandler::getState(int slot)
{
  int slot_state = SLOT_STATE_UNKNOWN;

  if (getPower(slot))
  {
    if ((status & slot_masks[slot]) != 0)
    {
      slot_state = SLOT_STATE_INSTALLED;
    }
    else
    {
      slot_state = SLOT_STATE_EMPTY;
    }
  }
  else
  {
    slot_state = SLOT_STATE_UNKNOWN;
  }

  return slot_state;
}
