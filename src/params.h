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

struct params_led_ctrl_0002{

    uint8_t rmin;
    uint8_t rmax;
    uint8_t gmin;
    uint8_t gmax;
    uint8_t bmin;
    uint8_t bmax;

    uint8_t rrate;
    uint8_t grate;
    uint8_t brate;

    uint16_t col_upd_rate;
};


struct params_led_ctrl_0007{

    char msg[32];
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint32_t puase_btw_msg; //pause between messages
};

struct parameters{

    char ap_ssid[32]; // Access point SSID name
    char ap_pwd[32];  // Access point password

    char sta_ssid[3][32];
    char sta_pwd[3][32];

    char host_name[32];

    int led_mode;

    struct params_led_ctrl_0001 led_ctrl_0001;
    struct params_led_ctrl_0002 led_ctrl_0002;
    struct params_led_ctrl_0007 led_ctrl_0007;

    /*checksum must be at the end of structure*/
    uint32_t crc;
};


extern struct parameters params;

void params_init(void);
void write_gen_params(struct parameters *ctx);



#endif


