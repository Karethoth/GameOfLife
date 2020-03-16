#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the canvas
    graphics_view = std::make_unique<QGraphicsView>(this);
    auto hbox = new QHBoxLayout(this);

    hbox->addWidget(graphics_view.get());

    auto contentWidget = new QWidget();
    contentWidget->setLayout(hbox);
    setCentralWidget(contentWidget);


    // Ensure painting is toggled on by default
    for(auto action : ui->mainToolBar->actions())
    {
        if (action->objectName() == "actionTogglePaint")
        {
            action->setChecked(true);
        }
    }


    // Create the speed selector with the initial value and connect it back
    // and connect it back
    speed_selector = std::make_unique<QSpinBox>();
    speed_selector->setRange(0, 100);
    speed_selector->setValue(1);
    speed_selector->connect(speed_selector.get(), QOverload<int>::of(&QSpinBox::valueChanged), [=](int i) {
        this->on_speed_changed(i);
    });

    auto brush = QBrush(QColor(255,0,0,127),Qt::BrushStyle::SolidPattern);

    life_grid = std::make_unique<LifeGrid>(this);
    graphics_view->setScene(life_grid.get());
    ui->mainToolBar->addWidget(speed_selector.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_speed_changed(int i)
{
    QString str = QString::number(i);
}
