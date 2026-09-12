#pragma once
#include <Arduino.h>

extern int16_t touch_raw_x;
extern int16_t touch_raw_y;
extern int16_t touch_last_x;
extern int16_t touch_last_y;
extern bool touch_swap_xy;
extern int16_t touch_map_x1;
extern int16_t touch_map_x2;
extern int16_t touch_map_y1;
extern int16_t touch_map_y2;
extern int16_t touch_max_x;
extern int16_t touch_max_y;

void touch_init(int16_t w, int16_t h, uint8_t r);
bool touch_has_signal();
void translate_touch_raw();
bool touch_touched();
bool touch_released();