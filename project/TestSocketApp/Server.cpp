#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <chrono>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>

struct LogMessage {
    std::chrono::system_clock::time_point timestamp;
    std::string level;
    std::string text;
    size_t length;
};

struct Statistics {
    size_t totalMessages = 0;
    std::map<std::string, size_t> perLevel; // information/error/critical
    std::vector<LogMessage> messages;

    size_t minLength = SIZE_MAX;
    size_t maxLength = 0;
    double avgLength = 0.0;
};

// --- Global variables ---
Statistics stats;
std::mutex statsMutex;
int N_MESSAGES = 0;
int T_SECONDS = 0;
int countSinceLastPrint = 0;
std::chrono::system_clock::time_point lastPrint;

// --- Functions ---
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

void updateStatistics(const LogMessage& log) {
    std::lock_guard<std::mutex> lock(statsMutex);

    stats.totalMessages++;
    stats.perLevel[log.level]++;
    stats.messages.push_back(log);

    stats.minLength = std::min(stats.minLength, log.length);
    stats.maxLength = std::max(stats.maxLength, log.length);
    stats.avgLength = ((stats.avgLength * (stats.totalMessages - 1)) + log.length)
                      / stats.totalMessages;

    // remove messages older than an hour
    auto now = std::chrono::system_clock::now();
    stats.messages.erase(
        std::remove_if(stats.messages.begin(), stats.messages.end(),
                       [&](const LogMessage& m) { return now - m.timestamp > std::chrono::hours(1); }),
        stats.messages.end());
}

void printStatistics() {
    std::lock_guard<std::mutex> lock(statsMutex);

    std::cout << "=== Statistics ===\n";
    std::cout << "Total: " << stats.totalMessages << "\n";
    for (auto& [level, count] : stats.perLevel)
        std::cout << level << ": " << count << "\n";
    std::cout << "Last hour: " << stats.messages.size() << "\n";
    std::cout << "Min len: " << (stats.minLength == SIZE_MAX ? 0 : stats.minLength)
              << " Max len: " << stats.maxLength
              << " Avg len: " << stats.avgLength << "\n";
    std::cout << "==================\n";
}

// --- Client handler ---
void handleClient(int clientSocket) {
    char buffer[1024];
    std::string leftover;

    while (true) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) break; // client disconnected

        leftover.append(buffer, bytesRead);

        size_t pos;
        while ((pos = leftover.find('\n')) != std::string::npos) {
            std::string msg = leftover.substr(0, pos);
            leftover.erase(0, pos + 1);

            if (msg.empty()) continue;

            LogMessage log = parseMessage(msg);
            std::cout << "LOG: " << msg << "\n";

            updateStatistics(log);

            // Count & timing for printing
            bool shouldPrint = false;
            {
                std::lock_guard<std::mutex> lock(statsMutex);
                countSinceLastPrint++;
                auto now = std::chrono::system_clock::now();
                if (countSinceLastPrint >= N_MESSAGES || now - lastPrint >= std::chrono::seconds(T_SECONDS)) {
                    shouldPrint = true;
                    countSinceLastPrint = 0;
                    lastPrint = now;
                }
            }

            if (shouldPrint) printStatistics();
        }
    }

    close(clientSocket);
}

// --- Main ---
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Using: " << argv[0] << " <port> <N_messages> <T_seconds>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);
    N_MESSAGES = std::stoi(argv[2]);
    T_SECONDS = std::stoi(argv[3]);
    lastPrint = std::chrono::system_clock::now();

    int server_fd;
    sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) { perror("bind"); exit(EXIT_FAILURE); }
    if (listen(server_fd, 5) < 0) { perror("listen"); exit(EXIT_FAILURE); }

    std::cout << "Server listening on port " << port << "...\n";

    while (true) {
        int clientSocket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);
        if (clientSocket < 0) { perror("accept"); continue; }

        std::thread(handleClient, clientSocket).detach();
    }

    close(server_fd);
    return 0;
}
