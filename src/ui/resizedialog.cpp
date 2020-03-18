#include "resizedialog.h"
#include "ui_resizedialog.h"

ResizeDialog::ResizeDialog(QWidget *parent, int width, int height) :
    QDialog(parent),
    ui(new Ui::ResizeDialog)
{
    ui->setupUi(this);

    new_width = width;
    new_height = height;

    is_grid_size_n_n_constrained = width == height;

    auto *width_input     = this->findChild<QObject*>({"new_width"});
    auto *height_input    = this->findChild<QObject*>({"new_height"});
    auto *lock_n_n_button = this->findChild<QObject*>({"lock_n_n"});

    if(width_input)
    {
        QMetaObject::invokeMethod(width_input, "setValue", Q_ARG(int, width));
    }

    if(height_input)
    {
        QMetaObject::invokeMethod(height_input, "setValue", Q_ARG(int, height));
    }
    if(lock_n_n_button)
    {
        QMetaObject::invokeMethod(lock_n_n_button, "setChecked", Q_ARG(bool, is_grid_size_n_n_constrained));
    }
}

ResizeDialog::~ResizeDialog()
{
    delete ui;
}

void ResizeDialog::on_new_width_valueChanged(int arg1)
{
    new_width = arg1;
    if(is_grid_size_n_n_constrained)
    {
        auto *height_input = this->findChild<QObject*>({"new_height"});
        if(height_input)
        {
            QMetaObject::invokeMethod(height_input, "setValue", Q_ARG(int, arg1));
        }
    }
}

void ResizeDialog::on_new_height_valueChanged(int arg1)
{
    new_height = arg1;
    if(is_grid_size_n_n_constrained)
    {
        auto *width_input = this->findChild<QObject*>({"new_width"});
        if(width_input)
        {
            QMetaObject::invokeMethod(width_input, "setValue", Q_ARG(int, arg1));
        }
    }
}

void ResizeDialog::on_lock_n_n_toggled(bool checked)
{
    is_grid_size_n_n_constrained = checked;
}
