#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SLIDE2 = 2,
    _SCREEN_ID_LAST = 2
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *slide2;
    lv_obj_t *botton;
    lv_obj_t *enterpass;
    lv_obj_t *space1;
    lv_obj_t *ledr;
    lv_obj_t *space1_1;
    lv_obj_t *ledg;
    lv_obj_t *space1_2;
    lv_obj_t *ledred;
    lv_obj_t *space1_3;
    lv_obj_t *ledgreen;
    lv_obj_t *space1_4;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_slide2();
void tick_screen_slide2();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/