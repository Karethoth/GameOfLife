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

private:
    Ui::ResizeDialog *ui;
};

#endif // RESIZEDIALOG_H
