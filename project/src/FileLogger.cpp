//
// Created by Stepan on 18.08.2025.
//

#include <fstream>
#include "../include/Logger/FileLogger.h"
#include "../include/Utilities/PortableClock.h"

namespace InfotecsLogger {
    void FileLogger::Log(std::string& message, std::optional<LogLevel> logLevel, std::optional<EventId> eventId) {
        if (!logLevel.has_value())
            logLevel.value() = _minimumLogLevel;
        
        if (IsEnabled(logLevel.value())){
            std::lock_guard<std::mutex> lock(_logMutex);
            
            std::ofstream fileWriter;
            fileWriter.open(_filePath, std::ios::app);
            
            std::string  outputMessage; 
            
            outputMessage += PortableClock::currentDateTime() + "| " + LogLevelToString(logLevel.value()) + "| ";
            
            if (eventId.has_value())
                outputMessage += eventId->Id + ": " + (eventId->Name.has_value() ? eventId->Name.value() + " " : " ");
            
            outputMessage += message;
            
            fileWriter << outputMessage << std::endl;
            fileWriter.close();
        }
    }

    bool FileLogger::IsEnabled(LogLevel logLevel) {
        return logLevel >= _minimumLogLevel;
    }

    void FileLogger::ChangeLogLevel(LogLevel newLogLevel) {
        _minimumLogLevel = newLogLevel;
    }
}