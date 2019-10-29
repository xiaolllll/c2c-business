#include "MessageBox.h"
#include "ui_MessageBox.h"

m_MessageBox::m_MessageBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);
}

m_MessageBox::~m_MessageBox()
{
    delete ui;
}

void m_MessageBox::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked())
    {

    }
    else
    {

    }
}
