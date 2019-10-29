#ifndef IMPAYMONEY_H
#define IMPAYMONEY_H

#include <QDialog>
#include <QMessageBox>
namespace Ui {
class IMPayMoney;
}

class IMPayMoney : public QDialog
{
    Q_OBJECT

public:
    explicit IMPayMoney(const int &typeTemp, const double balanceTemp = 0, const double moneyTemp = 0,
                        const QString paypsdTemp = nullptr, QWidget *parent = nullptr);
    ~IMPayMoney();
    int type;
    double balance;//余额
    double money;//支付金额
    QString paypsd;//用户密码
signals:
    void paySuccess(bool, int);//支付失败
    void payFail();//支付成功

private slots:
    void on_pushButtonSurePay_clicked();

    void on_pushButtonCancelPay_clicked();

private:

    Ui::IMPayMoney *ui;
};

#endif // IMPAYMONEY_H
