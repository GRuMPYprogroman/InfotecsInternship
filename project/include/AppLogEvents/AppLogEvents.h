#ifndef LOGGERLIBRARY_STREAMEVENTS_H
#define LOGGERLIBRARY_STREAMEVENTS_H

#include "EventId.h"

namespace AppLogEvents{
    class CalculatorEvents{
    public:
        static const inline EventId Divide = EventId(1000, "Error during divide.");
    };
}

#endif //LOGGERLIBRARY_STREAMEVENTS_H
