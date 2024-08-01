#ifndef LOGSERIAL_H
#define LOGSERIAL_H

#include <Arduino.h>
#include <LogFormat.h>
#include <common.h>

using namespace std;

struct SerialStats {
    uint32_t bytesWrittenTotal;
    uint32_t messagesWrittenTotal;
};

struct SerialSetting {
    uint8_t logId;
    Stream* serial;
    const char* serviceName;
    uint8_t logLevel;
    uint8_t logFlags;
};

class LogSerial {
public:
    void begin();
    void configure(const uint8_t maxRegistrations);
    void registerSerial(const uint8_t logId, const uint8_t loglevel, const char* serviceName, Stream& serial, const uint8_t logFlags);
    void outputFromBuffer(const LogLineEntry logLineEntry, bool muteSerialOutput);
    void handlePeek(const LogLineEntry logLineEntry, const uint8_t settingIndex);
    bool mustLog(const uint8_t logId, const uint8_t logLevel);
    void outputStats();

    void enableQuery(Stream& querySerial);
    void queryCmdHelp();
    bool queryCmdPeek(const char* serviceName, const char* loglevel, const char* textFilter);
    void queryCmdStatus();
    void queryPrintPrompt();

    void peekStop();

private:
    Formatting formatter;
    SerialStats serialStats;

    SerialSetting* serialSettings;
    uint8_t maxSerialRegistrations = 0;
    uint8_t registeredSerialCount = 0;

    bool peekEnabled = false;
    uint8_t peekLoglevel = 8; // FIXME: NOLOG instead
    uint8_t peekSettingIndex = 0;
    bool peekAllServices = false;
    bool peekFilter = false; // Text filter enabled
    char peekFilterText[30];

    Stream* querySerial = nullptr;

    void write(const LogLineEntry logLineEntry, SerialSetting& setting);
};

#endif // LOGSERIAL_H