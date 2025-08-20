//
// Created by Stepan on 18.08.2025.
//

#ifndef LOGGERLIBRARY_PORTABLECLOCK_H
#define LOGGERLIBRARY_PORTABLECLOCK_H


#include <string>
#include <string>
#include <stdio.h>
#include <time.h>

class PortableClock {
public:
    static const std::string currentDateTime() {
        time_t now = time(nullptr);
        tm  tstruct;
        char buf[80];
        tstruct = *localtime(&now);

        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    };
};


#endif //LOGGERLIBRARY_PORTABLECLOCK_H
