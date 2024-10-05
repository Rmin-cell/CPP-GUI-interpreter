#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <map>

// Token types
enum TokenType { NUMBER, OPERATOR, FUNCTION, VARIABLE, LPAREN, RPAREN };

// Structure for tokens
struct Token {
    TokenType type;
    std::string value;
};

// Function to check if the string is a mathematical function
bool isFunction(const std::string &str) {
    return str == "sin" || str == "cos" || str == "tan";
}

// Function to check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
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
        } else if (isalpha(equation[i])) {
            std::string func;
            while (i < equation.size() && isalpha(equation[i])) {
                func += equation[i++];
            }
            if (isFunction(func)) {
                tokens.push_back({FUNCTION, func});
            } else {
                tokens.push_back({VARIABLE, func});
            }
        } else if (isOperator(equation[i])) {
            tokens.push_back({OPERATOR, std::string(1, equation[i])});
            i++;
        } else if (equation[i] == '(') {
            tokens.push_back({LPAREN, "("});
            i++;
        } else if (equation[i] == ')') {
            tokens.push_back({RPAREN, ")"});
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

// Simulate plotting by evaluating the function for different x values
void simulatePlot(ASTNode* ast, double x_min, double x_max, int points = 100) {
    double step = (x_max - x_min) / points;
    for (double x = x_min; x <= x_max; x += step) {
        double y = evaluate(ast, x);
        std::cout << "f(" << x << ") = " << y << std::endl;
    }
}

// Main function
int main() {
    std::string equation;
    double x_min, x_max;

    std::cout << "Enter a mathematical function of x (e.g., sin(x) or x^2 + 3*x): ";
    std::getline(std::cin, equation);

    std::cout << "Enter the minimum value of x: ";
    std::cin >> x_min;

    std::cout << "Enter the maximum value of x: ";
    std::cin >> x_max;

    std::vector<Token> tokens = tokenize(equation);
    size_t index = 0;
    ASTNode* ast = parseExpression(tokens, index);

    if (ast) {
        simulatePlot(ast, x_min, x_max);
        deleteAST(ast);
    } else {
        std::cerr << "Failed to parse the equation." << std::endl;
    }

    return 0;
}
