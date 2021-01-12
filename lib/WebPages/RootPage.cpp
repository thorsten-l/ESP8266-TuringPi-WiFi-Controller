#include <App.hpp>
#include "WebPages.h"

static char titleBuffer[100];

const char ROOT_STYLE[] PROGMEM =
    "<style>\n"
      "pre { font-size: 0.8em; }\n"
      ".pure-button { width: 5em; margin-right: 1em;}\n"
      ".sid { margin-right: 1em; font-size: 1.2em; }\n"
    "</style>\n";

const char SLOT_STATES_SCRIPT[] PROGMEM = 
    "<script>\n"
      "function setPowerState( slot, state )\n"
      "{\n"
        "var apiUrl = \"/set?slot=\" + slot + \"&power_on=\" + state;\n"
        "console.log( apiUrl );\n"
        "fetch( apiUrl ).then(function(response) {\n"
          "return response.text().then(function(text) {\n"
            "console.log(text);\n"
            "getSlotStates();\n"
          "});\n"
        "});\n"
      "}\n"

      "function getSlotStates() {\n"
        "var e = document.getElementById(\"slotstates\");\n"
        "var r = document.getElementById(\"rtc\");\n"

        "fetch( \"/state\").then((resp) => resp.json()).then(function(data) {\n"
          "var html=\"\";\n"
          "var i;\n"
          "for ( i = 0; i < 7; i++) {\n"
            "var state = data.state[i];\n"
            "html += \"<p>\";\n"
            "html += \"<span class='sid'>\" + state.slot + \"</span>\";\n"
            "html += \"<button class='pure-button \" + ((state.power_on) ? \"button-on\" : \"\") \n"
              " + \"' onclick='setPowerState(\" + state.slot + \",true)'>ON</button>\";\n"
            "html += \"<button class='pure-button \" + ((state.power_on) ? \"\" : \"button-off\") \n"
              "+ \"' onclick='setPowerState(\" + state.slot + \",false)'>OFF</button>\";\n"
            "html += \"<span class='sid'>\" + state.status_str + \"</span>\";\n"
            "html += \"</p>\";\n"
          "}\n"
          "e.innerHTML=html;\n"
          "html=\"<span class='sid'>\" + data.rtc + \" (UTC)</span>\";\n"
          "r.innerHTML=html;\n"
        "})\n"
      "};\n"

      "setInterval(getSlotStates,60000);\n"
      "document.onload=getSlotStates();\n"
    "</script>\n";

void handleRootPage()
{
  sprintf(titleBuffer, APP_NAME " - %s", appcfg.ota_hostname );
  sendHeader(titleBuffer, false, ROOT_STYLE);

  sendPrint("<div class='pure-form pure-form-aligned'><fieldset>");

  sendLegend("Slots");
  sendPrint("<div id='slotstates'></div>");
  sendLegend("Real Time Clock");
  sendPrint("<div id='rtc'></div>");
  sendPrint("</fieldset></div>\n");

  sendPrint(SLOT_STATES_SCRIPT);

  sendFooter();
}
