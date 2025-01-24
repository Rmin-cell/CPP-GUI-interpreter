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

// Parse Tree Window
class ParseTreeWindow : public QDialog {
    Q_OBJECT

public:
    explicit ParseTreeWindow(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Parse Tree");
        setWindowIcon(QIcon(":/icons/tree.png")); // Add an icon
        resize(500, 400);

        QVBoxLayout *layout = new QVBoxLayout(this);
        parseTreeDisplay = new QTextEdit(this);
        parseTreeDisplay->setReadOnly(true);
        parseTreeDisplay->setStyleSheet("QTextEdit { background-color: #f0f0f0; border: 1px solid #ccc; padding: 10px; }");
        layout->addWidget(parseTreeDisplay);
    }

    void setParseTree(const QString &parseTree) {
        parseTreeDisplay->setText(parseTree);
    }

private:
    QTextEdit *parseTreeDisplay;
};

// Plot Window (Placeholder)
class PlotWindow : public QDialog {
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Plot");
        setWindowIcon(QIcon(":/icons/chart.png")); // Add an icon
        resize(600, 400);

        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *placeholderLabel = new QLabel("Plot will be displayed here.", this);
        placeholderLabel->setAlignment(Qt::AlignCenter);
        placeholderLabel->setStyleSheet("QLabel { font-size: 18px; color: #555; }");
        layout->addWidget(placeholderLabel);
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
        mainLayout->addWidget(scrollArea);

        // Add variable button
        QPushButton *addVariableButton = new QPushButton("Add Variable", this);
        addVariableButton->setIcon(QIcon(":/icons/add.png")); // Add an icon
        connect(addVariableButton, &QPushButton::clicked, this, &EquationInterpreter::onAddVariableClicked);
        mainLayout->addWidget(addVariableButton);

        // Equation input
        QLabel *equationLabel = new QLabel("Equation (e.g., y=x^2):", this);
        equationInput = new QLineEdit(this);
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
        QLineEdit *minInput = new QLineEdit(variableWidget);
        QLineEdit *maxInput = new QLineEdit(variableWidget);
        QPushButton *clearButton = new QPushButton("Clear", variableWidget);
        clearButton->setIcon(QIcon(":/icons/clear.png")); // Add an icon
        clearButton->setStyleSheet("QPushButton { background-color: #f44336; } QPushButton:hover { background-color: #d32f2f; }");

        // Set placeholders for min and max inputs
        minInput->setPlaceholderText("Min");
        maxInput->setPlaceholderText("Max");

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
        // Validate inputs before proceeding
        if (!validateInputs()) {
            return;
        }

        // Collect variable data
        QStringList variables;
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

            variables << QString("%1: [%2, %3]").arg(name).arg(min).arg(max);
        }

        // Display equation
        QString equation = equationInput->text();

        // Open parse tree window
        ParseTreeWindow *parseTreeWindow = new ParseTreeWindow(this);
        parseTreeWindow->setParseTree("Variables:\n" + variables.join("\n") + "\n\nEquation: " + equation + "\n\nParse Tree: (Placeholder)");
        parseTreeWindow->exec();
    }

    void onPlotClicked() {
        // Validate inputs before proceeding
        if (!validateInputs()) {
            return;
        }

        // Open plot window
        PlotWindow *plotWindow = new PlotWindow(this);
        plotWindow->exec();
    }

private:
    QVBoxLayout *variableLayout;
    QLineEdit *equationInput;

    bool validateInputs() {
        bool valid = true;

        // Check if at least one variable is added
        if (variableLayout->count() == 0) {
            QMessageBox::warning(this, "Input Error", "Please add at least one variable.");
            return false;
        }

        // Validate each variable
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
                valid = false;
                minInput->setStyleSheet("border: 1px solid red;");
                maxInput->setStyleSheet("border: 1px solid red;");
            } else {
                minInput->setStyleSheet("");
                maxInput->setStyleSheet("");
            }
        }

        if (!valid) {
            QMessageBox::warning(this, "Input Error", "Please provide valid min and max values for all variables.");
            return false;
        }

        // Check if equation is provided
        if (equationInput->text().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide an equation.");
            return false;
        }

        return true;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    EquationInterpreter window;
    window.show();

    return app.exec();
}

#include "main.moc"