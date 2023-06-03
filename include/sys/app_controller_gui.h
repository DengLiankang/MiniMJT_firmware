#ifndef APP_CONTROLLER_GUI_H
#define APP_CONTROLLER_GUI_H

#include "Arduino.h"
#include "common.h"
#include "lvgl.h"

struct AppCtrlMenuPage {
    lv_obj_t *appMenuScr;
    lv_obj_t *appImg;
    lv_obj_t *appName;
    struct AppCtrlMenuPage *nextPage;
};

void AppCtrlStyleInit(void);
void AppCtrlMenuGuiInit(void);
void AppCtrlMenuGuiRelease(void);
void AppCtrlLoadingGuiInit(void);
void AppCtrlLoadingGuiRelease(void);
void AppCtrlLoadingDisplay(int progress, const char *text, bool wait);
void AppCtrlMenuDisplay(const void *appImg, const char *appName, lv_scr_load_anim_t anim);

#endif