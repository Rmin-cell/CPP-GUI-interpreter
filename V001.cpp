#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
#include <stdexcept>

// Function to evaluate a simple mathematical expression with one variable
double evaluateExpression(const std::string &expression, double x, double y = 0) {
    std::istringstream iss(expression);
    char op = 0;
    double result = 0, temp = 0;
    
    // Initial value (in case there's no operator at the start)
    result = (expression.find("x") != std::string::npos) ? x : y;

    while (iss >> op) {
        if (op == '+') {
            if (iss.peek() == 'x') {
                iss.ignore();
                temp = x;
            } else if (iss.peek() == 'y') {
                iss.ignore();
                temp = y;
            } else {
                iss >> temp;
            }
            result += temp;
        } else if (op == '-') {
            if (iss.peek() == 'x') {
                iss.ignore();
                temp = x;
            } else if (iss.peek() == 'y') {
                iss.ignore();
                temp = y;
            } else {
                iss >> temp;
            }
            result -= temp;
        } else if (op == '*') {
            if (iss.peek() == 'x') {
                iss.ignore();
                temp = x;
            } else if (iss.peek() == 'y') {
                iss.ignore();
                temp = y;
            } else {
                iss >> temp;
            }
            result *= temp;
        } else if (op == '/') {
            if (iss.peek() == 'x') {
                iss.ignore();
                temp = x;
            } else if (iss.peek() == 'y') {
                iss.ignore();
                temp = y;
            } else {
                iss >> temp;
            }
            if (temp == 0) throw std::runtime_error("Division by zero error");
            result /= temp;
        }
    }
    
    return result;
}

int main() {
    std::string expression;
    double x = 0, y = 0;
    char isTwoVar;

    // Get the function from user
    std::cout << "Enter a function using x and y (e.g., x*x + y/2 - 5): ";
    std::getline(std::cin, expression);
    
    // Ask if it's a two-variable function
    std::cout << "Is this a two-variable function? (y/n): ";
    std::cin >> isTwoVar;
    
    if (isTwoVar == 'y' || isTwoVar == 'Y') {
        // Get values for both x and y if it's a two-variable function
        std::cout << "Enter value for x: ";
        std::cin >> x;
        std::cout << "Enter value for y: ";
        std::cin >> y;
    } else {
        // Get value for x only if it's a single-variable function
        std::cout << "Enter value for x: ";
        std::cin >> x;
    }

    try {
        double result = evaluateExpression(expression, x, y);
        std::cout << "Result: " << result << std::endl;

        // Simple text plot
        std::cout << "Simple text plot (values for x from -10 to 10):\n";
        for (int i = -10; i <= 10; ++i) {
            double plotX = static_cast<double>(i);
            double plotY = evaluateExpression(expression, plotX, y);
            int position = static_cast<int>(plotY) + 20;
            for (int j = 0; j < position; ++j) std::cout << " ";
            std::cout << "*" << std::endl;
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
