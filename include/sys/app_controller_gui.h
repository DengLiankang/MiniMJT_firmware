#ifndef APP_CONTROLLER_GUI_H
#define APP_CONTROLLER_GUI_H

#include "Arduino.h"
#include "lvgl.h"
#include "common.h"

#define ANIEND_WAIT                     \
    do {                                \
        lv_timer_handler();             \
        delay(1);                       \
    } while (lv_anim_count_running())

struct AppCtrlMenuPage {
    lv_obj_t *appMenuScr;
    lv_obj_t *appImg;
    lv_obj_t *appName;
    struct AppCtrlMenuPage *nextPage;
};

void AppCtrlMenuGuiInit(void);
void AppCtrlLoadingGuiInit(void);
void AppCtrlLoadingDisplay(int progress, const char *text, bool wait);
void AppCtrlMenuDisplay(const char *appName, lv_scr_load_anim_t anim, bool delPre);

#endif