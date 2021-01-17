#include "App.hpp"
#include "Util.hpp"
#include <TuringPiHandler.hpp>
#include "TelnetStream.h"

static String tpiStatusStr[] = {"-", "installed", "emtpy"};
static bool escape;

void testFn()
{
  TelnetStream.println("Test Function");
}

extern const char *getJsonStatus(WiFiClient *client);

void TelnetStreamClass::printHelp()
{
  TelnetStream.println("\n\nHELP page\n"
                       "help : this help\n"
                       "info : Turing Pi slot information\n"
                       "set_slot [1..7] power_on [0|1] : turn a slot power on/off \n"
                       "banner : print banner\n"
                       "uptime : uptime\n"
                       "json : json status\n"
                       "quit,exit : quit / exit this session\n");
}

void TelnetStreamClass::printBanner()
{
  appShowHeader(TelnetStream);
  TelnetStream.printf("Uptime : %s\n", appUptime());
  TelnetStream.printf("Client IP-Address = %s\n\n",
                      client.remoteIP().toString().c_str());
}

TelnetStreamClass::TelnetStreamClass(uint16_t port) : server(port)
{
}

void TelnetStreamClass::begin()
{
  escape = false;
  commandIndex = 0;
  commandLine[0] = 0;
  isConnected = false;
  server.begin();
  client = server.available();
  LOG0("Telnet server startet\n");
}

void TelnetStreamClass::handle()
{
  TelnetStream.available();

  if (client && client.connected())
  {
    if (isConnected == false)
    {
      isConnected = true;
      printBanner();
    }

    if (TelnetStream.available())
    {
      int c = TelnetStream.read();

      if (escape == false && (c == 10 || c == 13 || (c > 31 && c < 127)))
      {
        if ((c == 10 || c == 13) && commandIndex > 0)
        {
          // logMessage("command = <%s>", commandLine);

          int cmd = commandLine[0] & 0x00df;

          switch (cmd)
          {
          case 'B':
            printBanner();
            break;

          case 'U':
            TelnetStream.printf("\nTime      : %s\n", appDateTime());
            TelnetStream.printf("Uptime    : %s\n", appUptime());
            TelnetStream.printf("Free Heap : %u\n", ESP.getFreeHeap());
            TelnetStream.println();
            break;

          case 'J':
            TelnetStream.println(getJsonStatus(&client));
            break;

          case 'H':
            printHelp();
            break;

          case 'I':
          {
            TelnetStream.printf("RTC: (%lu) %s (UTC)\n",
                                turingPiHandler.getTime(), turingPiHandler.getDateTime());
            TelnetStream.printf("NTP: (%lu) %s\n\n",
                                time(nullptr), appDateTime());
            turingPiHandler.readRegisters();
            for (int slot = 1; slot < 8; slot++)
            {
              TelnetStream.printf("slot #%d : %s : %s (%d : %s)\n", slot,
                                  (turingPiHandler.getPower(slot)) ? "ON " : "OFF",
                                  tpiStatusStr[turingPiHandler.getState(slot)].c_str(),
                                  turingPiHandler.getPingLastRecv(slot),
                                  appDateTime(turingPiHandler.getPingLastSeen(slot)));
            }
          }
          break;

          case 'S':
          {
            char command[32];
            char subcommand[32];
            int slot = -1;
            int power_on = -1;

            sscanf(commandLine, "%s %d %s %d", command, &slot, subcommand, &power_on);
            if (strcmp("set_slot", command) == 0 
            && strcmp("power_on", subcommand) == 0 
            && slot >= 1 && slot <= 7 && power_on >= 0 && power_on <= 1)
            {
              logMessage("Turning slot %d to power %s.", slot, (power_on == 1) ? "on" : "off");
              turingPiHandler.setPower(slot, power_on == 1);
            }
            else
            {
              logMessage("unknown command");
            }
          }
          break;

          case 'Q':
          case 'E':
            isConnected = false;
            stop();
            break;
          }

          commandLine[0] = 0;
          commandIndex = 0;
        }
        else
        {
          if (commandIndex < 31 && c > 31)
          {
            commandLine[commandIndex++] = c;
            commandLine[commandIndex] = 0;
          }
        }
      }

      escape = c > 127;
    }
  }
}

void TelnetStreamClass::stop()
{
  client.stop();
}

boolean TelnetStreamClass::disconnected()
{
#ifdef ESP32
  if (!server)
    return true;
#else
  if (server.status() == CLOSED)
    return true;
#endif
  if (!client)
  {
    client = server.available();
  }
  if (client)
  {
    if (client.connected())
      return false;
    client.stop();
    client = server.available();
  }
  isConnected = false;
  return true;
}

int TelnetStreamClass::read()
{
  if (disconnected())
    return -1;
  return client.read();
}

int TelnetStreamClass::available()
{
  if (disconnected())
    return 0;
  return client.available();
}

int TelnetStreamClass::peek()
{
  if (disconnected())
    return -1;
  return client.peek();
}

size_t TelnetStreamClass::write(uint8_t val)
{
  if (disconnected())
    return 1;
  return client.write(val);
}

void TelnetStreamClass::flush()
{
  if (disconnected())
    return;
  client.flush();
}

TelnetStreamClass TelnetStream(23);
