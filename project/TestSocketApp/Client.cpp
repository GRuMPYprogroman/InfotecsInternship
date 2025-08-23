#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "SocketLogger.h"
#include "AppLogEvents.h"
#include "Calculator.h"

using namespace InfotecsLogger;
using namespace LoggerUtilites;
using namespace SimpleCalculator;


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Using: " << argv[0] << " <port>n";
        return 1;
    }

    int port =std::stoi(argv[1]);
    int sock = 0;
    sockaddr_in serv_addr;
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        perror("socket creation");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Converts IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "10.0.2.15", &serv_addr.sin_addr) <= 0){
        perror("converting IP");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("server connection");
        exit(EXIT_FAILURE);
    }

    SocketLogger sockLogger = SocketLogger(LogLevel::Information, sock);

    std::string line;
    while (true) {
        std::cout << "Enter math expression or STOP: ";
        if (!std::getline(std::cin, line)) break;

        if (line == "STOP") {
            sockLogger.Log("User stopped program.", LogLevel::Information);
            break;
        }

        // Parsing loglevel
        auto [parsedLevel, shift] = LoggerUtilites::LogLevelParser::Parse(line);
        
        std::string expr = line.substr(shift);

        double result;
        if (evaluateExpression(expr, result)) {
            sockLogger.Log("Input: " + line, parsedLevel);
            std::cout << "Result: " << result << "\n";
        } 
        else {
            sockLogger.Log("Error during calculating: " + line, 
                        LogLevel::Error, 
                        AppLogEvents::CalculatorEvents::Divide);
            std::cerr << "Error: invalid expression.\n";
            break;
        }
    }

    close(sock);
    return 0;
}