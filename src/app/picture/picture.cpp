#include "picture.h"
#include "sys/app_controller.h"

#define PICTURE_APP_NAME "Picture"

static PICTURE_APP_CONFIG g_pictureAppCfg;
PictureApp *g_pictureApp = NULL;

static void ToString(PICTURE_APP_CONFIG *cfg, String &result)
{
    if (cfg == NULL) {
        Serial.println("[ERROR] cfg is NULL");
        return;
    }
    result = "";
    result += "autoSwitchInterval:" + String(cfg->autoSwitchInterval);
}

static void fromString(const char *cfgStr, PICTURE_APP_CONFIG *cfg)
{
    String tmpStr(cfgStr);
    cfg->autoSwitchInterval = SplitCfgString(tmpStr).toInt();
}

static void WriteConfig(PICTURE_APP_CONFIG *cfg)
{
    if (cfg == NULL) {
        return;
    }
    String cfgStr;
    ToString(cfg, cfgStr);
    WriteConfigToCard(PICTURE_APP_NAME, cfgStr.c_str());
}

static void ReadConfig(PICTURE_APP_CONFIG *cfg)
{
    if (cfg == NULL) {
        return;
    }
    String cfgStr;
    uint8_t tmpStr[50];
    int16_t size = ReadConfigFromCard(PICTURE_APP_NAME, tmpStr);
    if (size <= 0) {
        // 默认值
        cfg->autoSwitchInterval = 10000;
        WriteConfig(cfg);
        return;
    }
    fromString((const char *)tmpStr, cfg);
}

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool JpgOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
    // Stop further decoding as image is running off bottom of screen
    if (y >= tft->height())
        return 0;

    // This function will clip the image block rendering automatically at the TFT boundaries
    tft->pushImage(x, y, w, h, bitmap);

    // This might work instead if you adapt the sketch to use the Adafruit_GFX library
    // tft.drawRGBBitmap(x, y, bitmap, w, h);

    // Return 1 to decode next block
    return 1;
}

PictureApp::PictureApp()
{
    // 保存系统的tft设置参数 用于退出时恢复设置
    m_sysSwapStatus = tft->getSwapBytes();
    m_nowDispItem = 0;
    m_lastRefreshMillis = 0;

    tft->setSwapBytes(true); // We need to swap the colour bytes (endianess)
    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
    TJpgDec.setJpgScale(1);
    // The decoder must be given the exact name of the rendering function above
    TJpgDec.setCallback(JpgOutput);

    GetAllSupportedFiles(IMAGE_PATH);
    if (m_imgFiles.size() == 0) {
        PictureAppDisplayError();
    }
}

PictureApp::~PictureApp()
{
    PictureAppGuiRelease();
    // 释放文件
    m_imgFiles.clear();
    // 恢复此前的驱动参数
    tft->setSwapBytes(m_sysSwapStatus);
}

String PictureApp::GetLastImgFilePath(void)
{
    if (m_imgFiles.size() == 0)
        return "";

    if (m_nowDispItem == 0) {
        m_nowDispItem = m_imgFiles.size() - 1;
        return m_imgFiles[m_nowDispItem];
    }
    return m_imgFiles[--m_nowDispItem];
}

String PictureApp::GetNextImgFilePath(void)
{
    if (m_imgFiles.size() == 0)
        return "";

    if (m_nowDispItem == m_imgFiles.size() - 1) {
        m_nowDispItem = 0;
        return m_imgFiles[m_nowDispItem];
    }
    return m_imgFiles[++m_nowDispItem];
}

bool PictureApp::NeedAutoRefresh(void)
{
    if (g_pictureAppCfg.autoSwitchInterval != 0 &&
        millis() - g_pictureApp->m_lastRefreshMillis >= g_pictureAppCfg.autoSwitchInterval) {
        return true;
    }
    return false;
}

void PictureApp::DisplayImage(String filePath)
{
    if (filePath == "")
        return;

    if (filePath.indexOf(".jpg") != -1 || filePath.indexOf(".JPG") != -1) {
        // 直接解码jpg格式的图片
        TJpgDec.drawSdJpg(0, 0, filePath);
        delay(500);
    } else if (filePath.indexOf(".bin") != -1 || filePath.indexOf(".BIN") != -1) {
        // 使用LVGL的bin格式的图片
        PictureAppDisplayImage(filePath.c_str(), LV_SCR_LOAD_ANIM_NONE);
    }
    // 重置更新的时间标记
    g_pictureApp->m_lastRefreshMillis = millis();
}

int16_t PictureApp::GetAllSupportedFiles(const char *filePath)
{
    int16_t result = 0;
    String totalFileStr = g_tfCard.ListDir(filePath, 0);
    m_imgFiles.clear();
    while (1) {
        int startIndex = totalFileStr.indexOf("FILE: ");
        if (startIndex == -1)
            break;
        totalFileStr = totalFileStr.substring(startIndex + 6);
        String fileName = totalFileStr.substring(0, totalFileStr.indexOf(" SIZE"));
        if (fileName.indexOf(".bin") == -1 && fileName.indexOf(".BIN") == -1 && fileName.indexOf(".jpg") == -1 &&
            fileName.indexOf(".JPG") == -1) {
            continue;
        }
        m_imgFiles.push_back(String(filePath) + fileName);
        result++;
    }
    return result;
}

static int PictureAppInit(AppController *sys)
{
    ReadConfig(&g_pictureAppCfg);
    PictureAppGuiInit();
    g_pictureApp = new PictureApp();

    return 0;
}

static void PictureAppMainPorcess(AppController *sys, const ImuAction *act_info)
{
    if (g_pictureApp == NULL)
        return;

    if (RETURN == act_info->active) {
        sys->AppExit();
        return;
    }

    String filePath;
    bool refreshFlag = false;
    switch (act_info->active) {
        case TURN_RIGHT:
        case DOWN:
            filePath = g_pictureApp->GetNextImgFilePath();
            refreshFlag = true;
            break;
        case TURN_LEFT:
        case UP:
            filePath = g_pictureApp->GetLastImgFilePath();
            refreshFlag = true;
            break;
        default:
            break;
    }

    // 自动切换的时间检测
    if (!refreshFlag && g_pictureApp->NeedAutoRefresh()) {
        refreshFlag = true;
    }

    if (refreshFlag) {
        Serial.print(F("Display image: "));
        Serial.println(filePath);
        g_pictureApp->DisplayImage(filePath);
    }
}

static int PictureAppExit(void *param)
{
    WriteConfig(&g_pictureAppCfg);

    if (NULL != g_pictureApp) {
        delete g_pictureApp;
        g_pictureApp = NULL;
    }
    return 0;
}

static void PictureAppMessageHandle(const char *from, const char *to, APP_MESSAGE_TYPE type, void *data,
                                    void *extData)
{
    switch (type) {
        case APP_MESSAGE_GET_PARAM: {
            if (!strcmp((const char *)data, "switchInterval")) {
                snprintf((char *)extData, 32, "%lu", g_pictureAppCfg.autoSwitchInterval);
            } else {
                snprintf((char *)extData, 32, "%s", "NULL");
            }
        } break;
        case APP_MESSAGE_SET_PARAM: {
            if (!strcmp((const char *)data, "switchInterval")) {
                g_pictureAppCfg.autoSwitchInterval = atol((const char *)extData);
            }
        } break;
        case APP_MESSAGE_READ_CFG: {
            ReadConfig(&g_pictureAppCfg);
        } break;
        case APP_MESSAGE_WRITE_CFG: {
            WriteConfig(&g_pictureAppCfg);
        } break;
        default:
            break;
    }
}

APP_OBJ PICTURE_APP = {PICTURE_APP_NAME, &PictureAppLogo,        "", PictureAppInit, PictureAppMainPorcess, NULL,
                       PictureAppExit,   PictureAppMessageHandle};