#ifndef LOGGERLIBRARY_EVENTID_H
#define LOGGERLIBRARY_EVENTID_H

#include <string>
#include <optional>

struct EventId{
    int Id;
    std::optional<std::string> Name;
    
    explicit EventId(int id, std::optional<std::string> name = std::nullopt){
        Id = id;
        if (name.has_value())
            Name = name.value();
    }
};

#endif //LOGGERLIBRARY_EVENTID_H
