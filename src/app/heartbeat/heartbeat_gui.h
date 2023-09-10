#ifndef APP_HEARTBEAT_GUI_H
#define APP_HEARTBEAT_GUI_H

#include "common.h"
#include "lvgl.h"
#include <Arduino.h>

void HeartbeatAppGuiInit();

void HeartbeatAppDisplayError();

void HeartbeatAppDisplayImage(const String filePath, lv_scr_load_anim_t anim);

void HeartbeatAppGuiRelease(void);

LV_IMG_DECLARE(HeartbeatAppLogo);

#endif