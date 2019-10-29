/*************************************************
Class Name： IMMainCtrl
Description: 用于控制主菜单的类
*************************************************/
#ifndef IMMAINCTRL_H
#define IMMAINCTRL_H

#include <QObject>
#include <QMultiMap>
#include "IMConstant.h"
#include <QVector>

class IMTcpSocket;

class IMMainCtrl : public QObject
{
    Q_OBJECT
public:
    IMMainCtrl(UserInformation userInf, QObject *parent = nullptr);

    //获取卖家所有销售的商品
    void getAllSellCommodityInformation(const QString& sellName);
    // 关闭链接
    void closeConnect();
    //修改个人信息
    void updateUserInformation(const UserInformation& userInf);
    // 更新商品信息
    void updateCommodityInformation(const Commodity &commodityInf);
    //发货
    void send_Commodity(const Commodity& commodity);
    //提醒对方付款
    void remindBuyerPay(const Commodity& commodity);

public slots:
    //发送更新商品信息的请求
    void requestUpdateCommodityInf(const Commodity &commodityInf);
    //发送修改用户信息的请求
    void requestUpdateUserInf(const UserInformation& userInf);
    //发送获取卖家所有销售的商品的请求
    void requestgetAllSellCommodityInf();
    //读取信息
    void readMessage();
    // 关闭程序
    void closeWindow();
signals:
    void addToMyMessageBox(QString );
    void closeWindowSignal();
    void getAllCommoditySuccess(const QVector<Commodity> &);
    //收到了买家购买商品的通知
    void getBuyerHaveBuyCommodity(Commodity& commodityInf);
    void getBuyerHaveBuyFromCart(Commodity& commodityInf);
    void getBuyerHaveJoinShoppingCart(Commodity &commodityInf);
    void getBuyerHaveDeleteShoppingCart(Commodity &commodityInf);

private:
    QVector<Commodity> commodityVec;
    QString sellerName;
    IMTcpSocket *m_tcpSocket;
    Commodity commodityInf;
    UserInformation userInf;
    quint16 m_blockSize;
};

#endif // IMMAINCTRL_H
