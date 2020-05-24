#include "params.h"

#include <EEPROM.h>
#include <FS.h>

#include "config.h"



#define EEPROM_SIZE 4096

#define PARAMS_LED_CTRL_FN_PREF "/led_ctrl_"

#define PARAMS_START_ADR    0


struct parameters params;



uint16_t checksum16_calc(void *buf, size_t len){

    uint16_t checksum = 0;
    uint8_t *data = (uint8_t *)buf;

    for(size_t i = 0; i < len; i++){

        checksum ^= data[i];
        checksum <<= 1;
    }


    return checksum;
}


bool checksum16_check(void *buf, size_t len, uint16_t checksum){

    uint16_t calc_checksum = checksum16_calc(buf, len);

    return checksum == calc_checksum;
}




void read_gen_params(struct parameters *ctx){

    uint8_t *buf = (uint8_t *)ctx;

    EEPROM.begin(EEPROM_SIZE);

    for( size_t i = 0; i < sizeof(struct parameters); i++ ){
        buf[i] = EEPROM.read(PARAMS_START_ADR + i);  
    }

    EEPROM.end();
    
}

void write_gen_params(struct parameters *ctx){

    uint8_t *buf = (uint8_t *)ctx;

    ctx->crc = checksum16_calc( ctx, sizeof(struct parameters)-2 );

    EEPROM.begin(EEPROM_SIZE);

    for( size_t i = 0; i < sizeof(struct parameters); i++ ){
        EEPROM.write(PARAMS_START_ADR + i, buf[i]);
    }

    EEPROM.commit();
    EEPROM.end();
}

void write_def_gen_params(struct parameters *ctx){

    sprintf(ctx->ap_ssid, "%s", DEFAULT_AP_SSID);
    sprintf(ctx->ap_pwd,  "%s", DEFAULT_AP_PWD);

    ctx->led_mode = 0;
    ctx->led_state = 0;

    write_gen_params(ctx);
}


void params_init(void){


    //read general parameters

    

    read_gen_params(&params);
    
    if( checksum16_check(&params, sizeof(struct parameters) - 2, params.crc) != true ){

        write_def_gen_params(&params);

    }

    



/*
    SPIFFS.begin();

    if( SPIFFS.exists(PARAMS_LED_CTRL_FN_PREF + "0") ){
    }
*/
    
}