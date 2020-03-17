#include "resizedialog.h"
#include "ui_resizedialog.h"

ResizeDialog::ResizeDialog(QWidget *parent, int width, int height) :
    QDialog(parent),
    ui(new Ui::ResizeDialog)
{
    ui->setupUi(this);

    new_width = width;
    new_height = height;

    for(auto &child : this->children())
    {
        if(child->objectName() == "new_width")
        {
            QMetaObject::invokeMethod(child, "setValue", Q_ARG(int, width));
        }
        else if(child->objectName() == "new_height")
        {
            QMetaObject::invokeMethod(child, "setValue", Q_ARG(int, height));
        }
    }
}

ResizeDialog::~ResizeDialog()
{
    delete ui;
}

void ResizeDialog::on_new_width_valueChanged(int arg1)
{
    new_width = arg1;
}

void ResizeDialog::on_new_height_valueChanged(int arg1)
{
    new_height = arg1;
}
