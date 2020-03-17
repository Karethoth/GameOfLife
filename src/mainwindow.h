#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSpinBox>

#include <memory>

#include "lifegridscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<QGraphicsView> graphics_view;
    std::unique_ptr<QHBoxLayout> hbox_layout;
    std::unique_ptr<QSpinBox> speed_selector;
    std::unique_ptr<LifeGridScene> life_grid_scene;

    void on_speed_changed(int i);
};

#endif // MAINWINDOW_H
