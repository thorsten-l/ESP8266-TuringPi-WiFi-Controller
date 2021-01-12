#include <App.hpp>
#include <Util.hpp>
#include <TuringPiHandler.hpp>
#include "WebPages.h"

void handleSetAPI()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  sendHeaderNoCache();
  sendAuthentication();
  
  bool requestFailed = true;

  if ( server.args() == 2
    && server.argName(0) == "slot" 
    && server.argName(1) == "power_on" )
  {
    int slot = atoi(server.arg(0).c_str());

    if ( slot >=1 && slot <= 7 )
    {
      bool validValue = false;
      bool power_on = false;

      String p = server.arg(1);
      if ( p == "0" || p == "false" ) 
      {
        validValue = true;
      }
      if ( p == "1" || p == "true" ) 
      {
        power_on = true;
        validValue = true;
      }

      if ( validValue )
      {
        sprintf( buffer, "OK: slot=%d power_on=%s", slot, (power_on) ? "true" : "false" );
        turingPiHandler.setPower( slot, power_on );
        requestFailed = false;
      }
    }
  }
  
  if( requestFailed )
  {
    server.send ( 500, "text/plain", "ERROR: Wrong or missing parameters <url>?slot=[1..7]&power_on=[1|0]");    
  }
  else
  {
    server.send ( 200, "text/plain", buffer );
  }
  
  server.client().stop();
}
