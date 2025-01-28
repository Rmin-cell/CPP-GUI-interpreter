#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <map>
#include <iomanip>

// Token types
enum TokenType { NUMBER, OPERATOR, FUNCTION, VARIABLE, LPAREN, RPAREN };

// Structure for tokens
struct Token {
    TokenType type;
    std::string value;
};

// Global vectors to simulate different windows
std::vector<std::string> inputWindow;
std::vector<std::string> processWindow;
std::vector<std::pair<double, double>> plotWindow;  // Stores (x, f(x)) pairs

// Function to check if the string is a mathematical function
bool isFunction(const std::string &str) {
    return str == "sin" || str == "cos" || str == "tan";
}

// Function to check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Simulate input window
void showInputWindow() {
    std::cout << "\n=== Input Window ===" << std::endl;
    for (const auto &line : inputWindow) {
        std::cout << line << std::endl;
    }
}

// Simulate process window
void showProcessWindow() {
    std::cout << "\n=== Process Window ===" << std::endl;
    for (const auto &line : processWindow) {
        std::cout << line << std::endl;
    }
}

// Simulate plotting window
void showPlotWindow() {
    std::cout << "\n=== Plotting Window ===" << std::endl;
    std::cout << std::setw(10) << "x" << std::setw(20) << "f(x)" << std::endl;
    for (const auto &point : plotWindow) {
        std::cout << std::setw(10) << point.first << std::setw(20) << point.second << std::endl;
    }
}

// Tokenize the input equation
std::vector<Token> tokenize(const std::string &equation) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < equation.size()) {
        if (isspace(equation[i])) {
            i++;
            continue;
        }
        if (isdigit(equation[i])) {
            std::string num;
            while (i < equation.size() && (isdigit(equation[i]) || equation[i] == '.')) {
                num += equation[i++];
            }
            tokens.push_back({NUMBER, num});
            processWindow.push_back("Tokenized NUMBER: " + num);
        } else if (isalpha(equation[i])) {
            std::string func;
            while (i < equation.size() && isalpha(equation[i])) {
                func += equation[i++];
            }
            if (isFunction(func)) {
                tokens.push_back({FUNCTION, func});
                processWindow.push_back("Tokenized FUNCTION: " + func);
            } else {
                tokens.push_back({VARIABLE, func});
                processWindow.push_back("Tokenized VARIABLE: " + func);
            }
        } else if (isOperator(equation[i])) {
            tokens.push_back({OPERATOR, std::string(1, equation[i])});
            processWindow.push_back("Tokenized OPERATOR: " + std::string(1, equation[i]));
            i++;
        } else if (equation[i] == '(') {
            tokens.push_back({LPAREN, "("});
            processWindow.push_back("Tokenized LPAREN: (");
            i++;
        } else if (equation[i] == ')') {
            tokens.push_back({RPAREN, ")"});
            processWindow.push_back("Tokenized RPAREN: )");
            i++;
        } else {
            std::cerr << "Unknown character: " << equation[i] << std::endl;
            i++;
        }
    }
    return tokens;
}

// AST (Abstract Syntax Tree) node structure
struct ASTNode {
    std::string value;
    ASTNode *left;
    ASTNode *right;

    ASTNode(const std::string &val) : value(val), left(nullptr), right(nullptr) {}
};

// Forward declaration of parsing functions
ASTNode* parseExpression(std::vector<Token>& tokens, size_t& index);

// Parse a factor (a number, variable, or function call)
ASTNode* parseFactor(std::vector<Token>& tokens, size_t& index) {
    if (tokens[index].type == NUMBER || tokens[index].type == VARIABLE) {
        return new ASTNode(tokens[index++].value);
    } else if (tokens[index].type == FUNCTION) {
        std::string funcName = tokens[index++].value;
        if (tokens[index].type != LPAREN) {
            std::cerr << "Expected '(' after function name." << std::endl;
            return nullptr;
        }
        index++;  // Skip '('
        ASTNode* arg = parseExpression(tokens, index);
        if (tokens[index].type != RPAREN) {
            std::cerr << "Expected ')' after function argument." << std::endl;
            return nullptr;
        }
        index++;  // Skip ')'
        ASTNode* node = new ASTNode(funcName);
        node->left = arg;
        return node;
    } else if (tokens[index].type == LPAREN) {
        index++;  // Skip '('
        ASTNode* node = parseExpression(tokens, index);
        if (tokens[index].type != RPAREN) {
            std::cerr << "Expected ')'." << std::endl;
            return nullptr;
        }
        index++;  // Skip ')'
        return node;
    }
    std::cerr << "Unexpected token: " << tokens[index].value << std::endl;
    return nullptr;
}

// Parse a term (handling multiplication, division, and powers)
ASTNode* parseTerm(std::vector<Token>& tokens, size_t& index) {
    ASTNode* node = parseFactor(tokens, index);
    while (index < tokens.size() && (tokens[index].value == "*" || tokens[index].value == "/" || tokens[index].value == "^")) {
        std::string op = tokens[index++].value;
        ASTNode* rightNode = parseFactor(tokens, index);
        ASTNode* newNode = new ASTNode(op);
        newNode->left = node;
        newNode->right = rightNode;
        node = newNode;
    }
    return node;
}

// Parse an expression (handling addition and subtraction)
ASTNode* parseExpression(std::vector<Token>& tokens, size_t& index) {
    ASTNode* node = parseTerm(tokens, index);
    while (index < tokens.size() && (tokens[index].value == "+" || tokens[index].value == "-")) {
        std::string op = tokens[index++].value;
        ASTNode* rightNode = parseTerm(tokens, index);
        ASTNode* newNode = new ASTNode(op);
        newNode->left = node;
        newNode->right = rightNode;
        node = newNode;
    }
    return node;
}

// Recursively delete AST
void deleteAST(ASTNode* node) {
    if (node) {
        deleteAST(node->left);
        deleteAST(node->right);
        delete node;
    }
}

// Evaluate the AST for a given x value
double evaluate(ASTNode* node, double x) {
    if (!node) return 0.0;

    if (node->value == "+") {
        return evaluate(node->left, x) + evaluate(node->right, x);
    } else if (node->value == "-") {
        return evaluate(node->left, x) - evaluate(node->right, x);
    } else if (node->value == "*") {
        return evaluate(node->left, x) * evaluate(node->right, x);
    } else if (node->value == "/") {
        return evaluate(node->left, x) / evaluate(node->right, x);
    } else if (node->value == "^") {
        return pow(evaluate(node->left, x), evaluate(node->right, x));
    } else if (node->value == "sin") {
        return sin(evaluate(node->left, x));
    } else if (node->value == "cos") {
        return cos(evaluate(node->left, x));
    } else if (node->value == "tan") {
        return tan(evaluate(node->left, x));
    } else if (node->value == "x") {
        return x;
    } else {
        return std::stod(node->value);  // It's a number
    }
}

// Main function
int main() {
    std::string equation;
    double min_x, max_x;

    // Input window
    inputWindow.push_back("Enter an equation (e.g., 3*x^2 + 2*x - 5): ");
    showInputWindow();
    std::getline(std::cin, equation);

    inputWindow.push_back("Enter the range for x (e.g., -10 10): ");
    showInputWindow();
    std::cin >> min_x >> max_x;

    std::vector<Token> tokens = tokenize(equation);

    size_t index = 0;
    ASTNode* root = parseExpression(tokens, index);

    // Generate plot values
    for (double x = min_x; x <= max_x; x += 1.0) {
        double y = evaluate(root, x);
        plotWindow.push_back({x, y});
    }

    // Show all windows
    showInputWindow();
    showProcessWindow();
    showPlotWindow();

    // Clean up
    deleteAST(root);

    return 0;
}
