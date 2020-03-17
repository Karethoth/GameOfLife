#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeDialog;
}

class ResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeDialog(QWidget *parent, int width, int height);
    ~ResizeDialog();

    int new_width;
    int new_height;

private slots:
    void on_new_width_valueChanged(int arg1);
    void on_new_height_valueChanged(int arg1);

    void on_lock_n_n_toggled(bool checked);

private:
    Ui::ResizeDialog *ui;

    bool is_grid_size_n_n_constrained;
};

#endif // RESIZEDIALOG_H
