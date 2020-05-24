#ifndef SETTINGS_H__
#define SETTINGS_H__

#include "config.h"

struct parameters{

    char ap_ssid[32]; // Access point SSID name
    char ap_pwd[32];  // Access point password

    int led_state;
    int led_mode;


    uint16_t crc;
};


extern struct parameters params;

void params_init(void);




#endif


