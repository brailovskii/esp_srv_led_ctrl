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
    }

    checksum = ( ((checksum<<8)&0xFF00) | checksum) + 1;

    return checksum;
}




/*read general parameters*/
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

    ctx->crc = checksum16_calc( ctx, sizeof(struct parameters)-4 );

    EEPROM.begin(EEPROM_SIZE);

    for( size_t i = 0; i < sizeof(struct parameters); i++ ){
        EEPROM.write(PARAMS_START_ADR + i, buf[i]);
    }

    EEPROM.commit();
    EEPROM.end();
}

void write_def_gen_params(struct parameters *ctx){

    memset(ctx, 0, sizeof(struct parameters));

    sprintf(ctx->ap_ssid, "%s", DEFAULT_AP_SSID);
    sprintf(ctx->ap_pwd,  "%s", DEFAULT_AP_PWD);

    sprintf(&ctx->sta_ssid[0][0], "%s", DEFAULT_STA_SSID_1);
    sprintf(&ctx->sta_pwd[0][0],  "%s", DEFAULT_STA_PWD_1);
    sprintf(&ctx->sta_ssid[1][0], "%s", DEFAULT_STA_SSID_2);
    sprintf(&ctx->sta_pwd[1][0],  "%s", DEFAULT_STA_PWD_2);
    sprintf(&ctx->sta_ssid[2][0], "%s", DEFAULT_STA_SSID_3);
    sprintf(&ctx->sta_pwd[2][0],  "%s", DEFAULT_STA_PWD_3);

    sprintf(ctx->host_name, "%s", DEFAULT_HOST_NAME);


    ctx->led_mode = 0;
    ctx->led_state = 0;

    write_gen_params(ctx);
}


void params_init(void){


    //read general parameters
    read_gen_params(&params);

    Serial.println("Reading dev parameters...");
    Serial.println("STA SSID passwords");
    Serial.print(&params.sta_ssid[0][0]);
    Serial.print(" ");
    Serial.print(&params.sta_pwd[0][0]);
    Serial.println("\n....");
    Serial.print(&params.sta_ssid[1][0]);
    Serial.print(" ");
    Serial.print(&params.sta_pwd[1][0]);
    Serial.println("\n....");
    Serial.print(&params.sta_ssid[2][0]);
    Serial.print(" ");
    Serial.print(&params.sta_pwd[2][0]);
    Serial.print("\n.....\n");



    Serial.println("Settings CRC is: ");
    Serial.println(params.crc);
    Serial.println("...");

    uint32_t clc_crc = checksum16_calc(&params, sizeof(struct parameters)-sizeof(params.crc) );

    Serial.println("Calculated CRC is: ");
    Serial.println(clc_crc);
    Serial.println("...");



    if( clc_crc != params.crc ){

        Serial.println("\nRestoring to default parameters\n");
        write_def_gen_params(&params);

    }else{
        Serial.println("Settings have been read");
    }

/*
    SPIFFS.begin();

    if( SPIFFS.exists(PARAMS_LED_CTRL_FN_PREF + "0") ){
    }
*/
    
}