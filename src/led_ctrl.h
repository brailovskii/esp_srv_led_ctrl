#ifndef _LED_CTRL_H__
#define _LED_CTRL_H__



void led_ctrl_init(void);

void led_ctrl_proces(void);

void led_ctrl_0001(int r, int g, int b);

void led_ctrl_morse_msg(const char * msg);

void led_ctr_set_all(int r, int g, int b);


int led_ctrl_parse_json_msg(const char *json_msg, char *resp);

#endif
