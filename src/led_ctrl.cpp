#include "led_ctrl.h"
#include <FastLED.h>
#include "config.h"

CRGB leds[NUM_LEDS];

void led_ctrl_init(void)
{

    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void led_ctrl_proces(void)
{
}

void led_ctr_set_all(int r, int g, int b, int brightness)
{
    float r_f = r * LED_FACTORY_BRIGHTNESS_COEF * (float)brightness * 255.0 / 10000.0;
    float g_f = g * LED_FACTORY_BRIGHTNESS_COEF * (float)brightness * 255.0 / 10000.0;
    float b_f = b * LED_FACTORY_BRIGHTNESS_COEF * (float)brightness * 255.0 / 10000.0;

    r = (int)r_f;
    g = (int)g_f;
    b = (int)b_f;

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(r, g, b);
    }

    FastLED.show();
}

void led_ctrl_0001(int r, int g, int b, int brightness)
{
    led_ctr_set_all(r, g, b, brightness);
}






void space() { delay(1200); } //space between words
void dot()
{
    led_ctr_set_all(100, 100,100,100);
    delay(300);
    led_ctr_set_all(0, 0,0,0);
    delay(300);
} //the dot this code make the led on for 300 than off for 300

void dash()
{
    led_ctr_set_all(100, 100,100,100);
    delay(900);
    led_ctr_set_all(0, 0,0,0);
    delay(300);
} //the dash this code make the led on for 900 than off for 300

void shortspace() { delay(600); } //space between letters

//fonctions for the letters and the numbers
void lA () {dot();dash();shortspace();}//letter A in morse code!
void lB () {dash();dot();dot();dot();shortspace();}//same for B
void lC () {dash();dot();dash();dot();shortspace();}
void lD () {dash();dot();dot();shortspace();}
void lE () {dot();shortspace();}
void lF () {dot();dot();dash();dot();shortspace();}
void lG () {dash();dash();dot();shortspace();}
void lH () {dot();dot();dot();dot();shortspace();}
void lI () {dot();dot();shortspace();}
void lJ () {dot();dash();dash();dash();shortspace();}
void lK () {dash();dot();dash();shortspace();}
void lL () {dot();dash();dot();dot();shortspace();}
void lM () {dash();dash();shortspace();}
void lN () {dash();dot();shortspace();}
void lO () {dash();dash();dash();shortspace();}
void lP () {dot();dash();dash();dot();shortspace();}
void lQ () {dash();dash();dot();dash();shortspace();}
void lR () {dot();dash();dot();shortspace();}
void lS () {dot();dot();dot();shortspace();}
void lT () {dash();shortspace();}
void lU () {dot();dot();dash();shortspace();}
void lV () {dot();dot();dot();dash();shortspace();}
void lW () {dot();dash();dash();shortspace();}
void lX () {dash();dot();dot();dash();shortspace();}
void lY () {dash();dot();dash();dash();shortspace();}
void lZ () {dash();dash();dot();dot();shortspace();}
void n1 () {dot();dash();dash();dash();dash();shortspace();}//number 1 in morse code
void n2 () {dot();dot();dash();dash();dash();shortspace();}
void n3 () {dot();dot();dot();dash();dash();shortspace();}
void n4 () {dot();dot();dot();dot();dash();shortspace();}
void n5 () {dot();dot();dot();dot();dot();shortspace();}
void n6 () {dash();dot();dot();dot();dot();shortspace();}
void n7 () {dash();dash();dot();dot();dot();shortspace();}
void n8 () {dash();dash();dash();dot();dot();shortspace();}
void n9 () {dash();dash();dash();dash();dot();shortspace();}
void n0 () {dash();dash();dash();dash();dash();shortspace();}



void led_ctrl_morse_char(char input)
{

    if (input == 'a' || input == 'A') {lA();}//if the input is a or A go to function lA
    if (input == 'b' || input == 'B') {lB();}//same but with b letter
    if (input == 'c' || input == 'C') {lC();}
    if (input == 'd' || input == 'D') {lD();}
    if (input == 'e' || input == 'E') {lE();}
    if (input == 'f' || input == 'F') {lF();}
    if (input == 'g' || input == 'G') {lG();}
    if (input == 'h' || input == 'H') {lH();}
    if (input == 'i' || input == 'I') {lI();}
    if (input == 'j' || input == 'J') {lJ();}
    if (input == 'k' || input == 'K') {lK();}
    if (input == 'l' || input == 'L') {lL();}
    if (input == 'm' || input == 'M') {lM();}
    if (input == 'n' || input == 'N') {lN();}
    if (input == 'o' || input == 'O') {lO();}
    if (input == 'p' || input == 'P') {lP();}
    if (input == 'q' || input == 'Q') {lQ();}
    if (input == 'r' || input == 'R') {lR();}
    if (input == 's' || input == 'S') {lS();}
    if (input == 't' || input == 'T') {lT();}
    if (input == 'u' || input == 'U') {lU();}
    if (input == 'v' || input == 'V') {lV();}
    if (input == 'w' || input == 'W') {lW();}
    if (input == 'x' || input == 'X') {lX();}
    if (input == 'y' || input == 'Y') {lY();}
    if (input == 'z' || input == 'Z') {lZ();}
    if (input == '1') {n1();}// the numbers
    if (input == '2') {n2();}
    if (input == '3') {n3();}
    if (input == '4') {n4();}
    if (input == '5') {n5();}
    if (input == '6') {n6();}
    if (input == '7') {n7();}
    if (input == '8') {n8();}
    if (input == '9') {n9();}
    if (input == '0') {n0();}
    if (input == ' ') {space();}//the space
}

void led_ctrl_morse_msg(const char *msg)
{

    for (int i = 0; i < strlen(msg); i++)
    {
        led_ctrl_morse_char(msg[i]);
    }
}
