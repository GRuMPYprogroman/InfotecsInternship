//
// Created by Stepan on 20.08.2025.
//

#ifndef LOGGERLIBRARY_SOCKETLOGGER_H
#define LOGGERLIBRARY_SOCKETLOGGER_H

#include <sys/socket.h>
#include "ILogger.h"

namespace InfotecsLogger {
    class SocketLogger : ILogger {
        int serverSocket = socket()
    };
}

#endif //LOGGERLIBRARY_SOCKETLOGGER_H
