#include "msg_parser.h"
#include <ArduinoJson.h>

#include "params.h"





int led_ctrl_parse_json_msg_id_0001(const char *json_msg, char *resp){

  //var send_str = '{"type":"led_ctrl","id":"1","save":"no","cnt":"0000","r":"0000","g":"0000","b":"0000","br":"0000"}';

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json_msg); // Deserialize the JSON document

  const char *r_str = doc["r"];
  const char *g_str = doc["g"];
  const char *b_str = doc["b"];
  const char *br_str = doc["br"];

  int r=-1, g=-1, b=-1, br=-1;

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

  //one of parameters could not be read
  if( r<0 || g<0 || b<0 || br<0 ){
    sprintf(resp, "led_ctrl_0001 parse error");
    return -1;
  }

  params.led_ctrl_0001.r = r;
  params.led_ctrl_0001.g = g;
  params.led_ctrl_0001.b = b;
  params.led_ctrl_0001.br = br;

  // r = (r*br)/255;
  // g = (g*br)/255;
  // b = (b*br)/255;

  // led_ctrl_0001(r,g,b);

  sprintf(resp, " r: %d g: %d b: %d br: %d ", r,g,b,br);

  return 0;
}


int msg_parser_parse(const char *json_msg, char *resp){

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

  const char *save_me = doc["save"];
  if( strstr(save_me, "yes") != NULL ){
    need_save = 1;
  }



  const char *ctrl_id = doc["id"];
  int id = atoi(ctrl_id);
  if( id < 0  || id > 100){
    return -3;
  }


  int res = 0;
  switch(id){

    case 1:
      res = led_ctrl_parse_json_msg_id_0001(json_msg, resp);
      if(res == 0){
        params.led_mode = 1;
      }
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

