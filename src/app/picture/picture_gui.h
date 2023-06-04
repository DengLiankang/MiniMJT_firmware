#ifndef APP_PICTURE_GUI_H
#define APP_PICTURE_GUI_H

#define PIC_FILENAME_MAX_LEN 100

#include "lvgl.h"
#include <Arduino.h>
#include "common.h"

void PictureAppGuiInit(void);
void PictureAppDisplayError();
void PictureAppDisplayImage(const String filePath, lv_scr_load_anim_t anim);
void PictureAppGuiRelease(void);
void PictureAppGuiChildRelease(void);

LV_IMG_DECLARE(PictureAppLogo);

#endif