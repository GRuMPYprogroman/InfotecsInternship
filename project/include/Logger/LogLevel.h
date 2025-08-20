#ifndef LOGGERLIBRARY_LOGLEVEL_H
#define LOGGERLIBRARY_LOGLEVEL_H

#include <unordered_map>
#include <algorithm>

namespace LoggerUtilites{
    enum LogLevel{Information, Error, Critical};
    
    // Allows to construct output log string
    inline std::string LogLevelToString(LogLevel level) {
        switch (level) {
            case Information: return "information";
            case Error:       return "error";
            case Critical:    return "critical";
            default:          return "unknown";
        }
    }

    // Used for parsing user input
    struct LogLevelParser {
        static const inline std::unordered_map<std::string, LogLevel> levelMap {
                {"information", LogLevel::Information},
                {"error",       LogLevel::Error},
                {"critical",    LogLevel::Critical}
        };

        // Returns a level and how many bytes to skip.
        static std::pair<LogLevel, size_t> Parse(const std::string& str) {
            // We take first word till space
            auto pos = str.find(' ');
            std::string token = (pos == std::string::npos) ? str : str.substr(0, pos);

            std::string lowered = token;
            std::transform(lowered.begin(), lowered.end(), lowered.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            auto it = levelMap.find(lowered);
            if (it != levelMap.end()) {
                // If level exist => return it's size + 1 (space)
                return {it->second, token.size() + 1};
            }

            // nothing found => default value
            return {LogLevel::Information, 0};
        }
    };
}
#endif //LOGGERLIBRARY_LOGLEVEL_H
