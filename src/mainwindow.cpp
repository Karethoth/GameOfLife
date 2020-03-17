#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the canvas
    graphics_view = std::make_unique<QGraphicsView>(this);
    hbox_layout = std::make_unique<QHBoxLayout>(this);

    hbox_layout->addWidget(graphics_view.get());

    auto contentWidget = new QWidget();
    contentWidget->setLayout(hbox_layout.get());
    setCentralWidget(contentWidget);

    life_grid_scene = std::make_unique<LifeGridScene>(this);
    graphics_view->setScene(life_grid_scene.get());


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
    speed_selector->connect(speed_selector.get(), static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) {
        this->on_speed_changed(i);
    });

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

void MainWindow::on_actionStep_triggered()
{
    life_grid_scene->step_and_update();
}
