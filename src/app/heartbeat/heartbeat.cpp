#include "heartbeat.h"
#include "sys/app_controller.h"

#define HEARTBEAT_APP_NAME "Heartbeat"

HeartbeatApp *g_heartbeatApp = NULL;

HeartbeatApp::HeartbeatApp()
{
    m_nowDispItem = 0;
    m_lastRefreshMillis = 0;
    m_nowFilePath = "";
    m_frameInterval = 0;

    GetAllValidFiles(HEART_PATH);
    if (m_imgFiles.empty()) {
        HeartbeatAppDisplayError();
    } else {
        m_frameInterval = 1000 / m_imgFiles.size();
    }
    Serial.printf("[heartbeat] find %d valid files, frame interval: %d\n", m_imgFiles.size(), m_frameInterval);
}

HeartbeatApp::~HeartbeatApp() { m_imgFiles.clear(); }

int16_t HeartbeatApp::GetAllValidFiles(const char *filePath)
{
    int16_t result = 0;
    String totalFileStr = g_tfCard.ListDir(filePath, 0);
    m_imgFiles.clear();
    while (1) {
        int startIndex = totalFileStr.indexOf("FILE: ");
        if (startIndex == -1)
            break;
        totalFileStr = totalFileStr.substring(startIndex + 6);
        String fileName = totalFileStr.substring(0, totalFileStr.indexOf(" | SIZE"));
        if (fileName.indexOf(".bin") == -1 && fileName.indexOf(".BIN") == -1) {
            continue;
        }
        m_imgFiles.push_back(String(filePath) + "/" + fileName);
        result++;
    }
    if (!m_imgFiles.empty()) {
        m_nowFilePath = m_imgFiles[0];
    }
    return result;
}

String HeartbeatApp::GetNextImgFilePath(void)
{
    if (m_imgFiles.size() == 0)
        return "";

    if (m_nowDispItem == m_imgFiles.size() - 1) {
        m_nowDispItem = 0;
        m_nowFilePath = m_imgFiles[m_nowDispItem];
    } else {
        m_nowFilePath = m_imgFiles[++m_nowDispItem];
    }
    return m_nowFilePath;
}

bool HeartbeatApp::NeedAutoRefresh(void)
{
    if (m_nowFilePath == "")
        return false;

    if (millis() - m_lastRefreshMillis >= m_frameInterval) {
        GetNextImgFilePath();
        Serial.printf("[picture] need to auto fresh, interval: %d\n", m_frameInterval);
        return true;
    }
    return false;
}

void HeartbeatApp::DisplayNextImage(void)
{
    if (m_nowFilePath == "")
        return;

    Serial.printf("[heartbeat] DisplayImage: %s\n", m_nowFilePath.c_str());
    HeartbeatAppDisplayImage(m_nowFilePath, LV_SCR_LOAD_ANIM_NONE);
    m_lastRefreshMillis = millis();
}

static int HeartbeatAppInit(AppController *sys)
{
    HeartbeatAppGuiInit();
    g_heartbeatApp = new HeartbeatApp();
    return 0;
}

static void HeartbeatAppMainProcess(AppController *sys, const ImuAction *act_info)
{
    if (g_heartbeatApp == NULL)
        return;

    // 自动切换的时间检测
    if (g_heartbeatApp->NeedAutoRefresh()) {
        g_heartbeatApp->DisplayNextImage();
    }
}

static int HeartbeatAppExitFinish(void *param)
{
    HeartbeatAppGuiRelease();
    if (NULL != g_heartbeatApp) {
        delete g_heartbeatApp;
        g_heartbeatApp = NULL;
    }
    return 0;
}

static void HeartbeatAppMessageHandle(const char *from, const char *to, APP_MESSAGE_TYPE type, void *message,
                                      void *ext_info)
{
    return;
}

APP_OBJ heartbeat_app = {
    HEARTBEAT_APP_NAME,     &HeartbeatAppLogo,        "", HeartbeatAppInit, HeartbeatAppMainProcess, NULL, NULL,
    HeartbeatAppExitFinish, HeartbeatAppMessageHandle};
