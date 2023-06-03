#ifndef APP_PICTURE_H
#define APP_PICTURE_H

#include "common.h"
#include "picture_gui.h"
#include "sys/interface.h"
#include <vector>
// Include the jpeg decoder library
#include <TJpg_Decoder.h>

#define IMAGE_PATH "/picture/images"

struct PICTURE_APP_CONFIG {
    unsigned long autoSwitchInterval; // 自动播放下一张的时间间隔 ms
};

class PictureApp
{
public:
    PictureApp();
    ~PictureApp();

    String GetLastImgFilePath(void);
    String GetNextImgFilePath(void);
    bool NeedAutoRefresh(void);
    void DisplayImage();

private:
    int16_t GetAllSupportedFiles(const char *filePath);

private:
    std::vector<String> m_imgFiles;
    bool m_sysSwapStatus;
    int m_nowDispItem;
    bool m_nowDispDirection;
    unsigned long m_lastRefreshMillis;
    String m_nowFilePath;
};

extern APP_OBJ PICTURE_APP;

#endif