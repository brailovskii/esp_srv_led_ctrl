/*
 * Hello world web server
 * circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>


#define LED_PIN     8
#define NUM_LEDS    16
#define LED_FACTORY_BRIGHTNESS_COEF 1.0
CRGB leds[NUM_LEDS];


//SSID and Password to your ESP Access Point
const char* ssid = "ESPWebServer";
const char* password = "12345678";

ESP8266WebServer server(80); //Server on port 80

#define LED 2  //On board LED

const char MAIN_page[] = R"=====(


<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>
.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 25px;
  background: #d3d3e3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  background: #000000;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}
</style>

<script>
function fun_range_onChange(range_id){

	valueR = document.getElementById("range_R").value;
    valueG = document.getElementById("range_G").value;
    valueB = document.getElementById("range_B").value;
    
    valueBrightness = document.getElementById("range_Brightness").value;
    
    state_str = "R=" + valueR;
  
    state_str += "&G=" + valueG;
    state_str += "&B=" + valueB;
    state_str += "&Brightness=" + valueBrightness;
    
	//alert("Range changed " + state_str  );
    
  	var xhttp = new XMLHttpRequest();
  	xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("response_text").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "led_ctrl_0001?" + state_str, true);
  xhttp.send();

}

</script>
</head>
<body>

<h1>Custom Range Slider</h1>

<div style="width:100%;" >
  <p>Red:</p>
  <input type="range" min="0" max="100" value="50" id="range_R" class="slider" style="background: #cc0000;" onchange = "fun_range_onChange('R')">
  
   <p>Green:</p>
  <input type="range" min="0" max="100" value="50" id="range_G" class="slider" style="background: #00cc00;" onchange = "fun_range_onChange('G')">
  
   <p>Blue:</p>
  <input type="range" min="0" max="100" value="50" id="range_B" class="slider" style="background: #0000cc;" onchange = "fun_range_onChange('B')">
  
  <p>Brightness:</p>
  <input type="range" min="0" max="100" value="50" id="range_Brightness" class="slider" style="background: #cccccc;" onchange = "fun_range_onChange('Brigtness')">
  
  <p></p>
  <h5 id="response_text"> Response </h5>
</div>

</body>
</html>


)=====";





//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot11() {
  server.send(200, "text/plain", "hello from esp8266!");
}




void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleADC() {
 int a = analogRead(A0);
 String adcValue = String(a);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleLED() {
 String ledState = "OFF";
 String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
 Serial.println(t_state);
 if(t_state == "1")
 {
  digitalWrite(LED,LOW); //LED ON
  ledState = "ON"; //Feedback parameter
 }
 else
 {
  digitalWrite(LED,HIGH); //LED OFF
  ledState = "OFF"; //Feedback parameter  
 }
 
 server.send(200, "text/plane", ledState); //Send web page
}


void led_ctrl_0001(int r, int g, int b, int brightness){

  float r_f = r * LED_FACTORY_BRIGHTNESS_COEF * (float)brightness * 255.0 / 10000.0;
  float g_f = g * LED_FACTORY_BRIGHTNESS_COEF * (float)brightness * 255.0 / 10000.0;
  float b_f = b * LED_FACTORY_BRIGHTNESS_COEF * (float)brightness * 255.0 / 10000.0;

  r = (int)r_f;
  g = (int)g_f;
  b = (int)b_f;

  for( int  i = 0; i < NUM_LEDS; i++ ){
    leds[i] = CRGB(r,g,b);
  }

  FastLED.show();
}

void handle_led_ctrl_0001(void){

  static int cnt=0;

  cnt++;
  String resp = String(cnt) + " ";

  String state_r = server.arg("R");
  String state_g = server.arg("G");
  String state_b = server.arg("B");
  String state_brightness = server.arg("Brightness");

  resp += "R:" + state_r + " ";
  resp += "S:" + state_g + " ";
  resp += "T:" + state_b + " ";
  resp += "Brigntness:" + state_brightness + " ";

  server.send(200, "text/plane", resp ); //Send web page

  led_ctrl_0001( state_r.toInt(), state_g.toInt(), state_b.toInt(), state_brightness.toInt() );

}



//===============================================================
//                  SETUP
//===============================================================
void setup(void){
  Serial.begin(115200);
  Serial.println("\nStarting...");


  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);


  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
 
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/setLED", handleLED);
  server.on("/readADC", handleADC);

  server.on("/led_ctrl_0001", handle_led_ctrl_0001);

  server.begin();                  //Start server
  Serial.println("HTTP server started");

}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
