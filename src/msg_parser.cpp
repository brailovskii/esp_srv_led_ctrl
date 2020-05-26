#include "msg_parser.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "params.h"

#include <HardwareSerial.h>

int led_ctrl_parse_json_msg_id_0001(const char *json_msg, char *resp)
{

    //{"type":"led_ctrl","id":"1","save":"no","cnt":"0","r":"25","g":"26","b":"36","br":"100"}

    int r = -1, g = -1, b = -1, br = -1;
    int cnt;

    // Serial.println("paser 0001");
    // Serial.println(json_msg);

    int res = 0;
    if (strstr(json_msg, "\"save\":\"yes\"") != NULL)
    {
        res = sscanf(json_msg,
                     "{\"type\":\"led_ctrl\",\"id\":\"1\",\"save\":\"yes\",\"cnt\":\"%d\",\"r\":\"%d\",\"g\":\"%d\",\"b\":\"%d\",\"br\":\"%d\"}",
                     &cnt, &r, &g, &b, &br);
    }
    else
    {
        res = sscanf(json_msg,
                     "{\"type\":\"led_ctrl\",\"id\":\"1\",\"save\":\"no\",\"cnt\":\"%d\",\"r\":\"%d\",\"g\":\"%d\",\"b\":\"%d\",\"br\":\"%d\"}",
                     &cnt, &r, &g, &b, &br);
    }

    // Serial.println("Recovered parameters: ");
    // Serial.println(cnt);
    // Serial.println(r);
    // Serial.println(g);
    // Serial.println(b);
    // Serial.println(br);

    if (res != 5)
    {
        sprintf(resp, "led_ctrl_001 parse error");
        return -1;
    }

    params.led_ctrl_0001.r = r;
    params.led_ctrl_0001.g = g;
    params.led_ctrl_0001.b = b;
    params.led_ctrl_0001.br = br;

    sprintf(resp, "! r: %d g: %d b: %d br: %d ", r, g, b, br);

    return 0;
}

int led_ctrl_parse_json_msg_id_0002(const char *json_msg, char *resp)
{

    //{"type":"led_ctrl","id":"2","save":"no","cnt":"0","rmin":"255","rmax":"255","gmin":"255","gmax":"255","bmin":"255","bmax":"255","rrate":"255","grate":"255","brate":"255","col_upd_rate":"255"}

    int rmin = -1, rmax = -1, gmin = -1, gmax = -1, bmin = -1, bmax = -1, rrate = -1, grate = -1, brate = -1, col_upd_rate = -1;
    int cnt;

    // Serial.println("parser 002");
    // Serial.println(json_msg);


    int res = 0;
    if (strstr(json_msg, "\"save\":\"yes\"") != NULL)
    {
    res = sscanf(json_msg,
                     "{\"type\":\"led_ctrl\",\"id\":\"2\",\"save\":\"yes\",\"cnt\":\"%d\",\"rmin\":\"%d\",\"rmax\":\"%d\",\"gmin\":\"%d\",\"gmax\":\"%d\",\"bmin\":\"%d\",\"bmax\":\"%d\",\"rrate\":\"%d\",\"grate\":\"%d\",\"brate\":\"%d\",\"col_upd_rate\":\"%d\"}",
                     &cnt, &rmin, &rmax, &gmin, &gmax, &bmin, &bmax, &rrate, &grate, &brate, &col_upd_rate);
    }
    else
    {
    res = sscanf(json_msg,
                     "{\"type\":\"led_ctrl\",\"id\":\"2\",\"save\":\"no\",\"cnt\":\"%d\",\"rmin\":\"%d\",\"rmax\":\"%d\",\"gmin\":\"%d\",\"gmax\":\"%d\",\"bmin\":\"%d\",\"bmax\":\"%d\",\"rrate\":\"%d\",\"grate\":\"%d\",\"brate\":\"%d\",\"col_upd_rate\":\"%d\"}",
                     &cnt, &rmin, &rmax, &gmin, &gmax, &bmin, &bmax, &rrate, &grate, &brate, &col_upd_rate);
    }



    if (res != 11)
    {
        //one of parameters could not be read
        sprintf(resp, "led_ctrl_0002 parse error");
        return -1;
    }

    params.led_ctrl_0002.rmin = rmin;
    params.led_ctrl_0002.rmax = rmax;
    params.led_ctrl_0002.gmin = gmin;
    params.led_ctrl_0002.gmax = gmax;
    params.led_ctrl_0002.bmin = bmin;
    params.led_ctrl_0002.bmax = bmax;
    params.led_ctrl_0002.rrate = rrate;
    params.led_ctrl_0002.grate = grate;
    params.led_ctrl_0002.brate = brate;
    params.led_ctrl_0002.col_upd_rate = col_upd_rate;

    sprintf(resp, "! %d;%d;%d;%d;%d;%d; %d;%d;%d; %d;", rmin, rmax, gmin, gmax, bmin, bmax, rrate, grate, brate, col_upd_rate);

    return 0;
}

int msg_parser_parse(const char *json_msg, char *resp)
{

    *resp = 0;
    int need_save = 0;

    if (strstr(json_msg, "\"type\":\"led_ctrl\"") == NULL)
    {
        //not led control message
        return -2;
    }

    if (strstr(json_msg, "\"save\":\"yes\"") != NULL)
    {
        need_save = 1;
    }

    int id = 0;
    const char *ctrl_id = strstr(json_msg, "\"id\":\"");
    if (ctrl_id != NULL)
    {
        if (ctrl_id[7] == '"')
        {
            id = ctrl_id[6] - '0';
        }
        else if (ctrl_id[8] == '"')
        {
            id = (ctrl_id[6] - '0') * 10 + ctrl_id[7] - '0';
        }
    }
    else
    {
        sprintf(resp, " error parsing ID");
        return -3;
    }

    int res = 0;
    switch (id)
    {

    case 1:
        res = led_ctrl_parse_json_msg_id_0001(json_msg, resp);
        if (res == 0)
        {
            params.led_mode = 1;
        }
        break;

    case 2:
        res = led_ctrl_parse_json_msg_id_0002(json_msg, resp);
        if (res == 0)
        {
            params.led_mode = 2;
        }

    default:
        break;
    }

    if (need_save)
    {
        //save gloabal parameters
        write_gen_params(&params);
    }

    return res;
}
