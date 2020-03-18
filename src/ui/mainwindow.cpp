#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resizedialog.h"

#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Don't allow moving or hiding the main toolbar
    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setMovable(false);
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    // Create the canvas
    graphics_view = std::make_unique<QGraphicsView>(this);
    hbox_layout = std::make_unique<QHBoxLayout>(this);
    hbox_layout->addWidget(graphics_view.get());

    auto contentWidget = new QWidget();
    contentWidget->setLayout(hbox_layout.get());
    setCentralWidget(contentWidget);

    life_grid_scene = std::make_unique<LifeGridScene>(this);
    graphics_view->setScene(life_grid_scene.get());


    // Create the speed selector with the initial value and connect it back

    // First a label for it
    ui->mainToolBar->addSeparator();
    speed_selector_label = std::make_unique<QLabel>(ui->mainToolBar);
    speed_selector_label->setText("Max iterations/s: ");
    ui->mainToolBar->addWidget(speed_selector_label.get());

    // And then the selector itself
    speed_selector = std::make_unique<QSpinBox>();
    speed_selector->setRange(1, 100);
    speed_selector->setValue(1);
    speed_selector->connect(speed_selector.get(), static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int i) {
        this->on_speed_changed(i);
    });

    ui->mainToolBar->addWidget(speed_selector.get());

    // Finally, ensure painting is toggled on by default
    for(auto action : ui->mainToolBar->actions())
    {
        const auto name = action->objectName();
        if (action->objectName() == "actionTogglePaint")
        {
            action->setChecked(true);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    life_grid_scene->stop_and_wait_for_thread();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_speed_changed(int i)
{
    life_grid_scene->set_speed(i);
}

void MainWindow::on_actionStep_triggered()
{
    life_grid_scene->next_generation();
    life_grid_scene->update();
}

void MainWindow::on_actionRun_toggled(bool arg1)
{
    life_grid_scene->run(arg1);
}

void MainWindow::on_actionWrap_Grid_toggled(bool arg1)
{
    life_grid_scene->set_wrap_grid(arg1);
}

void MainWindow::on_actionClear_triggered()
{
    life_grid_scene->clear_grid();
    life_grid_scene->update();
}

void MainWindow::on_actionTogglePaint_toggled(bool arg1)
{
    life_grid_scene->toggle_painting_enabled(arg1);
}

void MainWindow::on_actionResize_triggered()
{
    resize_dialog = std::make_unique<ResizeDialog>(
        this,
        life_grid_scene->get_grid_width(),
        life_grid_scene->get_grid_height()
    );

    QObject::connect(resize_dialog.get(), &QDialog::accepted, this, &MainWindow::on_resize_dialog_accepted);

    resize_dialog->show();
}

void MainWindow::on_resize_dialog_accepted()
{
    life_grid_scene->resize_grid(
        resize_dialog->new_width,
        resize_dialog->new_height
    );
    life_grid_scene->update();
}
