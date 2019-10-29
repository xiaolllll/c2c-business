#ifndef IMTCPSERVER_H
#define IMTCPSERVER_H


#include <QTcpServer>
#include "IMClientSocketCtrl.h"
#include "IMConstant.h"
#include "IMDatabaseCtrl.h"


class IMTcpServer: public QTcpServer
{
    Q_OBJECT
public:
    IMTcpServer(QObject *parent = nullptr);
    ~IMTcpServer();
public slots:
    // 客户端断开连接
    void clientDisconnected(const QString &id);
    // 要求clientSocket发送信息
    void serverSendMessage(const SaveTmpInformation &temp);


signals:
    void haveNewAddress(QString);
    void haveLeftAddress(QString);
    void haveNewID(const QString & id);
    void haveLeftID(const QString & id);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    // 当有新的连接时，会调用此函数。用于创建新的线程去控制socket通信

    void tellSellerBuyFromCart(const QString& sellName);
    //通知卖家买家购买成功
    void tellSellerBuy(const QString& sellName);
    //通知买家卖家修改的商品信息
    void tellBuyerChange();
    //通知卖家买家加入购物车的商品信息
    void tellSellerJoinShopingCart(const QString& sellName);
    //通知卖家买家将商品从购物车去除
    void tellSelleRemoveFromShoppingCart(const QString& sellName);
    //通知卖家买家将商品从购物车去除
    void sellerRemindBuyerPay(const QString& sellName);
    //通知卖家卖家退货
    void tellSellerSaleReturn(const Commodity& commodityInf);
    //通知卖家卖家发货
    void tellSellerSendCommodity(const QString& sellName);

private:
    // 临时信息
    SaveTmpInformation m_save;
    // 操作数据库
    IMDatabaseCtrl m_database;
    //利用hashmap通过name判断出socket
    QMap<QString, IMClientSocketCtrl*> m_userMap;
};

#endif // IMTCPSERVER_H
