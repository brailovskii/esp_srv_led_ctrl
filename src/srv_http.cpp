#include <string.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>


#include "html/html_led_ctrl_0001.h"
#include "html/html_led_ctrl_0002.h"
#include "html/html_main_page.h"
#include "html/colour_ctrl_list.h"


#include "config.h"

#include "msg_parser.h"

#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include "led_ctrl.h"
#include "params.h"

static const char *myHostname = "esp";

ESP8266WebServer server(80); //Server on port 80

/** Is this an IP? */
boolean isIp(String str)
{
  for (size_t i = 0; i < str.length(); i++)
  {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9'))
    {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip)
{
  String res = "";
  for (int i = 0; i < 3; i++)
  {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal()
{
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local"))
  {
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop();             // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void handleRoot()
{
  if (captivePortal())
  { // If caprive portal redirect instead of displaying the page.
    return;
  }

  String s = html_page_main;             //Read HTML contents

  server.send(200, "text/html", s); //Send web page
}


void handle_settings(void){


  server.send(200, "text/html", "Here will be settings"); //Send web page
}

void handle_colour_ctrl_list(void){


  String s = html_page_colour_ctrl_list;

  server.send(200, "text/html", s); //Send web page

}

void handle_html_led_ctrl_0001(void){

  String s = html_page_led_ctrl_0001;             //Read HTML contents

  String r(params.led_ctrl_0001.r);
  String g(params.led_ctrl_0001.g);
  String b(params.led_ctrl_0001.b);
  String br(params.led_ctrl_0001.br);
  s.replace("value=\"5555\" id=\"range_R\"", "value=\"" + r + "\" id=\"range_R\"");
  s.replace("value=\"5555\" id=\"range_G\"", "value=\"" + g + "\" id=\"range_G\"");
  s.replace("value=\"5555\" id=\"range_B\"", "value=\"" + b + "\" id=\"range_B\"");
  s.replace("value=\"5555\" id=\"range_Brightness\"", "value=\"" + br + "\" id=\"range_Brightness\"");


  server.send(200, "text/html", s); //Send web page

}
void handle_html_led_ctrl_0002(void){


  String s = html_page_led_ctrl_0002;             //Read HTML contents

  // String r(params.led_ctrl_0001.r);
  // String g(params.led_ctrl_0001.g);
  // String b(params.led_ctrl_0001.b);
  // String br(params.led_ctrl_0001.br);
  // s.replace("value=\"5555\" id=\"range_R\"", "value=\"" + r + "\" id=\"range_R\"");
  // s.replace("value=\"5555\" id=\"range_G\"", "value=\"" + g + "\" id=\"range_G\"");
  // s.replace("value=\"5555\" id=\"range_B\"", "value=\"" + b + "\" id=\"range_B\"");
  // s.replace("value=\"5555\" id=\"range_Brightness\"", "value=\"" + br + "\" id=\"range_Brightness\"");


  server.send(200, "text/html", s); //Send web page

}

void handle_led_ctrl_0001(void)
{
  static int cnt = 0;
  char response[128];

  cnt++;  
  String msg = server.arg("json_msg");
  msg_parser_parse(msg.c_str(), response);

  String s(response);
  String resp = String(cnt) + " " + s;

  server.send(200, "text/plane", resp); //Send web page
}

void handle_led_ctrl_0002(void)
{

  static int cnt = 0;

  cnt++;

  char response[128];

  String msg = server.arg("json_msg");
  msg_parser_parse(msg.c_str(), response);

  String s(response);
  String resp = String(cnt) + " " + s;

  server.send(200, "text/plane", resp); //Send web page
}






void srv_http_init(void)
{

  server.on("/", handleRoot); //Which routine to handle at root location

  server.on("/colour_ctrl.html", handle_colour_ctrl_list);
  server.on("/settings.html", handle_settings);


  //these html pages are responsible for showing html content
  server.on("/html_led_ctrl_0001.html", handle_html_led_ctrl_0001);
  server.on("/html_led_ctrl_0002.html", handle_html_led_ctrl_0002);

  //these addresses are responsible for AJAX data process
  server.on("/led_ctrl_0001", handle_led_ctrl_0001);
  server.on("/led_ctrl_0002", handle_led_ctrl_0002);


  server.on("/generate_204", handleRoot); //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);       //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.

  server.begin(); //Start server
  Serial.println("HTTP server started");
}


void srv_http_process(void)
{
  server.handleClient(); //Handle client requests
}