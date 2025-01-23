#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>
#include <cmath>

class PlotWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(400, 300);
    }

    void setEquation(const QString &equation, double xMin, double xMax) {
        this->equation = equation;
        this->xMin = xMin;
        this->xMax = xMax;
        update(); // Trigger repaint
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        if (equation.isEmpty())
            return;

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);

        // Draw axes
        painter.setPen(Qt::black);
        int centerX = width() / 2;
        int centerY = height() / 2;
        painter.drawLine(0, centerY, width(), centerY); // X-axis
        painter.drawLine(centerX, 0, centerX, height()); // Y-axis

        // Plot equation
        painter.setPen(Qt::blue);
        QPolygonF points;

        double step = (xMax - xMin) / 100.0;
        for (double x = xMin; x <= xMax; x += step) {
            double y = evaluateEquation(x);
            points << QPointF(centerX + x * 20, centerY - y * 20);
        }

        painter.drawPolyline(points);
    }

private:
    QString equation;
    double xMin = -10, xMax = 10;

    double evaluateEquation(double x) {
        // For simplicity, handle only y = x^2 as an example.
        // You can extend this with a proper math parser.
        if (equation.trimmed() == "y=x^2") {
            return x * x;
        } else {
            return 0; // Default if unsupported
        }
    }
};

class EquationInterpreter : public QMainWindow {
    Q_OBJECT

public:
    EquationInterpreter(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Set window title and size
        setWindowTitle("Equation Interpreter");
        resize(600, 500);

        // Create labels
        QLabel *xLabel = new QLabel("X Min:", this);
        QLabel *xMaxLabel = new QLabel("X Max:", this);
        QLabel *equationLabel = new QLabel("Equation (e.g., y=x^2):", this);

        // Create input fields
        xMinInput = new QLineEdit(this);
        xMaxInput = new QLineEdit(this);
        equationInput = new QLineEdit(this);

        // Create buttons
        QPushButton *plotButton = new QPushButton("Plot", this);
        QPushButton *clearButton = new QPushButton("Clear", this);

        // Connect buttons to actions
        connect(plotButton, &QPushButton::clicked, this, &EquationInterpreter::onPlotClicked);
        connect(clearButton, &QPushButton::clicked, this, &EquationInterpreter::onClearClicked);

        // Create layout
        QVBoxLayout *mainLayout = new QVBoxLayout();
        QHBoxLayout *xLayout = new QHBoxLayout();
        QHBoxLayout *xMaxLayout = new QHBoxLayout();
        QHBoxLayout *equationLayout = new QHBoxLayout();
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        // Add widgets to layouts
        xLayout->addWidget(xLabel);
        xLayout->addWidget(xMinInput);

        xMaxLayout->addWidget(xMaxLabel);
        xMaxLayout->addWidget(xMaxInput);

        equationLayout->addWidget(equationLabel);
        equationLayout->addWidget(equationInput);

        buttonLayout->addWidget(plotButton);
        buttonLayout->addWidget(clearButton);

        mainLayout->addLayout(xLayout);
        mainLayout->addLayout(xMaxLayout);
        mainLayout->addLayout(equationLayout);
        mainLayout->addLayout(buttonLayout);

        // Add plot widget
        plotWidget = new PlotWidget(this);
        mainLayout->addWidget(plotWidget);

        // Set central widget
        QWidget *centralWidget = new QWidget(this);
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);
    }

private slots:
    void onPlotClicked() {
        bool xMinOk, xMaxOk;
        double xMin = xMinInput->text().toDouble(&xMinOk);
        double xMax = xMaxInput->text().toDouble(&xMaxOk);
        QString equation = equationInput->text();

        if (!xMinOk || !xMaxOk || equation.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please provide valid X range and an equation.");
            return;
        }

        if (xMin >= xMax) {
            QMessageBox::warning(this, "Input Error", "X Min must be less than X Max.");
            return;
        }

        plotWidget->setEquation(equation, xMin, xMax);
    }

    void onClearClicked() {
        xMinInput->clear();
        xMaxInput->clear();
        equationInput->clear();
        plotWidget->setEquation("", 0, 0);
    }

private:
    QLineEdit *xMinInput;
    QLineEdit *xMaxInput;
    QLineEdit *equationInput;
    PlotWidget *plotWidget;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    EquationInterpreter window;
    window.show();

    return app.exec();
}

#include "main.moc"
