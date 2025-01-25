#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QTextEdit>
#include <QDialog>
#include <QSpacerItem>
#include <QIcon>
#include <stack>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>

// Parse Tree Window
class ParseTreeWindow : public QDialog {
    Q_OBJECT

public:
    explicit ParseTreeWindow(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Parse Tree");
        setWindowIcon(QIcon(":/icons/tree.png")); // Add an icon
        resize(500, 400);

        QVBoxLayout *layout = new QVBoxLayout(this);

        // Add a label for error messages
        errorLabel = new QLabel(this);
        errorLabel->setStyleSheet("QLabel { color: red; font-size: 14px; }");
        errorLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(errorLabel);

        // Add the parse tree display
        parseTreeDisplay = new QTextEdit(this);
        parseTreeDisplay->setReadOnly(true);
        parseTreeDisplay->setStyleSheet("QTextEdit { background-color:rgb(17, 16, 16); border: 1px solid #ccc; padding: 10px; }");
        layout->addWidget(parseTreeDisplay);
    }

    void setParseTree(const QString &parseTree) {
        parseTreeDisplay->setText(parseTree);
        errorLabel->clear(); // Clear any previous error messages
    }

    void setError(const QString &errorMessage) {
        errorLabel->setText(errorMessage); // Display the error message
        parseTreeDisplay->clear(); // Clear the parse tree display
    }

private:
    QTextEdit *parseTreeDisplay;
    QLabel *errorLabel; // Label to display errors
};

// Plot Window
class PlotWindow : public QDialog {
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Plot");
        setWindowIcon(QIcon(":/icons/chart.png")); // Add an icon
        resize(600, 400);

        QVBoxLayout *layout = new QVBoxLayout(this);

        // Add a label for error messages
        errorLabel = new QLabel(this);
        errorLabel->setStyleSheet("QLabel { color: red; font-size: 14px; }");
        errorLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(errorLabel);

        // Add a placeholder for the plot
        plotPlaceholder = new QLabel("Plot will be displayed here.", this);
        plotPlaceholder->setAlignment(Qt::AlignCenter);
        plotPlaceholder->setStyleSheet("QLabel { font-size: 18px; color: #555; }");
        layout->addWidget(plotPlaceholder);
    }

    void setError(const QString &errorMessage) {
        errorLabel->setText(errorMessage); // Display the error message
        plotPlaceholder->clear(); // Clear the plot placeholder
    }

private:
    QLabel *plotPlaceholder;
    QLabel *errorLabel; // Label to display errors
};

// Main Window
class EquationInterpreter : public QMainWindow {
    Q_OBJECT

public:
    EquationInterpreter(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Set window title and icon
        setWindowTitle("Equation Interpreter");
        setWindowIcon(QIcon(":/icons/app.png")); // Add an icon
        resize(800, 600);

        // Apply a global stylesheet
        setStyleSheet(
            "QMainWindow { background-color: #f5f5f5; }"
            "QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px; font-size: 14px; }"
            "QPushButton:hover { background-color: #45a049; }"
            "QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 4px; }"
            "QLineEdit:invalid { border: 1px solid red; }"
            "QLabel { font-size: 14px; color: #333; }"
            "QScrollArea { border: none; }"
        );

        // Create main layout
        QVBoxLayout *mainLayout = new QVBoxLayout();

        // Create scroll area for dynamic variables
        QScrollArea *scrollArea = new QScrollArea(this);
        QWidget *scrollContent = new QWidget(scrollArea);
        variableLayout = new QVBoxLayout(scrollContent);
        scrollArea->setWidget(scrollContent);
        scrollArea->setWidgetResizable(true);

        // Add a label to specify where to insert variables
        QLabel *variableSpecLabel = new QLabel("Add variables below (Name, Min, Max):", this);
        variableSpecLabel->setStyleSheet("QLabel { font-size: 14px; color: #555; }");
        mainLayout->addWidget(variableSpecLabel);
        mainLayout->addWidget(scrollArea);

        // Add variable button
        QPushButton *addVariableButton = new QPushButton("Add Variable", this);
        addVariableButton->setIcon(QIcon(":/icons/add.png")); // Add an icon
        connect(addVariableButton, &QPushButton::clicked, this, &EquationInterpreter::onAddVariableClicked);
        mainLayout->addWidget(addVariableButton);

        // Equation input
        QLabel *equationLabel = new QLabel("Enter your equation below (e.g., y=x^2):", this);
        equationLabel->setStyleSheet("QLabel { font-size: 14px; color: #555; }");
        equationInput = new QLineEdit(this);
        equationInput->setPlaceholderText("Enter equation here...");
        mainLayout->addWidget(equationLabel);
        mainLayout->addWidget(equationInput);

        // Parse tree button
        QPushButton *parseTreeButton = new QPushButton("Show Parse Tree", this);
        parseTreeButton->setIcon(QIcon(":/icons/tree.png")); // Add an icon
        connect(parseTreeButton, &QPushButton::clicked, this, &EquationInterpreter::onParseTreeClicked);
        mainLayout->addWidget(parseTreeButton);

        // Plot button
        QPushButton *plotButton = new QPushButton("Show Plot", this);
        plotButton->setIcon(QIcon(":/icons/chart.png")); // Add an icon
        connect(plotButton, &QPushButton::clicked, this, &EquationInterpreter::onPlotClicked);
        mainLayout->addWidget(plotButton);

        // Add a spacer to push everything up
        mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

        // Set central widget
        QWidget *centralWidget = new QWidget(this);
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);
    }

private slots:
    void onAddVariableClicked() {
        QWidget *variableWidget = new QWidget(this);
        QHBoxLayout *variableRowLayout = new QHBoxLayout(variableWidget);

        QLineEdit *nameInput = new QLineEdit(variableWidget);
        nameInput->setPlaceholderText("Variable Name");
        QLineEdit *minInput = new QLineEdit(variableWidget);
        minInput->setPlaceholderText("Min Value");
        QLineEdit *maxInput = new QLineEdit(variableWidget);
        maxInput->setPlaceholderText("Max Value");
        QPushButton *clearButton = new QPushButton("Clear", variableWidget);
        clearButton->setIcon(QIcon(":/icons/clear.png")); // Add an icon
        clearButton->setStyleSheet("QPushButton { background-color: #f44336; } QPushButton:hover { background-color: #d32f2f; }");

        // Validate min and max inputs
        auto validateInputs = [minInput, maxInput]() {
            bool minValid = false, maxValid = false;
            double minVal = minInput->text().toDouble(&minValid);
            double maxVal = maxInput->text().toDouble(&maxValid);

            if (minValid && maxValid && minVal < maxVal) {
                minInput->setStyleSheet("");
                maxInput->setStyleSheet("");
            } else {
                minInput->setStyleSheet("border: 1px solid red;");
                maxInput->setStyleSheet("border: 1px solid red;");
            }
        };

        connect(minInput, &QLineEdit::textChanged, validateInputs);
        connect(maxInput, &QLineEdit::textChanged, validateInputs);

        variableRowLayout->addWidget(new QLabel("Variable:", variableWidget));
        variableRowLayout->addWidget(nameInput);
        variableRowLayout->addWidget(new QLabel("Min:", variableWidget));
        variableRowLayout->addWidget(minInput);
        variableRowLayout->addWidget(new QLabel("Max:", variableWidget));
        variableRowLayout->addWidget(maxInput);
        variableRowLayout->addWidget(clearButton);

        connect(clearButton, &QPushButton::clicked, [variableWidget]() {
            delete variableWidget;
        });

        variableLayout->addWidget(variableWidget);
    }

    void onParseTreeClicked() {
        // Check if the equation is empty
        if (equationInput->text().isEmpty()) {
            ParseTreeWindow *parseTreeWindow = new ParseTreeWindow(this);
            parseTreeWindow->setError("Error: Please enter an equation before generating the parse tree.");
            parseTreeWindow->exec();
            return;
        }

        // Generate parse tree
        QString equation = equationInput->text();
        QString parseTree = generateParseTree(equation.toStdString());

        // Open parse tree window
        ParseTreeWindow *parseTreeWindow = new ParseTreeWindow(this);
        parseTreeWindow->setParseTree("Equation: " + equation + "\n\nParse Tree:\n" + parseTree);
        parseTreeWindow->exec();
    }

    void onPlotClicked() {
        // Check if no variables are added
        if (variableLayout->count() == 0) {
            PlotWindow *plotWindow = new PlotWindow(this);
            plotWindow->setError("Error: Please add at least one variable before plotting.");
            plotWindow->exec();
            return;
        }

        // Check if the equation is empty
        if (equationInput->text().isEmpty()) {
            PlotWindow *plotWindow = new PlotWindow(this);
            plotWindow->setError("Error: Please enter an equation before plotting.");
            plotWindow->exec();
            return;
        }

        // Open plot window
        PlotWindow *plotWindow = new PlotWindow(this);
        plotWindow->exec();
    }

private:
    QVBoxLayout *variableLayout;
    QLineEdit *equationInput;

    // Token types for the parser
    enum TokenType {
        TOKEN_NUMBER,
        TOKEN_VARIABLE,
        TOKEN_OPERATOR,
        TOKEN_FUNCTION,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_END
    };

    // Token structure
    struct Token {
        TokenType type;
        std::string value;
    };

    // Lexer to tokenize the input equation
    std::vector<Token> tokenize(const std::string &equation) {
        std::vector<Token> tokens;
        size_t i = 0;

        while (i < equation.size()) {
            char c = equation[i];

            if (isspace(c)) {
                i++;
            } else if (isdigit(c) || c == '.') {
                std::string num;
                while (i < equation.size() && (isdigit(equation[i]) || equation[i] == '.')) {
                    num += equation[i++];
                }
                tokens.push_back({TOKEN_NUMBER, num});
            } else if (isalpha(c)) {
                std::string var;
                while (i < equation.size() && isalpha(equation[i])) {
                    var += equation[i++];
                }
                if (i < equation.size() && equation[i] == '(') {
                    tokens.push_back({TOKEN_FUNCTION, var});
                } else {
                    tokens.push_back({TOKEN_VARIABLE, var});
                }
            } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                tokens.push_back({TOKEN_OPERATOR, std::string(1, c)});
                i++;
            } else if (c == '(') {
                tokens.push_back({TOKEN_LPAREN, "("});
                i++;
            } else if (c == ')') {
                tokens.push_back({TOKEN_RPAREN, ")"});
                i++;
            } else {
                i++;
            }
        }

        tokens.push_back({TOKEN_END, ""});
        return tokens;
    }

    // Recursive function to generate the parse tree
    QString generateParseTree(const std::string &equation) {
        std::vector<Token> tokens = tokenize(equation);
        size_t index = 0;
        return parseExpression(tokens, index).c_str();
    }

    // Parse an expression
    std::string parseExpression(const std::vector<Token> &tokens, size_t &index) {
        std::string left = parseTerm(tokens, index);

        while (index < tokens.size() && (tokens[index].type == TOKEN_OPERATOR && (tokens[index].value == "+" || tokens[index].value == "-"))) {
            std::string op = tokens[index].value;
            index++;
            std::string right = parseTerm(tokens, index);
            left = "Expression:\n  " + left + "\n  Operator: " + op + "\n  " + right;
        }

        return left;
    }

    // Parse a term
    std::string parseTerm(const std::vector<Token> &tokens, size_t &index) {
        std::string left = parseFactor(tokens, index);

        while (index < tokens.size() && (tokens[index].type == TOKEN_OPERATOR && (tokens[index].value == "*" || tokens[index].value == "/"))) {
            std::string op = tokens[index].value;
            index++;
            std::string right = parseFactor(tokens, index);
            left = "Term:\n  " + left + "\n  Operator: " + op + "\n  " + right;
        }

        return left;
    }

    // Parse a factor
    std::string parseFactor(const std::vector<Token> &tokens, size_t &index) {
        if (tokens[index].type == TOKEN_NUMBER) {
            return "Number: " + tokens[index++].value;
        } else if (tokens[index].type == TOKEN_VARIABLE) {
            return "Variable: " + tokens[index++].value;
        } else if (tokens[index].type == TOKEN_FUNCTION) {
            std::string func = tokens[index++].value;
            if (tokens[index].type == TOKEN_LPAREN) {
                index++;
                std::string arg = parseExpression(tokens, index);
                if (tokens[index].type == TOKEN_RPAREN) {
                    index++;
                }
                return "Function: " + func + "\n  " + arg;
            }
        } else if (tokens[index].type == TOKEN_LPAREN) {
            index++;
            std::string expr = parseExpression(tokens, index);
            if (tokens[index].type == TOKEN_RPAREN) {
                index++;
            }
            return "Parentheses:\n  " + expr;
        }

        return "Unknown";
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    EquationInterpreter window;
    window.show();

    return app.exec();
}

#include "main.moc"