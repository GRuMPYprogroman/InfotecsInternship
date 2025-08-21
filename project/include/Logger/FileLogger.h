#ifndef LOGGERLIBRARY_FILELOGGER_H
#define LOGGERLIBRARY_FILELOGGER_H

#include <utility>
#include <mutex>

#include "ILogger.h"
using namespace LoggerUtilites;

namespace InfotecsLogger {
    class FileLogger : public ILogger {
        LogLevel _minimumLogLevel;
        std::string  _filePath;
        std::mutex _logMutex;
    public:
        explicit FileLogger(LogLevel minumumLogLevel, std::string filePath) : 
        _minimumLogLevel(minumumLogLevel),
        _filePath(std::move(filePath)){};
        
        void Log(const std::string& message, std::optional<LogLevel> logLevel, std::optional<EventId>) override;
        bool  IsEnabled(LogLevel logLevel) override;
        void ChangeLogLevel(LogLevel newLogLevel);
    };
}

#endif //LOGGERLIBRARY_FILELOGGER_H
