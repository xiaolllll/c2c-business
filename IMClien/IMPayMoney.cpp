#include "IMPayMoney.h"
#include "ui_impaymoney.h"

IMPayMoney::IMPayMoney(const int &typeTemp, const double balanceTemp, const double moneyTemp,
                       const QString paypsdTemp, QWidget *parent) :
    QDialog(parent), type(typeTemp),
    balance(balanceTemp),money(moneyTemp),
    paypsd(paypsdTemp), ui(new Ui::IMPayMoney)
{
    ui->setupUi(this);
}

IMPayMoney::~IMPayMoney()
{
    delete ui;
}

void IMPayMoney::on_pushButtonSurePay_clicked()
{
    QString userInput = ui->lineEditPayPsd->text();
    if(ui->lineEditPayPsd->text().isEmpty())//判断用户输入密码是否为空
    {
        QMessageBox::information(this,tr("请输入密码"),tr("输入密码不能为空"),QMessageBox::Ok);
        this->close();
    }
    else if(userInput == paypsd)
    {
        if(balance >= money)//账户余额充足
        {
            if(type == 1)//立即购买
            {
                emit paySuccess(true, type);
            }
            else//从购物车中购买
            {
                emit paySuccess(true, type);
            }
            this->close();
        }
        else
        {
            QMessageBox::information(this, tr("提示"),tr("账户余额不足"),QMessageBox::Ok);
            close();
        }
    }
    else//用户名和密码匹配
    {
        QMessageBox::information(this,tr("密码错误"),tr("请输入正确的支付密码"),QMessageBox::Ok);
        ui->lineEditPayPsd->clear();
        ui->lineEditPayPsd->setFocus();
        close();
    }
}

void IMPayMoney::on_pushButtonCancelPay_clicked()
{
    QMessageBox::information(this,tr("取消支付"),tr("取消支付"),QMessageBox::Ok);
    close();
}
