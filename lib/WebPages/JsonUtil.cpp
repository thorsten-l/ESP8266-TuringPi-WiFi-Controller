#include <App.hpp>
#include <WifiHandler.hpp>
#include <ESP8266WebServer.h>
#include <TuringPiHandler.hpp>
#include <Util.hpp>
#include "WebPages.h"

extern ESP8266WebServer server;

void buildJsonMessage()
{
  turingPiHandler.readRegisters();

  int idx = sprintf(buffer, "{\"state\":[");

  for (int slot = 1; slot < 8; slot++)
  {
    idx += sprintf(buffer + idx,
                   "{\"slot\":%d,\"power_on\":%s,"
                   "\"status\":%d,"
                   "\"ping_last_recv\":%d,\"ping_last_seen\":%lu"
                   "}",
                   slot,
                   (turingPiHandler.getPower(slot)) ? "true" : "false",
                   turingPiHandler.getState(slot),
                   turingPiHandler.getPingLastRecv(slot),
                   turingPiHandler.getPingLastSeen(slot));

    if (slot < 7)
    {
      buffer[idx++] = ',';
      buffer[idx] = 0;
    }
  }

  sprintf(buffer + idx, "], \"rtc\":\"%s\", \"rtc_now\":%ld}",
          turingPiHandler.getDateTime(), turingPiHandler.getTime());
}

void handleJsonStatusState()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  sendHeaderNoCache();
  buildJsonMessage();
  server.send(200, "application/json", buffer);
}
