/*
    Keys.h
    Created on Jul 09, 2024
    Author: @POEG1726
*/

#ifndef _KEYS_H_
#define _KEYS_H_

#include "stm32g0xx.h"
#include <stdint.h>
#include <string.h>

#define TICKS_INTERVAL    5	//ms
#define SHORT_TICKS       (10/* ms */ /TICKS_INTERVAL)
#define LONG_TICKS        (400/* ms */ /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);

typedef enum
{
    PRESS_DOWN = 0,
    PRESS_UP,
    PRESS_REPEAT,
    SINGLE_CLICK,
    DOUBLE_CLICK,
    LONG_PRESS_START,
    LONG_PRESS_HOLD,
    number_of_event,
    NONE_PRESS
}PressEvent;

typedef struct Key
{
    uint16_t ticks;
    uint8_t  repeat : 4;
    uint8_t  event : 4;
    uint8_t  state : 3;
    uint8_t  debounce_cnt : 3;
    uint8_t  active_level : 1;
    uint8_t  Key_level : 1;
    uint8_t  Key_id;
    uint8_t(*hal_Key_Level)(uint8_t Key_id_);
    BtnCallback  cb[number_of_event];
    struct Key* next;
}Key;

void Key_init(struct Key* handle, uint8_t(*pin_level)(uint8_t), uint8_t active_level, uint8_t Key_id);
void Key_attach(struct Key* handle, PressEvent event, BtnCallback cb);
PressEvent get_Key_event(struct Key* handle);
int  Key_start(struct Key* handle);
void Key_ticks(void);


#endif // _KeyS_H_