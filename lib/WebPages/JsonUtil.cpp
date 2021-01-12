#include <App.hpp>
#include <WifiHandler.hpp>
#include <ESP8266WebServer.h>
#include <TuringPiHandler.hpp>
#include <Util.hpp>
#include "WebPages.h"

extern ESP8266WebServer server;

static String tpiStatusStr[] = { "-", "installed", "emtpy" };

void handleJsonStatusState()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  sendHeaderNoCache();

  turingPiHandler.readRegisters();

  int idx = sprintf(buffer, "{\"state\":[");

  for (int slot = 1; slot < 8; slot++)
  {
    idx += sprintf(buffer + idx, "{\"slot\":%d,\"power_on\":%s,\"status\":%d,\"status_str\":\"%s\"}", slot,
                   (turingPiHandler.getPower(slot)) ? "true" : "false",
                   turingPiHandler.getState(slot),
                   tpiStatusStr[turingPiHandler.getState(slot)].c_str());

    if( slot < 7 )
    {
      buffer[idx++] = ',';
      buffer[idx] = 0;
    }
  }

  sprintf(buffer + idx, "], \"rtc\":\"%s\"}", turingPiHandler.getDateTime() );

  server.send(200, "application/json", buffer);
}
