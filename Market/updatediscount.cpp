#include "updatediscount.h"
#include "sell.h"
#include "ui_updatediscount.h"

UpdateDiscount::UpdateDiscount(Sell *sellTemp, QWidget *parent) :
    QWidget(parent), m_sell(sellTemp),
    ui(new Ui::UpdateDiscount)
{
    ui->setupUi(this);
}

UpdateDiscount::~UpdateDiscount()
{
    delete ui;
}

void UpdateDiscount::on_pushButton_clicked()
{
    discount1 = ui->lineEdit->text().toInt();
    discount2 = ui->lineEdit_2->text().toInt();
    discount3 = ui->lineEdit_3->text().toInt();
    qDebug() << "discount1" << discount1 << "discount2" << discount2
             << "discount3" << discount3;
    m_sell->setDiscount(discount1, discount2, discount3);
    close();
}
