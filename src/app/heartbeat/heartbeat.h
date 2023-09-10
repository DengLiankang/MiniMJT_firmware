#ifndef APP_HEARTBEAT_H
#define APP_HEARTBEAT_H

#include "common.h"
#include "heartbeat_gui.h"
#include "sys/interface.h"
#include <vector>

#define HEART_PATH "/heartbeat"

class HeartbeatApp
{
public:
    HeartbeatApp();
    ~HeartbeatApp();

    bool NeedAutoRefresh(void);
    void DisplayNextImage(void);

private:
    int16_t GetAllValidFiles(const char *filePath);
    String GetNextImgFilePath(void);

private:
    std::vector<String> m_imgFiles;
    int m_nowDispItem;
    unsigned long m_lastRefreshMillis;
    String m_nowFilePath;
    unsigned long m_frameInterval;
};

extern APP_OBJ heartbeat_app;

#endif