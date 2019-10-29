#include "shopcartbtn.h"
#include "ui_shopcartbtn.h"
#include <QEvent>
#include "IMclient.h"

shopCartBtn::shopCartBtn(IMclient *sellInf,const int &IDTemp, const QString &commodityNameTemp, const int &buyNumberTemp,
                         const double &totalPriceTemp, QWidget *parent) :
    QWidget(parent), commodityID(IDTemp),
    commodityName(commodityNameTemp),
    buyNumber(buyNumberTemp), sell(sellInf),
    totalPrice(totalPriceTemp),ui(new Ui::shopCartBtn)
{
    ui->setupUi(this);
    setButtonStatus();
    setFixedSize(QSize(560, 70));
}

shopCartBtn::~shopCartBtn()
{
    delete ui;
}

void shopCartBtn::setButtonStatus()
{
    ui->label->setText(commodityName);
    ui->label_2->setNum(buyNumber);
    ui->label_3->setNum(totalPrice);
}

//点击cehckbox
void shopCartBtn::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked())
    {
        sell->addToPayCart(commodityID);
    }
    else
    {
        sell->deleteFromPayCart(commodityID);
    }
}

//点击删除
void shopCartBtn::on_pushButton_clicked()
{
    sell->deleteFromShoppingCart(commodityID);
}
