#ifndef APP_WEATHER_H
#define APP_WEATHER_H

#include "sys/interface.h"
#include "weather_gui.h"

struct WEATHER_APP_CONFIG {
    String weatherApiAppId;                 // tianqiapid 的 appid
    String weatherApiAppSecret;             // tianqiapid 的 appsecret
    String weatherApiCityAddr;                  // tianqiapid 的地址（填中文）
    unsigned long httpUpdataInterval; // 网络更新的时间间隔(s)
};

class WeatherApp{
public:
    unsigned long m_lastHttpUpdateMillis;    // 更新时间计数器
    unsigned long m_lastUpdateLocalTimeMillis;    // 上一次的本地机器时间戳
    WIFI_STATUS m_wifiStatus; // wifi标志
    uint8_t m_wifiRetryCnt;
    boolean m_weatherUpdateFlag;
    boolean m_timeUpdateFlag;
    boolean m_forceUpdate;
    unsigned long m_lastKeepWifiMillis;
    TaskHandle_t m_updateTaskHandle;

    struct tm *m_timeInfo;
    struct WEATHER_STRUCT m_weatherInfo;     // 保存天气状况
public:
    WeatherApp();
    ~WeatherApp();

    void UpdateTimeInfo(struct timeval *tv = NULL);
    void GetNowWeather(void);
    void GetDailyWeather(short maxT[], short minT[]);
    struct timeval GetNetworkTime(void);

private:

    void ValidateConfig(struct WEATHER_APP_CONFIG *cfg, const struct WEATHER_APP_CONFIG *defaultConfig);
};

extern APP_OBJ WEATHER_APP;

#endif