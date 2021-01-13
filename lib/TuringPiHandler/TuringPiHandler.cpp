#include <App.hpp>
#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <TelnetStream.h>
#include "TuringPiHandler.hpp"

/*
(11950) I2C SCL=5
(11953)     SDA=4
(11965) Found address: 87 (0x57)
(11966) Found address: 92 (0x5C)
(11968) Found address: 111 (0x6F)
(11969) Found 3 device(s).
*/

#define I2C_PE_ADDRESS 0x57
#define I2C_RTC_ADDRESS 0x6F
#define I2C_TPI_REGISTER_CONTROL 0xf2
#define I2C_TPI_REGISTER_STATUS 0xf8

#define I2C_READ_INTERVAL 10000

static char datetime_buffer[20];

// turing pi 1 slots    -     1     2     3     4     5     6     7
const int slot_masks[] = {0x00, 0x02, 0x04, 0x08, 0x10, 0x80, 0x40, 0x20};
const int daysOfMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

TuringPiHandler turingPiHandler;

uint8_t TuringPiHandler::readRegister(uint8_t bus_addr, uint8_t register_addr)
{
  Wire.beginTransmission(bus_addr);
  Wire.write(register_addr);
  Wire.endTransmission();
  Wire.requestFrom(bus_addr, ((uint8_t)1));
  return Wire.read();
}

void TuringPiHandler::writeRegister(uint8_t bus_addr, uint8_t register_addr, uint8_t value)
{
  Wire.beginTransmission(bus_addr);
  Wire.write(register_addr);
  Wire.write(value);
  Wire.endTransmission();
}

void TuringPiHandler::readRegisters()
{
  control = readRegister(I2C_PE_ADDRESS, I2C_TPI_REGISTER_CONTROL);
  status = readRegister(I2C_PE_ADDRESS, I2C_TPI_REGISTER_STATUS);
}

void TuringPiHandler::setPower(int slot, bool power_on)
{
  control = readRegister(I2C_PE_ADDRESS, I2C_TPI_REGISTER_CONTROL);

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

  writeRegister(I2C_PE_ADDRESS, I2C_TPI_REGISTER_CONTROL, data);
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

static uint8_t bcd2uint8_t(uint8_t bcd)
{
  return 10 * ((bcd & 0x70) >> 4) + (bcd & 0x0f);
}

const char *TuringPiHandler::getDateTime()
{
  Wire.beginTransmission(I2C_RTC_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(I2C_RTC_ADDRESS, 7);

  uint8_t sec = bcd2uint8_t(Wire.read());
  uint8_t min = bcd2uint8_t(Wire.read());
  uint8_t hou = bcd2uint8_t(Wire.read());
  Wire.read(); // day of week
  uint8_t dat = bcd2uint8_t(Wire.read());
  uint8_t mon = bcd2uint8_t(Wire.read());
  uint8_t yea = bcd2uint8_t(Wire.read());

  datetime_buffer[0] = 0;
  sprintf(datetime_buffer, "20%02d-%02d-%02d %02d:%02d:%02d",
          yea, mon, dat, hou, min, sec);
  datetime_buffer[19] = 0;

  // calc now
  int days = dat;
  for (int i = 1; i < mon; ++i)
  {
    days += daysOfMonth[i - 1];
  }
  if (mon > 2 && yea % 4 == 0)
  {
    days++;
  }
  days = days + 365 * yea + (yea + 3) / 4 - 1;
  rtcnow = ((days * 24UL + hou) * 60 + min) * 60 + sec;
  rtcnow += 946684800;

  return datetime_buffer;
}

time_t TuringPiHandler::getTime()
{
  getDateTime();
  return rtcnow;
}
