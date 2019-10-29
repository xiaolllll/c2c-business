#include "myinform.h"
#include "ui_myinform.h"

myInform::myInform(QString informTemp, QWidget *parent) :
    QWidget(parent),inform(informTemp),
    ui(new Ui::myInform)
{
    ui->setupUi(this);
    setStatus();
    setFixedSize(QSize(774, 65));
}

myInform::~myInform()
{
    delete ui;
}

void myInform::setStatus()
{
    ui->label->setText(inform);
}
