#include "led_ctrl.h"
#include "params.h"
#include <FastLED.h>
#include <ArduinoJson.h>


void prc_led_ctrl_0000(void);

CRGB leds[NUM_LEDS];
uint8_t leds_buf[3][NUM_LEDS];

void led_ctrl_init(void) {
  memset(leds_buf, 0, sizeof(leds_buf));
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void led_ctrl_proces(void) {

  switch (params.led_mode) {

  case 0:
    prc_led_ctrl_0000();
    break;
  default:
    break;
  }
}




int led_ctrl_parse_json_msg_id_0001(const char *json_msg, char *resp, int save_me){

  //var send_str = '{"type":"led_ctrl","id":"1","save":"no","cnt":"0000","r":"0000","g":"0000","b":"0000","br":"0000"}';

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json_msg); // Deserialize the JSON document

  const char *r_str = doc["r"];
  const char *g_str = doc["g"];
  const char *b_str = doc["b"];
  const char *br_str = doc["br"];

  int r=0, g=0, b=0, br=0;

  if( r_str!= NULL){
    r = atoi(r_str);
    if(r < 0 || r > 255){
      r = 0;
    }
  }
  if( g_str!= NULL){
    g = atoi(g_str);
    if(g < 0 || g > 255){
      g = 0;
    }
  }
    if( b_str!= NULL){
    b = atoi(b_str);
    if(b < 0 || b > 255){
      b = 0;
    }
  }
  if(br_str != NULL){
    br = atoi(br_str);
    if( br < 0 || br > 255){
      br = 128;
    }
  }

  r = (r*br)/255;
  g = (g*br)/255;
  b = (b*br)/255;

  led_ctrl_0001(r,g,b);

  sprintf(resp, " r: %d g: %d b: %d br: %d ", r,g,b,br);

  return 0;
}

int led_ctrl_parse_json_msg(const char *json_msg, char *resp){

  *resp = 0;
  int need_save = 0;

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json_msg); // Deserialize the JSON document

  // Test if parsing succeeds.
  if (error) {
    sprintf(resp, "parsing error!");
    return -1;
  }

  const char *msg_type = doc["type"];

  if( strstr(msg_type, "led_ctrl") == NULL ){
    return -2;
  }


  const char *ctrl_id = doc["id"];
  int id = atoi(ctrl_id);
  if( id < 0  || id > 100){
    return -3;
  }


  int res = 0;
  switch(id){

    case 1:
      res = led_ctrl_parse_json_msg_id_0001(json_msg, resp, need_save);
      params.led_mode = 1;
      break;

    default:
      break;
  }

  if(need_save){
    //save gloabal parameters
    write_gen_params(&params);
  }

  




  return res;
}




void led_ctrl_update(void) {

  for (int i = 0; i < NUM_LEDS; i++) {
    float r, g, b;
    r = LED_FACTORY_BRIGHTNESS_COEF * leds_buf[0][i];
    g = LED_FACTORY_BRIGHTNESS_COEF * leds_buf[1][i];
    b = LED_FACTORY_BRIGHTNESS_COEF * leds_buf[2][i];
    leds[i] = CRGB((int)r, (int)g, (int)b);
  }

  FastLED.show();
}

void prc_led_ctrl_0000(void) {

  static int r, g, b, state;
  int r_adder = 5, g_adder = 5, b_adder = 5;

  switch (state) {

  case 0:
    r += r_adder;
    if (r > 255) {
      r = 255;
      state = 1;
    }
    break;
  case 1:
    r -= r_adder;
    if (r < 0) {
      r = 0;
      state = 2;
    }
    break;
  case 2:
    g += g_adder;
    if (g > 255) {
      g = 255;
      state = 3;
    }
    break;
  case 3:
    g -= g_adder;
    if (g < 0) {
      g = 0;
      state = 4;
    }
    break;

  case 4:
    b += b_adder;
    if (b > 255) {
      b = 255;
      state = 5;
    }
    break;

  case 5:
    b -= b_adder;
    if (b < 0) {
      b = 0;
      state = 0;
    }
  default:
    break;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds_buf[0][i] = r;
    leds_buf[1][i] = g;
    leds_buf[2][i] = b;
  }

  led_ctrl_update();
}

void led_ctr_set_all(int r, int g, int b) {

  for (int i = 0; i < NUM_LEDS; i++) {
    leds_buf[0][i] = r;
    leds_buf[1][i] = g;
    leds_buf[2][i] = b;
  }

  led_ctrl_update();
}

void led_ctrl_0001(int r, int g, int b) {
  led_ctr_set_all(r, g, b);
}




void space() { 
  delay(1200); 
  } // space between words

void dot() {
  led_ctr_set_all(100, 100, 100);
  delay(300);
  led_ctr_set_all(0, 0, 0);
  delay(300);
} // the dot this code make the led on for 300 than off for 300

void dash() {
  led_ctr_set_all(100, 100, 100);
  delay(900);
  led_ctr_set_all(0, 0, 0);
  delay(300);
} // the dash this code make the led on for 900 than off for 300

void shortspace() { delay(600); } // space between letters

// fonctions for the letters and the numbers
void lA() {
  dot();
  dash();
  shortspace();
} // letter A in morse code!
void lB() {
  dash();
  dot();
  dot();
  dot();
  shortspace();
} // same for B
void lC() {
  dash();
  dot();
  dash();
  dot();
  shortspace();
}
void lD() {
  dash();
  dot();
  dot();
  shortspace();
}
void lE() {
  dot();
  shortspace();
}
void lF() {
  dot();
  dot();
  dash();
  dot();
  shortspace();
}
void lG() {
  dash();
  dash();
  dot();
  shortspace();
}
void lH() {
  dot();
  dot();
  dot();
  dot();
  shortspace();
}
void lI() {
  dot();
  dot();
  shortspace();
}
void lJ() {
  dot();
  dash();
  dash();
  dash();
  shortspace();
}
void lK() {
  dash();
  dot();
  dash();
  shortspace();
}
void lL() {
  dot();
  dash();
  dot();
  dot();
  shortspace();
}
void lM() {
  dash();
  dash();
  shortspace();
}
void lN() {
  dash();
  dot();
  shortspace();
}
void lO() {
  dash();
  dash();
  dash();
  shortspace();
}
void lP() {
  dot();
  dash();
  dash();
  dot();
  shortspace();
}
void lQ() {
  dash();
  dash();
  dot();
  dash();
  shortspace();
}
void lR() {
  dot();
  dash();
  dot();
  shortspace();
}
void lS() {
  dot();
  dot();
  dot();
  shortspace();
}
void lT() {
  dash();
  shortspace();
}
void lU() {
  dot();
  dot();
  dash();
  shortspace();
}
void lV() {
  dot();
  dot();
  dot();
  dash();
  shortspace();
}
void lW() {
  dot();
  dash();
  dash();
  shortspace();
}
void lX() {
  dash();
  dot();
  dot();
  dash();
  shortspace();
}
void lY() {
  dash();
  dot();
  dash();
  dash();
  shortspace();
}
void lZ() {
  dash();
  dash();
  dot();
  dot();
  shortspace();
}
void n1() {
  dot();
  dash();
  dash();
  dash();
  dash();
  shortspace();
} // number 1 in morse code
void n2() {
  dot();
  dot();
  dash();
  dash();
  dash();
  shortspace();
}
void n3() {
  dot();
  dot();
  dot();
  dash();
  dash();
  shortspace();
}
void n4() {
  dot();
  dot();
  dot();
  dot();
  dash();
  shortspace();
}
void n5() {
  dot();
  dot();
  dot();
  dot();
  dot();
  shortspace();
}
void n6() {
  dash();
  dot();
  dot();
  dot();
  dot();
  shortspace();
}
void n7() {
  dash();
  dash();
  dot();
  dot();
  dot();
  shortspace();
}
void n8() {
  dash();
  dash();
  dash();
  dot();
  dot();
  shortspace();
}
void n9() {
  dash();
  dash();
  dash();
  dash();
  dot();
  shortspace();
}
void n0() {
  dash();
  dash();
  dash();
  dash();
  dash();
  shortspace();
}

void led_ctrl_morse_char(char input) {

  if (input == 'a' || input == 'A') {
    lA();
  } // if the input is a or A go to function lA
  if (input == 'b' || input == 'B') {
    lB();
  } // same but with b letter
  if (input == 'c' || input == 'C') {
    lC();
  }
  if (input == 'd' || input == 'D') {
    lD();
  }
  if (input == 'e' || input == 'E') {
    lE();
  }
  if (input == 'f' || input == 'F') {
    lF();
  }
  if (input == 'g' || input == 'G') {
    lG();
  }
  if (input == 'h' || input == 'H') {
    lH();
  }
  if (input == 'i' || input == 'I') {
    lI();
  }
  if (input == 'j' || input == 'J') {
    lJ();
  }
  if (input == 'k' || input == 'K') {
    lK();
  }
  if (input == 'l' || input == 'L') {
    lL();
  }
  if (input == 'm' || input == 'M') {
    lM();
  }
  if (input == 'n' || input == 'N') {
    lN();
  }
  if (input == 'o' || input == 'O') {
    lO();
  }
  if (input == 'p' || input == 'P') {
    lP();
  }
  if (input == 'q' || input == 'Q') {
    lQ();
  }
  if (input == 'r' || input == 'R') {
    lR();
  }
  if (input == 's' || input == 'S') {
    lS();
  }
  if (input == 't' || input == 'T') {
    lT();
  }
  if (input == 'u' || input == 'U') {
    lU();
  }
  if (input == 'v' || input == 'V') {
    lV();
  }
  if (input == 'w' || input == 'W') {
    lW();
  }
  if (input == 'x' || input == 'X') {
    lX();
  }
  if (input == 'y' || input == 'Y') {
    lY();
  }
  if (input == 'z' || input == 'Z') {
    lZ();
  }
  if (input == '1') {
    n1();
  } // the numbers
  if (input == '2') {
    n2();
  }
  if (input == '3') {
    n3();
  }
  if (input == '4') {
    n4();
  }
  if (input == '5') {
    n5();
  }
  if (input == '6') {
    n6();
  }
  if (input == '7') {
    n7();
  }
  if (input == '8') {
    n8();
  }
  if (input == '9') {
    n9();
  }
  if (input == '0') {
    n0();
  }
  if (input == ' ') {
    space();
  } // the space
}

void led_ctrl_morse_msg(const char *msg) {

  for (size_t i = 0; i < strlen(msg); i++) {
    led_ctrl_morse_char(msg[i]);
  }
}
