#ifndef SHOPCARTBTN_H
#define SHOPCARTBTN_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class shopCartBtn;
}

class IMclient;

class shopCartBtn : public QWidget
{
    Q_OBJECT

public:
    explicit shopCartBtn(IMclient *sell,const int&, const QString &commodityNameTemp, const int &buyNumberTemp,
                         const double &totalPriceTemp, QWidget *parent = nullptr);
    ~shopCartBtn();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_clicked();

private:
    int commodityID;
    QString commodityName;
    int buyNumber;
    IMclient *sell;
    double totalPrice;
    void setButtonStatus();
    Ui::shopCartBtn *ui;
};

#endif // SHOPCARTBTN_H
