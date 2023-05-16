#ifndef APP_PICTURE_GUI_H
#define APP_PICTURE_GUI_H

#define PIC_FILENAME_MAX_LEN 100

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void photo_gui_init(void);
void display_photo_init(void);
void display_photo(const char *file_name, lv_scr_load_anim_t anim_type);
void photo_gui_del(void);

LV_IMG_DECLARE(PictureAppLogo);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif