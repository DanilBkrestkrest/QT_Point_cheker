#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <cmath>

class FunctionChecker : public QWidget {
    Q_OBJECT

public:
    FunctionChecker(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);

        // Input fields
        auto *xLayout = new QHBoxLayout();
        xLayout->addWidget(new QLabel("x:", this));
        xInput = new QLineEdit(this);
        xLayout->addWidget(xInput);

        auto *yLayout = new QHBoxLayout();
        yLayout->addWidget(new QLabel("y:", this));
        yInput = new QLineEdit(this);
        yLayout->addWidget(yInput);

        auto *kLayout = new QHBoxLayout();
        kLayout->addWidget(new QLabel("k:", this));
        kInput = new QLineEdit(this);
        kLayout->addWidget(kInput);

        auto *bLayout = new QHBoxLayout();
        bLayout->addWidget(new QLabel("b:", this));
        bInput = new QLineEdit(this);
        bLayout->addWidget(bInput);

        // Add layouts to main layout
        layout->addLayout(xLayout);
        layout->addLayout(yLayout);
        layout->addLayout(kLayout);
        layout->addLayout(bLayout);

        // Check button
        auto *checkButton = new QPushButton("Проверить", this);
        layout->addWidget(checkButton);

        // Connect button signal
        connect(checkButton, &QPushButton::clicked, this, &FunctionChecker::checkPoint);

        // Result label
        resultLabel = new QLabel(this);
        layout->addWidget(resultLabel);

        // Graphics View for plotting
        scene = new QGraphicsScene(this);
        graphicsView = new QGraphicsView(scene, this);
        layout->addWidget(graphicsView);

        drawGraph();
    }

private slots:
    void checkPoint() {
        // Read and validate inputs
        bool okX, okY, okK, okB;
        double x = xInput->text().toDouble(&okX);
        double y = yInput->text().toDouble(&okY);
        double k = kInput->text().toDouble(&okK);
        double b = bInput->text().toDouble(&okB);

        if (!okX || !okY || !okK || !okB) {
            QMessageBox::warning(this, "Ошибка", "Введите корректные числовые значения!");
            return;
        }

        // Check functions
        bool isParabola = (y == x * x);
        bool isLine = (y == k * x + b);

        QString result;
        if (isParabola && isLine) {
            result = "Точка принадлежит обеим функциям.";
        } else if (isParabola) {
            result = "Точка принадлежит функции y = x^2.";
        } else if (isLine) {
            result = "Точка принадлежит функции y = kx + b.";
        } else {
            result = "Точка не принадлежит ни одной из функций.";
        }

        resultLabel->setText(result);

        // Add point to the graph
        drawGraph();
        drawPoint(x, y);
    }

private:
    void drawGraph() {
        scene->clear();

        // Draw axes
        QPen axisPen(Qt::black);
        axisPen.setWidth(2);
        scene->addLine(-200, 0, 200, 0, axisPen); // X-axis
        scene->addLine(0, -200, 0, 200, axisPen); // Y-axis

        // Draw parabola (y = x^2)
        QPen parabolaPen(Qt::blue);
        parabolaPen.setWidth(2);
        for (double x = -100; x <= 100; x += 0.1) {
            double y1 = x * x;
            double y2 = (x + 0.1) * (x + 0.1);
            scene->addLine(x, -y1 / 10, x + 0.1, -y2 / 10, parabolaPen);
        }

        // Draw line (y = kx + b)
        QPen linePen(Qt::red);
        linePen.setWidth(2);
        bool okK, okB;
        double k = kInput->text().toDouble(&okK);
        double b = bInput->text().toDouble(&okB);
        if (okK && okB) {
            scene->addLine(-100, -(k * -100 + b) / 10, 100, -(k * 100 + b) / 10, linePen);
        }
    }

    void drawPoint(double x, double y) {
        QPen pointPen(Qt::green);
        pointPen.setWidth(2);
        QBrush pointBrush(Qt::green);
        scene->addEllipse(x - 2, -y / 10 - 2, 4, 4, pointPen, pointBrush);
    }

    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *kInput;
    QLineEdit *bInput;
    QLabel *resultLabel;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    FunctionChecker checker;
    checker.setWindowTitle("Проверка принадлежности точки");
    checker.resize(600, 600);
    checker.show();

    return app.exec();
}

#include "main.moc"
