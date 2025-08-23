#include <gtest/gtest.h>
#include "Parser.h"
#include "Calculator.h"

using namespace Parser;
using namespace SimpleCalculator;

struct Statistics {
    size_t totalMessages = 0;
    std::map<std::string, size_t> perLevel; // information/error/critical
    std::vector<LogMessage> messages;

    size_t minLength = SIZE_MAX;
    size_t maxLength = 0;
    double avgLength = 0.0;
};

void updateStatistics(const LogMessage& log, Statistics& stats) {
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


TEST(CalculatorTest, BasicAddition){
    double result;
    bool success = evaluateExpression("2 + 3", result);
    EXPECT_TRUE(success);
    EXPECT_EQ(5.0, result);
}

TEST(CalculatorTest, DivisionByZero){
    double result;
    bool success = evaluateExpression("4 / 0", result);
    EXPECT_FALSE(success);
}

TEST(CalculatorTest, MissingOperand){
    double result;
    bool success = evaluateExpression("5 +", result); // Incomplete
    EXPECT_FALSE(success);
}

TEST(CalculatorTest, InvalidOperator){
    double result;
    bool success = evaluateExpression("2 ^ 3", result); // ^ is invalid
    EXPECT_FALSE(success);
}

TEST(CalculatorTest, FloatingPoint){
    double result;
    bool success = evaluateExpression("1.5 * 2.0", result);
    EXPECT_TRUE(success);
    EXPECT_DOUBLE_EQ(3.0, result);
}

TEST(ParserTest,ValidMessage){
    std::string msg = "2025-08-22T12:00:00| Information| Hello world";
    LogMessage log = parseMessage(msg);
    EXPECT_EQ("Information", log.level);
    EXPECT_EQ("Hello world", log.text);
    EXPECT_EQ(11, log.length);
}

TEST(StatsTest, BasicUpdate){
    Statistics localStats;
    LogMessage log = {std::chrono::system_clock::now(), "Information", "Test msg", 8};
    updateStatistics(log, localStats);

    EXPECT_EQ(1, localStats.totalMessages);
    EXPECT_EQ(1, localStats.perLevel["Information"]);
    EXPECT_EQ(8, localStats.minLength);
    EXPECT_EQ(8, localStats.maxLength);
    EXPECT_DOUBLE_EQ(8.0, localStats.avgLength);
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}