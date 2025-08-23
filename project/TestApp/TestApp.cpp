#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <atomic>
#include "ILogger.h"
#include "FileLogger.h"
#include "AppLogEvents.h"
#include "Calculator.h"

using namespace InfotecsLogger;
using namespace AppLogEvents;
using namespace SimpleCalculator;

struct LogTask {
    std::string message;
    std::optional<LogLevel> level = std::nullopt;
    std::optional<EventId> eventId = std::nullopt;
};

class TaskQueue {
private:
    std::queue<LogTask> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
    bool _stop{false};
public:
    void push(LogTask task) {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(std::move(task));
        _cond.notify_one();
    }

    bool pop(LogTask& task) {
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [this]() { return !_queue.empty() || _stop; });
        if (_stop && _queue.empty()) return false;
        task = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _stop = true;
        }
        _cond.notify_all();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Using: " << argv[0] << " <logfile> <default_level>\n";
        return 1;
    }

    std::string logFile = argv[1];
    std::string defaultLevelStr = argv[2];

    std::unique_ptr<ILogger> logger = std::make_unique<FileLogger>(
            LoggerUtilites::LogLevelParser::Parse(defaultLevelStr).first, 
            logFile);

    TaskQueue queue;
    std::atomic<bool> running{true};

    // Thread to write logs
    std::thread logsWriter([&]() {
        LogTask task;
        while (queue.pop(task)) {
            logger->Log(task.message, task.level, task.eventId);
        }
    });

    std::string line;
    while (running) {
        std::cout << "Enter math expression or STOP: ";
        if (!std::getline(std::cin, line)) break;

        if (line == "STOP") {
            queue.push({"User stopped program.", LogLevel::Information});
            running = false;
            break;
        }

        // Parsing loglevel
        auto [parsedLevel, shift] = LoggerUtilites::LogLevelParser::Parse(line);
        
        std::string expr = line.substr(shift);

        double result;
        if (evaluateExpression(expr, result)) {
            queue.push({"Input: " + line, parsedLevel});
            std::cout << "Result: " << result << "\n";
        } 
        else {
            queue.push({"Error during calculating: " + line, 
                        LogLevel::Error, 
                        AppLogEvents::CalculatorEvents::Divide});
            std::cerr << "Error: invalid expression.\n";
            running = false;
            break;
        }
    }

    queue.stop();
    logsWriter.join();

    return 0;
}
