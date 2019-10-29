#ifndef IMDATABASECTRL_H
#define IMDATABASECTRL_H

#include <QObject>
#include <QMap>
#include "IMConstant.h"

class QSqlQuery;
class QSqlDatabase;
class IMUser;
class QDateTime;

class IMDatabaseCtrl : public QObject
{
    Q_OBJECT
public:
    IMDatabaseCtrl(QObject *parent = 0);

    ~IMDatabaseCtrl();
    // 创建连接
    bool createConnection();
    // 关闭数据库
    void close();
    // 打开数据库
    bool isOpen();
    //判断是否为买家
    bool judgeIfBuyer(const QString &userName);

    //增加用户信息
    int addUserInformation(UserInformation &userInf);
    //添加到购物车
    int addToShoppingCart(const Commodity &commodityInf);

    //从购物车中去除
    int removeFormShoppingCart(const Commodity &commodityInf);
    //清空购物车
    int clearShopingCart(const QVector<Commodity>& commodityInfVec, const QString& userName);

    //卖家获得卖家所卖的所有商品信息
    int searchSellerSellAllCommodityInf(const QString& sellerName, QVector<Commodity> &commodityVecInf);
    //查询用户信息
    int searchUserInformation(UserInformation &userInf);
//    //查询商品信息
//    int searchCommodityInformation(Commodity &commodityInf);
    //查询所有商品信息
    int searchAllCommodityInformation(QVector<Commodity> &commodityVecInf);
//    //查询美食信息
//    int searchFoodInformation(Food &foodInf);
//    //查询数码信息
//    int searchDigitalInformation(Digital &digitalInf);

    // 更新信息用户
    int updateUserInformation(const UserInformation &userInf);
    //更新商品信息
    int updateCommodityInformation(const Commodity &commodityInf);
//    //更新服装信息
//    int updateClothInformation(const Cloth &clothInf);
//    //更新美食信息
//    int updateFoodInformation(const Food &foodInf);
//    //更新数码信息
//    int updateDigitalInformation(const Digital &digitalInf);


private:
    //创建数据库表格
    void createTable();

private:
    QSqlDatabase *m_db;

};

#endif // IMDATABASECTRL_H
