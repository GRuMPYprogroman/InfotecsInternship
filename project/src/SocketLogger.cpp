//
// Created by Stepan on 20.08.2025.
//

#include "SocketLogger.h"
#include <PortableClock.h>
#include <cstring>


namespace InfotecsLogger {
    void SocketLogger::Log(const std::string &message, std::optional<LogLevel> logLevel, std::optional<EventId> eventId)
    {
        if (!logLevel.has_value())
            logLevel.value() = _minimumLogLevel;
        
        if (IsEnabled(logLevel.value())){
            std::lock_guard<std::mutex> lock(_logMutex);

            std::string  outputMessage; 
            
            outputMessage += PortableClock::currentDateTime() + "| " + LogLevelToString(logLevel.value()) + "| ";
            
            if (eventId.has_value())
                outputMessage += eventId->Id + ": " + (eventId->Name.has_value() ? eventId->Name.value() + " " : " ");
            
            outputMessage += message + "\n";
            
            send(_clientSocket, outputMessage.c_str(), outputMessage.size(), 0);
        }
    }

    bool SocketLogger::IsEnabled(LogLevel logLevel) {
        return logLevel >= _minimumLogLevel;
    }
}