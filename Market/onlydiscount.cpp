#include "onlydiscount.h"
#include "ui_onlydiscount.h"
#include "sell.h"

OnlyDiscount::OnlyDiscount(Sell *sellTemp,QWidget *parent) :
    QWidget(parent), m_sell(sellTemp),
    ui(new Ui::OnlyDiscount)
{
    ui->setupUi(this);
}

OnlyDiscount::~OnlyDiscount()
{
    delete ui;
}

void OnlyDiscount::on_pushButton_clicked()
{
    discount = ui->lineEdit->text().toInt();
    m_sell->setOnlyDiscount(discount);
    close();
}
