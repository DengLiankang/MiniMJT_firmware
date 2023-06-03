#include "picture_gui.h"

static lv_obj_t *lv_pictureAppScr = NULL;
static lv_obj_t *lv_displayImg = NULL;
static lv_obj_t *lv_errorMsgLabel = NULL;

static lv_style_t default_style;

void PictureAppGuiInit()
{
    lv_pictureAppScr = lv_obj_create(NULL);

    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, lv_color_hex(0x000000));

    lv_obj_add_style(lv_pictureAppScr, &default_style, LV_STATE_DEFAULT);
    lv_scr_load_anim(lv_pictureAppScr, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, false);
}

void PictureAppDisplayError()
{
    lv_errorMsgLabel = lv_label_create(lv_pictureAppScr);
    lv_obj_set_style_text_color(lv_errorMsgLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_recolor(lv_errorMsgLabel, true);
    lv_obj_align(lv_errorMsgLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(lv_errorMsgLabel, "No Supported Files");
}

void PictureAppDisplayImage(const String filePath, lv_scr_load_anim_t anim)
{
    String lvFilePath = "S:" + filePath;
    lv_img_set_src(lv_displayImg, lvFilePath.c_str());
    lv_obj_align(lv_displayImg, LV_ALIGN_CENTER, 0, 0);
    lv_scr_load_anim(lv_pictureAppScr, anim, 500, 0, false);
    ANIEND_WAIT(600);
}

void PictureAppGuiRelease(void)
{
    if (NULL != lv_pictureAppScr) {
        lv_obj_del(lv_pictureAppScr); // 清空此前页面
        lv_pictureAppScr = NULL;
        lv_displayImg = NULL;
        lv_errorMsgLabel = NULL;
    }
}