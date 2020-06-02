/*
 * Hello world web server
 * circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>


#include "led_ctrl.h"
#include "srv_http.h"

#include "config.h"

#include "Timer.h"
 
#include "params.h"


Timer t;


// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

/* Soft AP network parameters */
IPAddress apIP(8, 8, 8, 8);
IPAddress netMsk(255, 255, 255, 0);


int wifi_sta_conn = 0; 



void wifi_sta_100ms_cb()
{
  if (WiFi.status() == WL_CONNECTED && wifi_sta_conn == 0) {

    Serial.println("STA connected");

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

    Serial.print("STA IP: ");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin(params.host_name)) {
      Serial.println("Error setting up MDNS responder!");
    } else {
      Serial.println("mDNS responder started");
      // Add service to MDNS-SD
      MDNS.addService("http", "tcp", 80);
    }

    wifi_sta_conn = 1;
   }
   else if( WiFi.status() != WL_CONNECTED && wifi_sta_conn == 1 ){

     Serial.println("STA disconnected");

     wifi_sta_conn = 0;
   }
}






//===============================================================
//                  SETUP
//===============================================================
void setup(void)
{
  Serial.begin(115200);
  Serial.println("\nStarting...");

  /*read settings, if settings are broken set default parameters*/
  params_init();


  WiFi.mode(WIFI_AP_STA);     


  Serial.println("Configuring access point...");
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(params.ap_ssid, params.ap_pwd);
  delay(50);


  Serial.println("Configuring Station mode...");
  Serial.println(&params.sta_ssid[0][0]);
  Serial.println(&params.sta_pwd[0][0]);
  WiFi.begin(&params.sta_ssid[0][0], &params.sta_pwd[0][0]);


  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /*initialize server*/
  srv_http_init();

  //initialize leds
  led_ctrl_init();

  t.every(1000, wifi_sta_100ms_cb);
  t.every(LED_CTRL_CALL_PERIOD, led_ctrl_proces);
}

//===============================================================
//                     LOOP
//===============================================================
void loop(void)
{

  t.update();

  if (WiFi.status() == WL_CONNECTED) {
      //MDNS.update();
  }


  //DNS
  dnsServer.processNextRequest();
  
  srv_http_process();
}




