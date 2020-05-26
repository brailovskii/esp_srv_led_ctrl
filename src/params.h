#ifndef SETTINGS_H__
#define SETTINGS_H__

#include "config.h"
#include "stdlib.h"



struct params_led_ctrl_0001{

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t br;
};


struct parameters{

    char ap_ssid[32]; // Access point SSID name
    char ap_pwd[32];  // Access point password

    char sta_ssid[3][32];
    char sta_pwd[3][32];

    char host_name[32];

    int led_mode;

    struct params_led_ctrl_0001 led_ctrl_0001;

    /*checksum must be at the end of structure*/
    uint32_t crc;
};


extern struct parameters params;

void params_init(void);
void write_gen_params(struct parameters *ctx);



#endif


