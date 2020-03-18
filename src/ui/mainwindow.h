#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../lifegridscene.h"
#include "resizedialog.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSpinBox>
#include <QLabel>

#include <memory>

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

    /*!
     * \brief Signaled when exiting
     * \details Used to make sure the automatic stepping thread is stopped before exiting
     */
    void on_actionExit_triggered();

    /*!
     * \brief Signaled when the user wants to step a generation forwards manually
     */
    void on_actionStep_triggered();

    /*!
     * \brief Signaled when the run button is toggled
     * \details If pressed, a thread will be created which will step the simulation automatically
     * \param arg1 True if the button pressed
     */
    void on_actionRun_toggled(bool arg1);

    /*!
     * \brief Signaled when grid wrapping is toggled
     * \param arg1 True if the grid should wrap around itself
     */
    void on_actionWrap_Grid_toggled(bool arg1);

    /*!
     * \brief Signaled when the clear button is triggered
     * \details Clears the entire grid
     */
    void on_actionClear_triggered();

    /*!
     * \brief Signaled when the cell painting button is toggled
     * \details If released, the user can't edit grid contents by painting cells
     * \param arg1 True if painting cells is allowed
     */
    void on_actionTogglePaint_toggled(bool arg1);

    /*!
     * \brief Signaled when the resize button is pressed
     * \details Opens up the resize dialog
     */
    void on_actionResize_triggered();

    /*!
     * \brief Signaled when the resize dialog is accepted
     * \details Adjusts the grid size as requested
     */
    void on_resize_dialog_accepted();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<QGraphicsView> graphics_view;
    std::unique_ptr<QHBoxLayout> hbox_layout;
    std::unique_ptr<QLabel> speed_selector_label;
    std::unique_ptr<QSpinBox> speed_selector;
    std::unique_ptr<LifeGridScene> life_grid_scene;
    std::unique_ptr<ResizeDialog> resize_dialog;

    void on_speed_changed(int i);
};

#endif // MAINWINDOW_H
