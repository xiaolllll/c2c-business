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
    void getAllCommodityInf();//获取全部商品
    void updateUserInformation(const UserInformation& userInf);//更新用户信息
    void buyerBuyCommodity(const UserInformation &userInf, const Commodity& commodityInformation);//买家点击购买商品信息
    void buyerClearCart(const UserInformation &userInf, const QVector<Commodity>& commodityInformation);//买家点击购买商品信息
    void buyerJoinShoppingCart(const Commodity& commodityInformation);//买家将商品加入购物车
    void closeConnect();
    void buyerSaleReturn(const Commodity& commodityInformation);//买家退货
    void deleteFromShopCart(const Commodity& commodityInformation);
signals:
    void addToMyMessageBox(QString );
    void getAllCommoditySuccess(QVector<Commodity> &);
    void closeWindowSignal();
    void sellerChangeCommodityInf(Commodity);
private slots:
    //读取信息
    void readMessage();
    // 关闭程序
    void closeWindow();
    //发送所有销售的商品的请求
    void requestgetAllSellCommodityInf();
private:
    QVector<Commodity> commodityVec;
    IMTcpSocket *m_tcpSocket;
    Commodity commodityInf;
    UserInformation userInf;
    quint16 m_blockSize;
    //发送买家将商品加入购物车的请求
    void requestBuyerJoinShoppingCart(const Commodity& commodityInforamtion);
    //发送买家购买商品的请求
    void requestBuyerBuyCommodityInf(const UserInformation &userInf, const Commodity& commodityInformation);
    //发送买家清空支付车的请求
    void requestBuyerClearPayCart(const UserInformation &userInf, const QVector<Commodity>& commodityInfVec);

    //发送修改用户信息的请求
    void requestUpdateUserInf(const UserInformation& userInf);
    //发送买家退货的请求
    void requestSaleReturn(const Commodity &commodityInformation);

};

#endif // IMMAINCTRL_H
