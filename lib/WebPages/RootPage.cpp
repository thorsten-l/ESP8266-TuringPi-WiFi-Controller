#include <App.hpp>
#include "WebPages.h"

static char titleBuffer[100];

const char ROOT_STYLE[] PROGMEM =
    "<style>\n"
      "pre {font-size: 0.8em;}\n"
      ".pure-button {width:5em;margin-right:1em;}\n"
      ".sid {margin-right:1em;font-size:1.2em;}\n"
      ".svgicon {display:inline-block;width:2em;height:2em;vertical-align:middle;}\n"
      ".running {background:url('data:image/svg+xml;base64,PHN2ZyBiYXNlUHJvZmlsZT0idGlueSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB2aWV3Qm94PSIwIDAgMjYgMjYiIG92ZXJmbG93PSJ2aXNpYmxlIj48cGF0aCBmaWxsPSIjMDMwMTA0IiBkPSJNMjIuOSAxNC4ydi0yLjVsLTMtLjVjLS4yLS43LS41LTEuNC0uOC0ybDEuNy0yLjVMMTkgNC45bC0yLjQgMS43Yy0uNi0uNC0xLjMtLjYtMi0uOGwtLjUtMi45aC0yLjVMMTEuMyA2Yy0uNy4yLTEuNC40LTIgLjhMNi45IDUuMSA1LjEgNi44bDEuNyAyLjRjLS40LjYtLjYgMS4zLS44IDJsLTIuOS41djIuNWwyLjkuNWMuMi43LjUgMS40LjggMmwtMS43IDIuNCAxLjggMS44IDIuNC0xLjdjLjYuNCAxLjMuNiAyIC44bC41IDIuOWgyLjVsLjUtMi45Yy43LS4yIDEuNC0uNSAyLS44bDIuNSAxLjcgMS44LTEuOC0xLjctMi40Yy40LS42LjYtMS4zLjgtMmwyLjctLjV6TTEzIDE2LjVjLTEuOSAwLTMuNS0xLjYtMy41LTMuNXMxLjYtMy41IDMuNS0zLjUgMy41IDEuNiAzLjUgMy41LTEuNiAzLjUtMy41IDMuNXoiLz48cGF0aCBmaWxsPSJub25lIiBzdHJva2U9IiMxRDFEMUIiIHN0cm9rZS13aWR0aD0iMS4yIiBzdHJva2UtbWl0ZXJsaW1pdD0iMTAiIGQ9Ik0yNC4xIDE1LjljLTEuMyA1LTUuOCA4LjgtMTEuMSA4LjgtNi4zIDAtMTEuNC01LjMtMTEuNC0xMS43UzYuNyAxLjMgMTMgMS4zIDI0LjQgNi42IDI0LjQgMTMiLz48cGF0aCBmaWxsPSIjMUQxRDFCIiBkPSJNMjEuOSAxNi4ybDIuNS0zLjIgMS42IDMuOHoiLz48L3N2Zz4=');}\n"
      ".empty {background: url('data:image/svg+xml;base64,PHN2ZyBiYXNlUHJvZmlsZT0idGlueSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB2aWV3Qm94PSIwIDAgMjYgMjYiIG92ZXJmbG93PSJ2aXNpYmxlIj48cGF0aCBmaWxsPSIjMDMwMTA0IiBkPSJNMjQgMTl2NGMwIC42LS40IDEtMSAxSDNjLS42IDAtMS0uNC0xLTF2LTRIMHY0YzAgMS43IDEuMyAzIDMgM2gyMGMxLjcgMCAzLTEuMyAzLTN2LTRoLTJ6Ii8+PC9zdmc+');}\n"
      ".installed {background: url('data:image/svg+xml;base64,PHN2ZyBiYXNlUHJvZmlsZT0idGlueSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB2aWV3Qm94PSIwIDAgMjYgMjYiIG92ZXJmbG93PSJ2aXNpYmxlIj48cGF0aCBmaWxsPSIjMDMwMTA0IiBkPSJNMjIuOSAxMS4yVjguN2wtMy0uNWMtLjItLjctLjUtMS40LS44LTJsMS43LTIuNUwxOSAxLjlsLTIuNCAxLjdjLS42LS40LTEuMy0uNi0yLS44TDE0LjIgMGgtMi41bC0uNCAzYy0uNy4yLTEuNC40LTIgLjhMNi45IDIuMSA1LjEgMy44bDEuNyAyLjRjLS40LjYtLjYgMS4zLS44IDJsLTIuOS41djIuNWwyLjkuNWMuMi43LjUgMS40LjggMmwtMS43IDIuNCAxLjggMS44IDIuNC0xLjdjLjYuNCAxLjMuNiAyIC44bC41IDIuOWgyLjVsLjUtMi45Yy43LS4yIDEuNC0uNSAyLS44bDIuNSAxLjcgMS44LTEuOC0xLjctMi40Yy40LS42LjYtMS4zLjgtMmwyLjctLjV6TTEzIDEzLjRjLTEuOSAwLTMuNS0xLjYtMy41LTMuNXMxLjYtMy41IDMuNS0zLjUgMy41IDEuNiAzLjUgMy41YzAgMi0xLjYgMy41LTMuNSAzLjV6Ii8+PHBhdGggZmlsbD0iIzAzMDEwNCIgZD0iTTI0IDE5djRjMCAuNi0uNCAxLTEgMUgzYy0uNiAwLTEtLjQtMS0xdi00SDB2NGMwIDEuNyAxLjMgMyAzIDNoMjBjMS43IDAgMy0xLjMgMy0zdi00aC0yeiIvPjwvc3ZnPg==');}\n"
      ".unknown {background: url('data:image/svg+xml;base64,PHN2ZyBiYXNlUHJvZmlsZT0idGlueSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB2aWV3Qm94PSIwIDAgMjYgMjYiIG92ZXJmbG93PSJ2aXNpYmxlIj48cGF0aCBmaWxsPSJub25lIiBzdHJva2U9IiMwMDAiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLW1pdGVybGltaXQ9IjEwIiBkPSJNNS4zIDEzLjNoMTUuNCIvPjwvc3ZnPg==');}\n"
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

            "var iconclass = \"unknown\";\n"
            "if ( state.status === 1 ) iconclass = \"installed\";\n"
            "if ( state.status === 2 ) iconclass = \"empty\";\n"
            "if ( state.ping_total_recv > 0 ) iconclass = \"running\";\n"
            "html += \"<span class='svgicon \" + iconclass + \"'></span>\";\n"
            "var ls = new Date(state.ping_last_seen*1000).toLocaleString();\n"
            "if ( state.ping_last_seen === 0 ) { ls = \"\" }\n"
            "html += \"<span class='sid' style='margin-left:1em'>\" + ls + \"</span>\";\n"

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
