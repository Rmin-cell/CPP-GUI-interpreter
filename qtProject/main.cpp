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
#include "qcustomplot.h" // Include QCustomPlot header
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

        // Add QGraphicsView for displaying the parse tree
        graphicsView = new QGraphicsView(this);
        scene = new QGraphicsScene(this);
        graphicsView->setScene(scene);
        layout->addWidget(graphicsView);
    }

    void setParseTree(const QString &parseTree) {
        scene->clear(); // Clear any previous parse tree
        errorLabel->clear(); // Clear any previous error messages

        // Draw the parse tree
        drawParseTree(parseTree);
    }

    void setError(const QString &errorMessage) {
        errorLabel->setText(errorMessage); // Display the error message
        scene->clear(); // Clear the parse tree display
    }

private:
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QLabel *errorLabel; // Label to display errors

    void drawParseTree(const QString &parseTree) {
        scene->clear(); // Clear any previous parse tree

        QJsonDocument doc = QJsonDocument::fromJson(parseTree.toUtf8());
        if (!doc.isObject()) {
            errorLabel->setText("Error: Invalid parse tree format.");
            return;
        }

        QJsonObject rootObj = doc.object();
        drawNode(rootObj, 200, 50, nullptr);
    }

    void drawNode(const QJsonObject &node, int x, int y, QGraphicsTextItem *parent) {
    QString type = node["type"].toString();
    QString text;

    if (type == "Expression" || type == "Term") {
        text = node["operator"].toString();
    } else if (type == "Number" || type == "Variable") {
        text = node["value"].toString();
    } else if (type == "Function") {
        text = node["name"].toString();
    } else if (type == "Parentheses") {
        text = "()";
    } else {
        text = "Unknown";
    }

    QGraphicsTextItem *item = scene->addText(text);
    item->setPos(x, y);

    if (parent) {
        QPen pen(Qt::white); // Set the pen color to white
        scene->addLine(parent->x() + parent->boundingRect().width() / 2, parent->y() + parent->boundingRect().height(),
                       item->x() + item->boundingRect().width() / 2, item->y(), pen);
    }

    int xOffset = 50; // Adjust this value to decrease the horizontal distance between nodes
    int yOffset = 50; // Adjust this value to decrease the vertical distance between nodes

    if (node.contains("left")) {
        drawNode(node["left"].toObject(), x - xOffset, y + yOffset, item);
    }
    if (node.contains("right")) {
        drawNode(node["right"].toObject(), x + xOffset, y + yOffset, item);
    }
    if (node.contains("argument")) {
        drawNode(node["argument"].toObject(), x, y + yOffset, item);
    }
    if (node.contains("expression")) {
        drawNode(node["expression"].toObject(), x, y + yOffset, item);
    }
}
};

// Plot Window
class PlotWindow : public QDialog {
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Plot");
        setWindowIcon(QIcon(":/icons/chart.png")); // Add an icon
        resize(800, 600);

        QVBoxLayout *layout = new QVBoxLayout(this);

        // Add a label for error messages
        errorLabel = new QLabel(this);
        errorLabel->setStyleSheet("QLabel { color: red; font-size: 14px; }");
        errorLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(errorLabel);

        // Add QCustomPlot widget
        plot = new QCustomPlot(this);
        layout->addWidget(plot);

        // Set up the plot
        plot->xAxis->setLabel("x");
        plot->yAxis->setLabel("y");
        plot->xAxis->setRange(-10, 10); // Default x-axis range
        plot->yAxis->setRange(-10, 10); // Default y-axis range
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // Allow zooming and dragging
    }

    void plotEquation(const QString &equation, const std::map<std::string, std::pair<double, double>> &variables) {
        try {
            // Clear previous graph
            plot->clearGraphs();

            // Generate data points for the plot
            QVector<double> x(1000), y(1000);
            double minX = -10, maxX = 10; // Default x range
            if (variables.find("x") != variables.end()) {
                minX = variables.at("x").first;
                maxX = variables.at("x").second;
            }

            for (int i = 0; i < 1000; ++i) {
                x[i] = minX + (maxX - minX) * i / 999.0; // x values from minX to maxX
                y[i] = evaluateEquation(equation.toStdString(), x[i]); // Evaluate y = f(x)
            }

            // Add graph and set data
            plot->addGraph();
            plot->graph(0)->setData(x, y);
            plot->graph(0)->setPen(QPen(Qt::blue)); // Set line color
            plot->graph(0)->setName(equation); // Set legend name

            // Rescale axes and replot
            plot->rescaleAxes();
            plot->replot();

            // Clear any previous error messages
            errorLabel->clear();
        } catch (const std::exception &e) {
            errorLabel->setText("Error: " + QString::fromStdString(e.what()));
        }
    }

    void setError(const QString &errorMessage) {
        errorLabel->setText(errorMessage); // Display the error message
        plot->clearGraphs(); // Clear the plot
    }

private:
    QCustomPlot *plot;
    QLabel *errorLabel; // Label to display errors

    // Evaluate the equation for a given x value
    double evaluateEquation(const std::string &equation, double x) {
        // Replace variables with their values
        std::string expr = equation;
        size_t pos = expr.find("x");
        if (pos != std::string::npos) {
            expr.replace(pos, 1, std::to_string(x));
        }

        // Evaluate the expression (this is a placeholder; replace with your parser)
        // For now, we'll handle simple equations like y = x^2
        if (expr.find("x^2") != std::string::npos) {
            return x * x;
        } else if (expr.find("sin(x)") != std::string::npos) {
            return std::sin(x);
        } else {
            throw std::runtime_error("Unsupported equation");
        }
    }
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
        QLabel *equationLabel = new QLabel("Enter your equation below (e.g., 2*x+sin(x)):", this);
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
        //QPushButton *plotButton = new QPushButton("Show Plot", this);
        //plotButton->setIcon(QIcon(":/icons/chart.png")); // Add an icon
        //connect(plotButton, &QPushButton::clicked, this, &EquationInterpreter::onPlotClicked);
        //mainLayout->addWidget(plotButton);

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
    parseTreeWindow->setParseTree(parseTree);
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

        // Collect variable data
        std::map<std::string, std::pair<double, double>> variables;
        for (int i = 0; i < variableLayout->count(); i++) {
            QWidget *widget = variableLayout->itemAt(i)->widget();
            if (!widget) continue;

            QLineEdit *nameInput = widget->findChild<QLineEdit *>();
            QLineEdit *minInput = widget->findChild<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
            QLineEdit *maxInput = widget->findChild<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);

            if (!nameInput || !minInput || !maxInput) continue;

            QString name = nameInput->text();
            QString min = minInput->text();
            QString max = maxInput->text();

            bool minValid = false, maxValid = false;
            double minVal = min.toDouble(&minValid);
            double maxVal = max.toDouble(&maxValid);

            if (name.isEmpty() || !minValid || !maxValid || minVal >= maxVal) {
                PlotWindow *plotWindow = new PlotWindow(this);
                plotWindow->setError("Error: Invalid input values for variable " + name);
                plotWindow->exec();
                return;
            }

            variables[name.toStdString()] = {minVal, maxVal};
        }

        // Open plot window and plot the equation
        PlotWindow *plotWindow = new PlotWindow(this);
        plotWindow->plotEquation(equationInput->text(), variables);
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
        return QString::fromStdString(parseExpression(tokens, index));
    }

    // Parse an expression
    std::string parseExpression(const std::vector<Token> &tokens, size_t &index) {
        std::string left = parseTerm(tokens, index);

        while (index < tokens.size() && (tokens[index].type == TOKEN_OPERATOR && (tokens[index].value == "+" || tokens[index].value == "-"))) {
            std::string op = tokens[index].value;
            index++;
            std::string right = parseTerm(tokens, index);
            left = "{ \"type\": \"Expression\", \"left\": " + left + ", \"operator\": \"" + op + "\", \"right\": " + right + " }";
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
            left = "{ \"type\": \"Term\", \"left\": " + left + ", \"operator\": \"" + op + "\", \"right\": " + right + " }";
        }

        return left;
    }

    // Parse a factor
    std::string parseFactor(const std::vector<Token> &tokens, size_t &index) {
        if (tokens[index].type == TOKEN_NUMBER) {
            return "{ \"type\": \"Number\", \"value\": \"" + tokens[index++].value + "\" }";
        } else if (tokens[index].type == TOKEN_VARIABLE) {
            return "{ \"type\": \"Variable\", \"value\": \"" + tokens[index++].value + "\" }";
        } else if (tokens[index].type == TOKEN_FUNCTION) {
            std::string func = tokens[index++].value;
            if (tokens[index].type == TOKEN_LPAREN) {
                index++;
                std::string arg = parseExpression(tokens, index);
                if (tokens[index].type == TOKEN_RPAREN) {
                    index++;
                }
                return "{ \"type\": \"Function\", \"name\": \"" + func + "\", \"argument\": " + arg + " }";
            }
        } else if (tokens[index].type == TOKEN_LPAREN) {
            index++;
            std::string expr = parseExpression(tokens, index);
            if (tokens[index].type == TOKEN_RPAREN) {
                index++;
            }
            return "{ \"type\": \"Parentheses\", \"expression\": " + expr + " }";
        }

        return "{ \"type\": \"Unknown\" }";
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    EquationInterpreter window;
    window.show();

    return app.exec();
}

#include "main.moc"