#ifndef UNTITLED_ILOGGER_H
#define UNTITLED_ILOGGER_H

#include <string>
#include <optional>
#include "LogLevel.h"
#include "../AppLogEvents/EventId.h"

using namespace LoggerUtilites;

namespace InfotecsLogger {
    class ILogger{
    public:
        virtual bool  IsEnabled(LogLevel logLevel) = 0;
        virtual void Log(const std::string& message, 
                         std::optional<LogLevel> logLevel = std::nullopt, 
                         std::optional<EventId> = std::nullopt) = 0;
        virtual ~ILogger() = default;
    };
}
#endif //UNTITLED_ILOGGER_H
