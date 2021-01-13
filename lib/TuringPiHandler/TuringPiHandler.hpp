#ifndef __TURING_PI_HANDLER_H__
#define __TURING_PI_HANDLER_H__

#define SLOT_STATE_UNKNOWN 0
#define SLOT_STATE_INSTALLED 1
#define SLOT_STATE_EMPTY 2

class TuringPiHandler
{
private:
  uint8_t control;
  uint8_t status;
  time_t loopTimestamp = 0l;
  time_t rtcnow;

  uint8_t readRegister( uint8_t bus_addr, uint8_t register_addr);
  void writeRegister(uint8_t bus_addr, uint8_t register_addr, uint8_t value);

public:
  void readRegisters();
  void setPower( int slot, bool on );
  bool getPower( int slot );
  int getState( int slot ); // 0=unknown, 1=installed, 2=empty
  const char* getDateTime();
  time_t getTime();
};

extern TuringPiHandler turingPiHandler;

#endif
