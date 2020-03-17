#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSpinBox>

#include <memory>

#include "lifegridscene.h"
#include "ui/resizedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionExit_triggered();

    void on_actionStep_triggered();

    void on_actionRun_toggled(bool arg1);

    void on_actionWrap_Grid_toggled(bool arg1);

    void on_actionClear_triggered();

    void on_actionTogglePaint_toggled(bool arg1);

    void on_actionResize_triggered();

    void on_resize_dialog_accepted();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<QGraphicsView> graphics_view;
    std::unique_ptr<QHBoxLayout> hbox_layout;
    std::unique_ptr<QSpinBox> speed_selector;
    std::unique_ptr<LifeGridScene> life_grid_scene;
    std::unique_ptr<ResizeDialog> resize_dialog;

    void on_speed_changed(int i);
};

#endif // MAINWINDOW_H
