#include <sstream>

// Simple calculator

namespace SimpleCalculator{
bool evaluateExpression(const std::string& expr, double& result) {
    std::istringstream iss(expr);
    double a, b;
    char op;
    if (!(iss >> a)) return false;
    if (!(iss >> op)) return false;
    if (!(iss >> b)) return false;

    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (b == 0) return false;
            result = a / b;
            break;
        default: return false;
    }
    return true;
}
}

