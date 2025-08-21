//
// Created by Stepan on 20.08.2025.
//

#ifndef LOGGERLIBRARY_SOCKETLOGGER_H
#define LOGGERLIBRARY_SOCKETLOGGER_H

#include <sys/socket.h>
#include "ILogger.h"
#include <mutex>

namespace InfotecsLogger {
    class SocketLogger : ILogger {
        private:
            int _clientSocket;
            LogLevel _minimumLogLevel;
            std::mutex _logMutex;
        public:
            explicit SocketLogger(LogLevel minumumLogLevel, int clientSocket) : 
                                _minimumLogLevel(minumumLogLevel),
                                _clientSocket(clientSocket){};

            virtual bool  IsEnabled(LogLevel logLevel) override;
            virtual void Log(const std::string& message, 
                            std::optional<LogLevel> logLevel = std::nullopt, 
                            std::optional<EventId> = std::nullopt) override;
    };
}

#endif //LOGGERLIBRARY_SOCKETLOGGER_H
