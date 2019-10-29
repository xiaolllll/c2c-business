#ifndef SELL_H
#define SELL_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStackedWidget>
#include <QtSql>
#include <QSqlQuery>
#include <QFile>
#include <QtXml\QtXml>
#include <QDialog>
#include <QMap>
#include <QtXml\QDomDocument>
#include "IMCommodityListWidget.h"
#include "IMMainCtrl.h"

class Commoditybutton;

namespace Ui {
class Sell;
}

class Sell : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sell(const UserInformation me, QWidget *parent = nullptr);
    ~Sell();
    UserInformation userInf;
    Commodity commodityInf;
    QVector<Commodity> commodityVec;//卖家所卖的商品列表
    enum DateTimeType{Time, Date, DateTime};//枚举变量获取当前时间和日期

    void getCommodityVector();//获取卖家所卖商品的列表
    //获得具体的商品信息并显示
    void getCommodityInformation(const int& ID);
    void setDiscount(int, int, int);
    void setOnlyDiscount(int);

private slots:
    void addToMyInformBox(QString);
    //关闭窗口
    void closeWindow();

    //收到买家已经将商品加入购物车的通知
    void receiveBuyerHaveJoinShoopingCart(Commodity& commodityInformation);
    void receiveBuyerHaveDeleteShoopingCart(Commodity& commodityInformation);
    //收到买家已经购买商品的通知
    void receiveBuyerHaveBuy(Commodity& commodityInformation);

    void receiveBuyerHaveBuyFromCart(Commodity& commodityInformation);

    void on_buttonReadInform_clicked();

    void on_buttonCorrectInform_clicked();

    void on_buttonSellRecord_clicked();

    void on_buttonSellManage_clicked();

    void on_buttonQiut_clicked();

    bool creatXml();

    void on_send_clicked();

    void on_remindPay_clicked();

    void setCommodityVec(const QVector<Commodity> &);//将从服务器中得到的数据设置商品列表

    void on_btnCorrectCommodityINform_clicked();

    void on_sureChangeUserInfBtn_clicked();

    void on_cancelChangeUserInfBtn_clicked();

    void on_payData_currentIndexChanged(const QString &arg1);

    void on_sendData_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:

    QString fileName;
    double satisfyPrice;
    double decreasePrice;
    int discount1;
    int discount2;
    int discount3;
    QVector<QString> myMessageBox;
    IMMainCtrl *m_mainCtrl;
    QDomDocument doc;
    //显示个人信息
    void showUserInformation();
    void initIMMainWidget();//初始化
    //xml按日期和时间存储了所出售商品的信息
    QString getDateTime(DateTimeType type);//获取时间
    bool docRead();
    bool docWrite();
    void writeXml();
    void creatNodes(QDomElement &date);
    void showDailyList();//显示每日销售商品列表
    //商品ID和商品对应的map
    QMap<int, Commoditybutton*> m_commodityMap;
    QMap<int, Commodity*> commodityInformationMap;
    //需要发货的商品列表
    QVector<Commodity> sendVec;
    //需要提醒买家付款的商品列表
    QVector<Commodity> remindBuyerPayVec;
    QVBoxLayout *commodityLayout;
    void on_actionReadInform_clicked();//查看信息
    void on_actionCorrectInform_clicked();//修改信息
    void on_actionSellRecord_clicked();//销售记录
    void on_actionManage_clicked();//销售管理
    void on_actionQuit_clicked();//退出
    void showCouponToUI();
    void showDiscountToUI();
    void transferUIToCoupon();
    void transferUIToDiscount();
    Ui::Sell *ui;
};

#endif // SELL_H
