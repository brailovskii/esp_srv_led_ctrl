#include "led_ctrl.h"
#include "params.h"
#include <FastLED.h>

typedef enum
{
  MORSE_STATE_NONE,
  MORSE_STATE_DOT,
  MORSE_STATE_DOT_ON,
  MORSE_STATE_DOT_OFF,
  MORSE_STATE_DASH,
  MORSE_STATE_DASH_ON,
  MORSE_STATE_DASH_OFF,

  MORSE_STATE_SHORT_SPACE,
  MORSE_STATE_LONG_SPACE,

  MORSE_STATE_TRANSMITTING,

  MORSE_STATE_LETTER_DONE,
  MORSE_STATE_MSG_DONE
} morse_states;

void prc_led_ctrl_0000(void);
void prc_led_ctrl_0001(void);
void prc_led_ctrl_0002(void);
void prc_led_ctrl_0003(void);
void prc_led_ctrl_0007(void);

int led_ctrl_morse_char(char input);

CRGB leds[NUM_LEDS];
uint8_t leds_buf[3][NUM_LEDS];
static uint8_t morse_rgb[3] = { 255, 0, 0};
static uint8_t morse_state = MORSE_STATE_NONE;

void led_ctrl_init(void)
{

  memset(leds_buf, 0, sizeof(leds_buf));
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  randomSeed(analogRead(0));
}

void led_ctrl_proces(void)
{

  switch (params.led_mode)
  {

  case 0:
    prc_led_ctrl_0000();
    break;
  case 1:
    prc_led_ctrl_0001();
    break;
  case 2:
    prc_led_ctrl_0002();
    break;
  case 3:
    prc_led_ctrl_0003();
    break;
  case 7:
    prc_led_ctrl_0007();
    break;
  default:
    break;
  }
}

void led_ctrl_update(void)
{

  for (int i = 0; i < NUM_LEDS; i++)
  {
    float r, g, b;
    r = LED_FACTORY_BRIGHTNESS_COEF * leds_buf[0][i];
    g = LED_FACTORY_BRIGHTNESS_COEF * leds_buf[1][i];
    b = LED_FACTORY_BRIGHTNESS_COEF * leds_buf[2][i];
    leds[i] = CRGB((int)r, (int)g, (int)b);
  }

  FastLED.show();
}

void prc_led_ctrl_0000(void)
{

  static int r, g, b, state;
  int r_adder = 5, g_adder = 5, b_adder = 5;

  switch (state)
  {
  case 0:
    r += r_adder;
    if (r > 255)
    {
      r = 255;
      state = 1;
    }
    break;
  case 1:
    r -= r_adder;
    if (r < 0)
    {
      r = 0;
      state = 2;
    }
    break;
  case 2:
    g += g_adder;
    if (g > 255)
    {
      g = 255;
      state = 3;
    }
    break;
  case 3:
    g -= g_adder;
    if (g < 0)
    {
      g = 0;
      state = 4;
    }
    break;

  case 4:
    b += b_adder;
    if (b > 255)
    {
      b = 255;
      state = 5;
    }
    break;

  case 5:
    b -= b_adder;
    if (b < 0)
    {
      b = 0;
      state = 0;
    }
  default:
    break;
  }

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds_buf[0][i] = r;
    leds_buf[1][i] = g;
    leds_buf[2][i] = b;
  }

  led_ctrl_update();
}

void prc_led_ctrl_0001(void)
{

  static int cnt = 0;

  cnt++;

  //function is called periodically, so we dont have to update it every time
  if ((cnt % 2) != 0)
  {
    return;
  }

  int r, g, b;

  r = (params.led_ctrl_0001.r * params.led_ctrl_0001.br) / 255;
  g = (params.led_ctrl_0001.g * params.led_ctrl_0001.br) / 255;
  b = (params.led_ctrl_0001.b * params.led_ctrl_0001.br) / 255;

  led_ctr_set_all(r, g, b);

  led_ctrl_update();
}

void prc_led_ctrl_0002(void)
{

  //this function creates random colour
  static int cnt = 0;
  static int16_t r = 0, g = 0, b = 0; //must be more that 8bit
  static int r_goal = 0, g_goal = 0, b_goal = 0;
  static int r_dir = 1, g_dir = 1, b_dir = 1;

  uint8_t r_max = 255, g_max = 255, b_max = 255;
  uint8_t r_min = 0, g_min = 0, b_min = 0;
  int refresh_rate = 100;
  int r_step = 1, g_step = 1, b_step = 1;

  r_min = params.led_ctrl_0002.rmin;
  r_max = params.led_ctrl_0002.rmin;
  g_min = params.led_ctrl_0002.gmin;
  g_max = params.led_ctrl_0002.gmax;
  b_min = params.led_ctrl_0002.bmin;
  b_max = params.led_ctrl_0002.bmax;

  refresh_rate = params.led_ctrl_0002.col_upd_rate;

  r_step = params.led_ctrl_0002.rrate;
  g_step = params.led_ctrl_0002.grate;
  b_step = params.led_ctrl_0002.brate;

  if ((cnt % refresh_rate) == 0)
  {
    //generate new colours
    r_goal = random(r_min, r_max);
    g_goal = random(g_min, g_max);
    b_goal = random(b_min, b_max);

    if (r_goal > r)
    {
      r_dir = 1;
    }
    else if (r_goal < r)
    {
      r_dir = -1;
    }
    else
    {
      r_dir = 0;
    }

    if (g_goal > g)
    {
      g_dir = 1;
    }
    else if (g_goal < g)
    {
      g_dir = -1;
    }
    else
    {
      g_dir = 0;
    }

    if (b_goal > b)
    {
      b_dir = 1;
    }
    else if (b_goal < b)
    {
      b_dir = -1;
    }
    else
    {
      b_dir = 0;
    }

    // Serial.println("\nNew goal values: ");
    // Serial.println(r_goal);
    // Serial.println(g_goal);
    // Serial.println(b_goal);
  }

  cnt++;

  if (r_dir == 1)
  {
    r += r_step;
    if (r > r_goal)
    {
      r = r_goal;
    }
  }
  else if (r_dir == -1)
  {
    r -= r_step;
    if (r < r_goal)
    {
      r = r_goal;
    }
  }

  if (g_dir == 1)
  {
    g += g_step;
    if (g > g_goal)
    {
      g = g_goal;
    }
  }
  else if (g_dir == -1)
  {
    g -= g_step;
    if (g < g_goal)
    {
      g = g_goal;
    }
  }

  if (b_dir == 1)
  {
    b += b_step;
    if (b > b_goal)
    {
      b = b_goal;
    }
  }
  else if (b_dir == -1)
  {
    b -= b_step;
    if (b < b_goal)
    {
      b = b_goal;
    }
  }

  // Serial.println("\nRGB values: ");
  // Serial.println(r);
  // Serial.println(g);
  // Serial.println(b);

  led_ctr_set_all(r, g, b);

  led_ctrl_update();
}

void prc_led_ctrl_0003(void)
{

  led_ctrl_update();
}

/*morse code flashing*/
void prc_led_ctrl_0007(void)
{

  static int i = 0;
  static int morse_msg_state = MORSE_STATE_NONE;
  static uint32_t wait_start_time = 0;

  char *msg = params.led_ctrl_0007.msg;
  morse_rgb[0] = params.led_ctrl_0007.r;
  morse_rgb[1] = params.led_ctrl_0007.g;
  morse_rgb[2] = params.led_ctrl_0007.b;
  uint32_t wait_time_ms = params.led_ctrl_0007.puase_btw_msg;


  if (morse_msg_state == MORSE_STATE_NONE)
  {
    //start transmitting
    i = 0;
    morse_msg_state = MORSE_STATE_TRANSMITTING;
    Serial.println("Msg:");
    Serial.println(msg);
  }

  if( morse_msg_state == MORSE_STATE_TRANSMITTING ){

    if( msg[i] == 0 ){
      morse_msg_state = MORSE_STATE_MSG_DONE;
    }else{
      if( led_ctrl_morse_char(msg[i]) == 1 ){
        //letter has been transmitted
        i++;

        Serial.println("next letter:");
        Serial.println(msg[i]);
        Serial.println(msg[i], DEC);
      }
    }
  }


  if (morse_msg_state == MORSE_STATE_MSG_DONE)
  {
    Serial.println("message transmitted\n\n");

    if( wait_start_time == 0 ){
      wait_start_time = millis();
    }else{

      if( ( millis() - wait_start_time) > wait_time_ms ){
        morse_msg_state = MORSE_STATE_NONE;
        wait_start_time = 0;
      }

    }


    


  }

  led_ctrl_update();
}

void led_ctr_set_all(int r, int g, int b)
{

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds_buf[0][i] = r;
    leds_buf[1][i] = g;
    leds_buf[2][i] = b;
  }
}

void led_ctrl_0001(int r, int g, int b)
{
  led_ctr_set_all(r, g, b);
}







int dot()
{
  static uint32_t on_start_time = 0;
  static uint32_t off_start_time = 0;
  static int dot_state = MORSE_STATE_NONE;

  if (morse_state != MORSE_STATE_DOT)
  {
    return 0;
  }


  if( dot_state == MORSE_STATE_NONE ){
    dot_state = MORSE_STATE_DOT_ON;
    led_ctr_set_all(morse_rgb[0], morse_rgb[1], morse_rgb[2]);
    on_start_time = millis(); //get start time
  }


  if (dot_state == MORSE_STATE_DOT_ON)
  {
    if ((millis() - on_start_time) < 300){
      return 0;
    }
    else{
      dot_state = MORSE_STATE_DOT_OFF;
      led_ctr_set_all(0, 0, 0); //turn off light
      off_start_time = millis();
    }
  }

  if (dot_state == MORSE_STATE_DOT_OFF)
  {

    if ((millis() - off_start_time) < 300)
    {
      return 0;
    }
    else
    {
      off_start_time = 0;
      on_start_time = 0;
      dot_state = MORSE_STATE_NONE;
      return 1;
    }
  }

  return 0;
} // the dot this code make the led on for 300 than off for 300

int dash()
{

  static uint32_t on_start_time;
  static uint32_t off_start_time;
  static int dash_state = MORSE_STATE_NONE;

  if (morse_state != MORSE_STATE_DASH)
  {
    return 0;
  }

  if(dash_state == MORSE_STATE_NONE){

    dash_state = MORSE_STATE_DASH_ON;
    led_ctr_set_all(morse_rgb[0], morse_rgb[1], morse_rgb[2]);
    on_start_time = millis(); //get start time
  }

  if (dash_state == MORSE_STATE_DASH_ON)
  {
    if ((millis() - on_start_time) < 900)
    {
      return 0;
    }

          led_ctr_set_all(0, 0, 0); //turn off light
      off_start_time = millis();
      dash_state = MORSE_STATE_DASH_OFF;

  }

  if (dash_state == MORSE_STATE_DASH_OFF){


    if ((millis() - off_start_time) < 300)
    {
      return 0;
    }
    else
    {
      off_start_time = 0;
      on_start_time = 0;
      dash_state = MORSE_STATE_NONE;
      return 1;
    }
  }

  return 0;
} // the dash this code make the led on for 900 than off for 300

int shortspace()
{

  static uint32_t start_time = 0;

  if( morse_state == MORSE_STATE_SHORT_SPACE ){

    if( start_time == 0 ){
      start_time = millis();
    }

    if( (millis() - start_time) >= 600 ){
      start_time = 0;
      return 1;
    }

  }

  return 0;

} // space between letters


int longspace()
{

  static uint32_t start_time = 0;

  if( morse_state == MORSE_STATE_LONG_SPACE ){

    if( start_time == 0 ){
      start_time = millis();
    }

    if( (millis() - start_time) >= 1200 ){
      start_time = 0;
      return 1;
    }

  }

  return 0;

} // space between letters


static const char *morse_alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};
static const char *morse_num[] = {
    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9
};

static const char morse_space[] = " ";

int led_ctrl_morse_char(char input)
{

  static const char *letter = NULL;
  static int16_t i = 0;

  if (letter == NULL)
  {
    //get letter code
    if (isalpha(input))
    {
      letter = morse_alpha[toupper(input) - 'A'];
    }
    if (isdigit(input))
    {
      letter = morse_num[input - '0'];
    }
    if (input == ' ')
    {
      letter = morse_space;
    }

    i = 0;
  }

  if (letter != NULL)
  {
    //process letter code

    if (letter[i] == '.')
    {
      morse_state = MORSE_STATE_DOT;
      if (dot() == 1)
      {
        i++;
      }
    }

    if (letter[i] == '-')
    {
      morse_state = MORSE_STATE_DASH;
      if (dash() == 1)
      {
        i++;
      }
    }

    if( letter[i] == ' ' ){
      morse_state = MORSE_STATE_LONG_SPACE;
      if(longspace() == 1){
        //letter space is transmitted
        letter = NULL;
        i = 0;
        return 1;
      }


    }

    if( letter[i] == '\0'){
      morse_state = MORSE_STATE_SHORT_SPACE;
      if(shortspace() == 1){
        //letter is transmitted
        letter = NULL;
        i = 0;
        return 1;
      }

    }
  }

  return 0;
}

/*
void led_ctrl_morse_msg(const char *msg)
{

  for (size_t i = 0; i < strlen(msg); i++)
  {
    led_ctrl_morse_char(msg[i]);
  }
}
*/