#ifndef ELOG_H
#define ELOG_H

#include <Arduino.h>
#include <LogFormat.h>
#include <LogRingBuff.h>
#include <LogSerial.h>

// #ifndef LOGGING_SD_DISABLE
#include <LogSD.h>
// #endif // LOGGING_SD_DISABLE

// #ifndef LOGGING_SPIFFS_DISABLE
#include <LogSpiffs.h>
// #endif // LOGGING_SPIFFS_DISABLE

// #ifndef LOGGING_SYSLOG_DISABLE
#include <LogSyslog.h>
// #endif // LOGGING_SYSLOG_DISABLE

#include <esp_task_wdt.h>

#define LENGTH_COMMAND 10
#define LENGTH_COMMAND_BUFFER 50
#define LENGTH_ABSOLUTE_PATH 30

class Elog {
    enum QueryDevice {
        NONE,
        SPIFFS,
        SD,
        SER,
        SYSLOG
    };

    enum QueryState {
        QUERY_DISABLED = 0,
        QUERY_WAITING_FOR_COMMAND = 1,
        QUERY_WAITING_FOR_PEEK_QUIT = 2,
        QUERY_WAITING_FOR_TYPE_CMD = 3
    };

    struct BufferStats {
        uint32_t messagesBuffered;
        uint32_t messagesDiscarded;
    };

    friend class LogTimer;
    friend class LogSpiffs;
    friend class LogSerial;
    friend class LogSD;
    friend class LogSyslog;

public:
    // Ensure that the class is a singleton
    Elog(const Elog&) = delete;
    Elog& operator=(const Elog&) = delete;
    static Elog& getInstance();

    void configure(uint16_t logLineCapacity = 50, bool waitIfBufferFull = true);
    void log(uint8_t logId, uint8_t logLevel, const char* format, ...);
    void logHex(uint8_t logId, uint8_t logLevel, const char* message, const uint8_t* data, uint16_t length);
    void configureSerial(const uint8_t maxRegistrations = 10);
    void registerSerial(const uint8_t logId, const uint8_t logLevel, const char* serviceName, Stream& serial = Serial, const uint8_t logFlags = 0);
#ifndef LOGGING_SPIFFS_DISABLE
    void configureSpiffs(const uint8_t maxRegistrations = 10);
    void registerSpiffs(const uint8_t logId, const uint8_t logLevel, const char* fileName, const uint8_t logFlags = FLAG_NONE, const uint32_t maxLogFileSize = 100000);
#endif // LOGGING_SPIFFS_DISABLE
#ifndef LOGGING_SD_DISABLE
    void configureSd(SPIClass& spi, const uint8_t cs, const uint32_t speed = 2000000, const uint8_t spiOption = DEDICATED_SPI, const uint8_t maxFilesettings = 10);
    void registerSd(const uint8_t logId, const uint8_t logLevel, const char* fileName, const uint8_t logFlags = FLAG_NONE, const uint32_t maxLogFileSize = 100000);
#endif // LOGGING_SD_DISABLE
#ifndef LOGGING_SYSLOG_DISABLE
    void configureSyslog(const char* server, uint16_t port = 514, const char* hostname = "esp32", const uint8_t maxRegistrations = 10);
    void registerSyslog(const uint8_t logId, const uint8_t logLevel, const uint8_t facility, const char* appName);
#endif // LOGGING_SYSLOG_DISABLE
    void configureInternalLogging(Stream& internalLogDevice, uint8_t internalLogLevel = ERROR, uint16_t statsEvery = 10000);
    void enableQuery(Stream& serialPort);
    void provideTime(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t minute, const uint8_t second);

private:
    Elog() { } // Private constructor. Part of singleton pattern

    LogSpiffs logSpiffs;
    LogSerial logSerial;
    LogSD logSD;
    LogSyslog logSyslog;
    Formatting formatter;
    LogRingBuff<LogLineEntry> ringBuff;

    uint8_t registeredSpiffsCount = 0;
    uint8_t registeredSdCount = 0;
    uint8_t registeredSyslogCount = 0;
    uint8_t registeredSerialCount = 0;

    Stream* internalLogDevice = &Serial;
    uint8_t internalLogLevel = ERROR; // Tell library user when he is doing something wrong by default

    BufferStats bufferStats;
    uint16_t statsEvery = 10000;

    char queryCmdBuf[LENGTH_COMMAND_BUFFER];
    int queryCmdBufLen = 0;
    QueryState queryState = QUERY_DISABLED;
    bool queryEnabled = false;
    Stream* querySerial; // serial port for query commands
    QueryDevice queryDevice = SPIFFS;

    bool logStarted = false;
    bool waitIfBufferFull = false;

    void writerTaskStart();
    static void writerTask(void* parameter);
    void outputFromBuffer();
    void buffAddLogLine(LogLineEntry& logLineEntry);
    bool mustLog(uint8_t logId, uint8_t logLevel);
    void logInternal(const uint8_t logLevel, const char* format, ...);
    void outputStats();
    void panic(const char* message);

    void queryHandleSerialInput();
    void queryProcessIncomingCmd(const char* command);

    void queryStateDisabled(char c);
    void queryStateWaitCommand(char c);
    void queryStateWaitPeekQuit(char c);

    void queryCmdHelp();
    void queryCmdSpiffs();
    void queryCmdSd();
    void queryCmdSerial();
    void queryCmdSyslog();
    void queryCmdDir(const char* directory);
    void queryCmdCd(const char* directory);
    void queryCmdRm(const char* filename);
    void queryCmdRmdir(const char* directory);
    void queryCmdFormat();
    void queryCmdType(const char* filename);
    void queryCmdPeek(const char* filename, const char* loglevel, const char* textFilter);
    void queryCmdStatus();
    void queryPrintPrompt();
};

extern Elog& logger; // Make an instance available to user when he includes the library

#endif // ELOG_H
