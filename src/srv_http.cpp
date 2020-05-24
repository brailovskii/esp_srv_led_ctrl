#include <string.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>

#include "html_led_ctrl_0001_slider.h"
#include "config.h"



#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include "led_ctrl.h"

static const char *myHostname = "esp";


ESP8266WebServer server(80); //Server on port 80






/** Is this an IP? */
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}


/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}


void handleRoot()
{
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }

  String s = MAIN_page;             //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}






void handle_led_ctrl_0001(void)
{

  static int cnt = 0;

  cnt++;

  char response[128];

  String msg = server.arg("json_msg");
  led_ctrl_parse_json_msg(msg.c_str(), response);

  String s(response);
  String resp = String(cnt) + " " + s;

  server.send(200, "text/plane", resp); //Send web page

}






void srv_http_init(void){

  server.on("/", handleRoot); //Which routine to handle at root location

  server.on("/led_ctrl_0001", handle_led_ctrl_0001);

  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.

  server.begin(); //Start server
  Serial.println("HTTP server started");

}

void srv_http_process(void){



  server.handleClient(); //Handle client requests

}