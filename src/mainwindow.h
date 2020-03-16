#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QSpinBox>

#include <memory>

#include "lifegrid.h"

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

    std::unique_ptr<QSpinBox> speed_selector;
    std::unique_ptr<LifeGrid> life_grid;

    void on_speed_changed(int i);
};

#endif // MAINWINDOW_H
