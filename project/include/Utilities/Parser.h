#include <chrono>
#include <string>


namespace Parser{
struct LogMessage {
    std::chrono::system_clock::time_point timestamp;
    std::string level;
    std::string text;
    size_t length;
};

// parses incoming message to LogMessage struct
LogMessage parseMessage(const std::string& msg) {
    LogMessage log;
    size_t first = msg.find('|');
    size_t second = msg.find('|', first + 1);

    log.level = msg.substr(first + 2, second - (first + 2));
    log.text = msg.substr(second + 2);
    log.length = log.text.size();
    log.timestamp = std::chrono::system_clock::now();
    return log;
}
}

