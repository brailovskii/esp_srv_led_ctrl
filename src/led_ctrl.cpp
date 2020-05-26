#include "led_ctrl.h"
#include "params.h"
#include <FastLED.h>

void prc_led_ctrl_0000(void);
void prc_led_ctrl_0001(void);
void prc_led_ctrl_0002(void);
void prc_led_ctrl_0007(void);

CRGB leds[NUM_LEDS];
uint8_t leds_buf[3][NUM_LEDS];
static uint8_t morse_rgb[3];

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



void prc_led_ctrl_0007(void)
{
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













void space()
{
  delay(1200);
} // space between words

void dot()
{
  led_ctr_set_all(morse_rgb[0], morse_rgb[1], morse_rgb[2]);
  delay(300);
  led_ctr_set_all(0, 0, 0);
  delay(300);
} // the dot this code make the led on for 300 than off for 300

void dash()
{
  led_ctr_set_all(morse_rgb[0], morse_rgb[1], morse_rgb[2]);
  delay(900);
  led_ctr_set_all(0, 0, 0);
  delay(300);
} // the dash this code make the led on for 900 than off for 300

void shortspace() { delay(600); } // space between letters

// fonctions for the letters and the numbers
void lA()
{
  dot();
  dash();
  shortspace();
} // letter A in morse code!
void lB()
{
  dash();
  dot();
  dot();
  dot();
  shortspace();
} // same for B
void lC()
{
  dash();
  dot();
  dash();
  dot();
  shortspace();
}
void lD()
{
  dash();
  dot();
  dot();
  shortspace();
}
void lE()
{
  dot();
  shortspace();
}
void lF()
{
  dot();
  dot();
  dash();
  dot();
  shortspace();
}
void lG()
{
  dash();
  dash();
  dot();
  shortspace();
}
void lH()
{
  dot();
  dot();
  dot();
  dot();
  shortspace();
}
void lI()
{
  dot();
  dot();
  shortspace();
}
void lJ()
{
  dot();
  dash();
  dash();
  dash();
  shortspace();
}
void lK()
{
  dash();
  dot();
  dash();
  shortspace();
}
void lL()
{
  dot();
  dash();
  dot();
  dot();
  shortspace();
}
void lM()
{
  dash();
  dash();
  shortspace();
}
void lN()
{
  dash();
  dot();
  shortspace();
}
void lO()
{
  dash();
  dash();
  dash();
  shortspace();
}
void lP()
{
  dot();
  dash();
  dash();
  dot();
  shortspace();
}
void lQ()
{
  dash();
  dash();
  dot();
  dash();
  shortspace();
}
void lR()
{
  dot();
  dash();
  dot();
  shortspace();
}
void lS()
{
  dot();
  dot();
  dot();
  shortspace();
}
void lT()
{
  dash();
  shortspace();
}
void lU()
{
  dot();
  dot();
  dash();
  shortspace();
}
void lV()
{
  dot();
  dot();
  dot();
  dash();
  shortspace();
}
void lW()
{
  dot();
  dash();
  dash();
  shortspace();
}
void lX()
{
  dash();
  dot();
  dot();
  dash();
  shortspace();
}
void lY()
{
  dash();
  dot();
  dash();
  dash();
  shortspace();
}
void lZ()
{
  dash();
  dash();
  dot();
  dot();
  shortspace();
}
void n1()
{
  dot();
  dash();
  dash();
  dash();
  dash();
  shortspace();
} // number 1 in morse code
void n2()
{
  dot();
  dot();
  dash();
  dash();
  dash();
  shortspace();
}
void n3()
{
  dot();
  dot();
  dot();
  dash();
  dash();
  shortspace();
}
void n4()
{
  dot();
  dot();
  dot();
  dot();
  dash();
  shortspace();
}
void n5()
{
  dot();
  dot();
  dot();
  dot();
  dot();
  shortspace();
}
void n6()
{
  dash();
  dot();
  dot();
  dot();
  dot();
  shortspace();
}
void n7()
{
  dash();
  dash();
  dot();
  dot();
  dot();
  shortspace();
}
void n8()
{
  dash();
  dash();
  dash();
  dot();
  dot();
  shortspace();
}
void n9()
{
  dash();
  dash();
  dash();
  dash();
  dot();
  shortspace();
}
void n0()
{
  dash();
  dash();
  dash();
  dash();
  dash();
  shortspace();
}

void led_ctrl_morse_char(char input)
{

  if (input == 'a' || input == 'A')
  {
    lA();
  } // if the input is a or A go to function lA
  if (input == 'b' || input == 'B')
  {
    lB();
  } // same but with b letter
  if (input == 'c' || input == 'C')
  {
    lC();
  }
  if (input == 'd' || input == 'D')
  {
    lD();
  }
  if (input == 'e' || input == 'E')
  {
    lE();
  }
  if (input == 'f' || input == 'F')
  {
    lF();
  }
  if (input == 'g' || input == 'G')
  {
    lG();
  }
  if (input == 'h' || input == 'H')
  {
    lH();
  }
  if (input == 'i' || input == 'I')
  {
    lI();
  }
  if (input == 'j' || input == 'J')
  {
    lJ();
  }
  if (input == 'k' || input == 'K')
  {
    lK();
  }
  if (input == 'l' || input == 'L')
  {
    lL();
  }
  if (input == 'm' || input == 'M')
  {
    lM();
  }
  if (input == 'n' || input == 'N')
  {
    lN();
  }
  if (input == 'o' || input == 'O')
  {
    lO();
  }
  if (input == 'p' || input == 'P')
  {
    lP();
  }
  if (input == 'q' || input == 'Q')
  {
    lQ();
  }
  if (input == 'r' || input == 'R')
  {
    lR();
  }
  if (input == 's' || input == 'S')
  {
    lS();
  }
  if (input == 't' || input == 'T')
  {
    lT();
  }
  if (input == 'u' || input == 'U')
  {
    lU();
  }
  if (input == 'v' || input == 'V')
  {
    lV();
  }
  if (input == 'w' || input == 'W')
  {
    lW();
  }
  if (input == 'x' || input == 'X')
  {
    lX();
  }
  if (input == 'y' || input == 'Y')
  {
    lY();
  }
  if (input == 'z' || input == 'Z')
  {
    lZ();
  }
  if (input == '1')
  {
    n1();
  } // the numbers
  if (input == '2')
  {
    n2();
  }
  if (input == '3')
  {
    n3();
  }
  if (input == '4')
  {
    n4();
  }
  if (input == '5')
  {
    n5();
  }
  if (input == '6')
  {
    n6();
  }
  if (input == '7')
  {
    n7();
  }
  if (input == '8')
  {
    n8();
  }
  if (input == '9')
  {
    n9();
  }
  if (input == '0')
  {
    n0();
  }
  if (input == ' ')
  {
    space();
  } // the space
}

void led_ctrl_morse_msg(const char *msg)
{

  for (size_t i = 0; i < strlen(msg); i++)
  {
    led_ctrl_morse_char(msg[i]);
  }
}
