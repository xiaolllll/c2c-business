#include "IMDatabaseCtrl.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <qdebug.h>
#include <QSqlError>
#include <QTextDocument>

//增加语句
static const QString ADD_BUYER_IN_USERINFORMATION_SQL =
        "insert into user values(?,?,?,?,?,?,?)";
static const QString ADD_SELLER_IN_USERINFORMATION_SQL =
        "insert into user values(?,?,?,?,?,?,?)";
static const QString ADD_COMMODITY_IN_SHOPPINGCATR_SQL =
        "insert into shoppingcart values(?,?)";

//删除语句
static const QString DELETE_COMMODITY_IN_SHOPPINGCATR_SQL =
        "delete from shoppingcart where "
        "commodityID = ? and userName  = ?";
static const QString CLEAR_COMMODITY_IN_SHOPPINGCATR_SQL =
        "delete from ShoppingCart where "
        "userName = ?";

//查询语句
static const QString SEARCH_COMMODITYVEC_IN_USER_INFORMATION_SQL =
        "select * from commodity where sellName = ?";
static const QString SEARCH_USER_IN_USER_INFORMATION_ID_SQL =
        "select * from user where userName = ?";
static const QString SEARCH_COMMODITY_IN_COMMODITY_INFORMATION_SQL =
        "select * from commodity";
static const QString SEARCH_USERTYPR_IN_USER_INFORMATION_SQL=
        "select * from user where userName = ?";

static const QString SEARCH_USER_IN_CLOTH_INFORMATION_ID_SQL =
        "select * from Commodity where clothID = ?";
static const QString SEARCH_USER_IN_FOOD_INFORMATION_ID_SQL =
        "select * from Commodity where foodID = ?";
static const QString SEARCH_USER_IN_DIGITAL_INFORMATION_ID_SQL =
        "select * from User where digitalID = ?";

//更新语句
static const QString UPDATE_USER_INFORMATION_SQL =
        "update user set "
        "logInPassWord = ?, address = ?, balance = ? where userName = ?;";
static const QString UPDATE_COMMODITY_INFORMATION_SQL =
        "update commodity set "
        "originalCost = ?, couponVec = ?, discount = ?, leftNumber = ? where commodityID = ?;";
static const QString UPDATE_USERINFORMATION_SQL =
        "update UserInformation set "
        "head = ?, nickname = ?, sex = ?, birthday = ?, "
        "name = ?, phone = ?, address = ?  where userID = ?;";

/**
 * @brief IMDatabaseCtrl::IMDatabaseCtrl
 * @param parent
 * update commodity set leftNumber = 99 where commodityID = 1;
 */

IMDatabaseCtrl::IMDatabaseCtrl(QObject *parent) :
    QObject(parent)
{
    m_db = new QSqlDatabase;
}

IMDatabaseCtrl::~IMDatabaseCtrl()
{
    if (m_db != nullptr)
    {
        if (m_db->isOpen())
            m_db->close();
        delete m_db;
        m_db = nullptr;
    }
}

/*************************************************
Function Name： createConnection()
Description: 创建连接
*************************************************/
bool IMDatabaseCtrl::createConnection()
{
    if (m_db == nullptr)
        m_db = new QSqlDatabase;
    if (m_db->isOpen())
    {
        return true;
    }
    // 如果已经打开
    if (QSqlDatabase::contains("business"))
    {
        *m_db = QSqlDatabase::database("business");
    }
    else
    {// 如果没有，就添加
        *m_db = QSqlDatabase::addDatabase("QMYSQL", "business");
        m_db->setDatabaseName("business");
    }

    m_db->setUserName("root");
    m_db->setPassword("1234");
    m_db->setHostName(QLatin1String("localhost"));
    if (!m_db->open())
    {
        return false;
    }
    return true;
}

/*************************************************
Function Name： isOpen()
Description: 是否连接着
*************************************************/
bool IMDatabaseCtrl::isOpen()
{
    return (m_db->isOpen());
}

/*************************************************
Function Name： close()
Description: 关闭数据库
*************************************************/
void IMDatabaseCtrl::close()
{
    if (m_db != nullptr && m_db->isOpen())
        m_db->close();
}

/*************************************************
Function Name： close()
Description: 判断是否为买家
*************************************************/
bool IMDatabaseCtrl::judgeIfBuyer(const QString &userName)
{
    if (!createConnection())
    {
        return false;
    }
    qDebug() << "userName";
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(SEARCH_USERTYPR_IN_USER_INFORMATION_SQL);
    query.addBindValue(userName);
    query.exec();
    QSqlDatabase::database().commit();
    if (!query.isActive())
    {
        qDebug() << "query.is not Active()";
        m_db->close();
        return false;
    }
    qDebug() << "query.isActive()";
    if (query.size() <= 0)
    {
        m_db->close();
        return false;
    }
    if (query.next())
    {
         int userType = query.value(3).toInt();
         qDebug() << "uesrType" << userType;
         if(userType == 1)//1代表买家
         {
             qDebug() << "true is  buyer";
             return true;
         }
         else
         {
             qDebug() << "false is not buyer";
            return false;
         }
    }
    return false;
}


/********************************增加信息模块*******************************/

/*************************************************
Function Name： addUserInformation()
Description: 增加用户信息
*************************************************/
int IMDatabaseCtrl::addUserInformation(UserInformation &userInf)
{
    qDebug() << "database : user register!";
    if (!createConnection())
    {
        return REGISTER_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    if(userInf.userType == 1 || userInf.userType == 0)//代表买家
    {
        query.prepare(ADD_BUYER_IN_USERINFORMATION_SQL);
        query.addBindValue(userInf.userName);
        query.addBindValue(userInf.logInPassWord);
        query.addBindValue(userInf.address);
        query.addBindValue(userInf.userType);
        query.addBindValue(userInf.image);
        query.addBindValue(userInf.buyer.balance);
        query.addBindValue(userInf.buyer.payPassWord);
    }
    query.exec();
    QSqlDatabase::database().commit();
    if (!query.isActive())
    {
        m_db->close();
        return REGISTER_FAIL;
    }
    return REGISTER_SUCCESS;
}

/*************************************************
Function Name： addToShoppingCart()
Description: 加入到购物车
*************************************************/
int IMDatabaseCtrl::addToShoppingCart(const Commodity &commodityInf)
{
    if (!createConnection())
    {
        return JOIN_SHOPPING_CART_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(ADD_COMMODITY_IN_SHOPPINGCATR_SQL);
    query.addBindValue(commodityInf.commodityID);
    query.addBindValue(commodityInf.buyName);
    query.exec();
    QSqlDatabase::database().commit();
    if (!query.isActive())
    {
        m_db->close();
        return JOIN_SHOPPING_CART_FAIL;
    }
    return JOIN_SHOPPING_CART_SUCCESS;
}

/********************************删除信息模块*******************************/

/*************************************************
Function Name： removeFormShoppingCart()
Description: 从购物车中取去除
*************************************************/
int IMDatabaseCtrl::removeFormShoppingCart(const Commodity &commodityInf)
{
    if (!createConnection())
    {
        return DELETE_SHOPPING_CART_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(DELETE_COMMODITY_IN_SHOPPINGCATR_SQL);
    query.addBindValue(commodityInf.commodityID);
    query.addBindValue(commodityInf.buyName);
    query.exec();
    QSqlDatabase::database().commit();
    if (!query.isActive())
    {
        m_db->close();
        return DELETE_SHOPPING_CART_FAIL;
    }
    return DELETE_SHOPPING_CART_SUCCESS;
}

/*************************************************
Function Name： clearShopingCart()
Description: 从指定用户名的购物车清空
*************************************************/
int IMDatabaseCtrl::clearShopingCart(const QVector<Commodity>& commodityInfVec, const QString &userName)
{
    if (!createConnection())
    {
        return CLEAR_SHOPPING_CART_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    for (int i = 0; i < commodityInfVec.size(); i++)
    {
        query.prepare(DELETE_COMMODITY_IN_SHOPPINGCATR_SQL);
        query.addBindValue(commodityInfVec[i].commodityID);
        query.addBindValue(commodityInfVec[i].buyName);
    }
    query.exec();
    QSqlDatabase::database().commit();
    if (!query.isActive())
    {
        m_db->close();
        return CLEAR_SHOPPING_CART_FAIL;
    }
    return CLEAR_SHOPPING_CART_SUCCESS;
}

/********************************更新信息模块*******************************/

/*************************************************
Function Name： updateUserInformation()
Description: 修改用户信息
*************************************************/
int IMDatabaseCtrl::updateUserInformation(const UserInformation &user)
{
    if (!createConnection())
    {
        return CHANGE_USERINFORMATION_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(UPDATE_USER_INFORMATION_SQL);
    query.addBindValue(user.logInPassWord);
    query.addBindValue(user.address);
    query.addBindValue(user.buyer.balance);
    query.addBindValue(user.userName);
    query.exec();

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return CHANGE_USERINFORMATION_FAIL;
    }
    return CHANGE_USERINFORMATION_SUCCESS;
}

/*************************************************
Function Name： updateCommodityInformation()
Description: 卖家修改商品信息
*************************************************/
int IMDatabaseCtrl::updateCommodityInformation(const Commodity &commodityInf)
{
    if (!createConnection())
    {
        return CHANGE_COMMODITY_INFORMATION_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(UPDATE_COMMODITY_INFORMATION_SQL);
    query.addBindValue(commodityInf.originalCost);
    query.addBindValue(commodityInf.couponVec);
    query.addBindValue(commodityInf.discountString);
    query.addBindValue(commodityInf.leftNumber);
    query.addBindValue(commodityInf.commodityID);
    query.exec();

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return CHANGE_COMMODITY_INFORMATION_FAIL;
    }
    return CHANGE_COMMODITY_INFORMATION_SUCCESSS;
}

///*************************************************
//Function Name： updateClothInformation()
//Description: 修改服饰信息
//*************************************************/
//int IMDatabaseCtrl::updateClothInformation(const Cloth &clothInf)
//{
//    if (!createConnection())
//    {
//        return ADJUST_CLOTH_INFORMATION_FAIL;
//    }
//    QSqlDatabase::database().transaction();
//    QSqlQuery query(*m_db);
//    query.prepare(UPDATE_CLOTH_INFORMATION_SQL);
//    query.addBindValue(clothInf.cloth.originalCost);
//    query.addBindValue(clothInf.cloth.coupon);
//    query.addBindValue(clothInf.cloth.discount);
//    query.addBindValue(clothInf.cloth.leftNumber);
//    query.addBindValue(clothInf.sizeList);
//    query.addBindValue(clothInf.colorList);
//    query.exec();

//    QSqlDatabase::database().commit();

//    if (!query.isActive())
//    {
//        m_db->close();
//        return ADJUST_CLOTH_INFORMATION_FAIL;
//    }
//    return ADJUST_CLOTH_INFORMATION_SUCCESS;
//}

///*************************************************
//Function Name： updateFoodInformation()
//Description: 修改美食信息
//*************************************************/
//int IMDatabaseCtrl::updateFoodInformation(const Food &foodInf)
//{
//    if (!createConnection())
//    {
//        return ADJUST_FOOD_INFORMATION_FAIL;
//    }
//    QSqlDatabase::database().transaction();
//    QSqlQuery query(*m_db);
//    query.prepare(UPDATE_FOOD_INFORMATION_SQL);
//    query.addBindValue(foodInf.food.originalCost);
//    query.addBindValue(foodInf.food.coupon);
//    query.addBindValue(foodInf.food.discount);
//    query.addBindValue(foodInf.food.leftNumber);
//    query.addBindValue(foodInf.tasteList);
//    query.exec();

//    QSqlDatabase::database().commit();

//    if (!query.isActive())
//    {
//        m_db->close();
//        return ADJUST_FOOD_INFORMATION_FAIL;
//    }
//    return ADJUST_FOOD_INFORMATION_SUCCESS;
//}

///*************************************************
//Function Name： updateDigitalInformation()
//Description: 修改数码信息
//*************************************************/
//int IMDatabaseCtrl::updateDigitalInformation(const Digital &digitalInf)
//{
//    if (!createConnection())
//    {
//        return ADJUST_DIGITAL_INFORMATION_FAIL;
//    }
//    QSqlDatabase::database().transaction();
//    QSqlQuery query(*m_db);
//    query.prepare(UPDATE_DIGITAL_INFORMATION_SQL);
//    query.addBindValue(digitalInf.digital.originalCost);
//    query.addBindValue(digitalInf.digital.coupon);
//    query.addBindValue(digitalInf.digital.discount);
//    query.addBindValue(digitalInf.digital.leftNumber);
//    query.exec();

//    QSqlDatabase::database().commit();

//    if (!query.isActive())
//    {
//        m_db->close();
//        return ADJUST_DIGITAL_INFORMATION_FAIL;
//    }
//    return ADJUST_DIGITAL_INFORMATION_SUCCESS;
//}

/********************************查询信息模块*******************************/

/*************************************************
Description: 查询用户信息
*************************************************/
int IMDatabaseCtrl::searchUserInformation(UserInformation &userInf)
{
    qDebug() << "database:logIn";
    QString &id = userInf.userName;
    if (!createConnection())//检查是否连接数据库
    {
        return LOGIN_FAIL;
    }
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_USER_IN_USER_INFORMATION_ID_SQL);
    query.addBindValue(id);
    query.exec();
    QSqlDatabase::database().commit();
    if (!query.isActive())
    {
        m_db->close();
        return LOGIN_FAIL;
    }
    if (query.size() <= 0)
    {
        m_db->close();
        return LOGIN_FAIL;
    }
    if (query.next())
    {
         userInf.userType = query.value(3).toInt();
         userInf.address = query.value(2).toString();
         userInf.logInPassWord = query.value(1).toString();
         if(userInf.userType == 1)//1代表买家
         {
             userInf.buyer.balance = query.value(5).toDouble();
             userInf.buyer.payPassWord = query.value(6).toString();
         }
    }
    return LOGIN_SUCCESS;
}

/*************************************************
Description: 卖家获得卖家所卖的所有商品信息
*************************************************/
int IMDatabaseCtrl::searchSellerSellAllCommodityInf(const QString &sellerName, QVector<Commodity> &commodityVecInf)
{
    if (!createConnection())
    {
        return SELLER_ALL_COMMODITY_INF_FAIL;
    }
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_COMMODITYVEC_IN_USER_INFORMATION_SQL);
    query.addBindValue(sellerName);
    query.exec();
    QSqlDatabase::database().commit();
    if (!query.isActive())
    {
        m_db->close();
        return SELLER_ALL_COMMODITY_INF_FAIL;
    }
    commodityVecInf.clear();
    while (query.next())
    {
        Commodity common;
        common.commodityID = query.value(0).toInt();
        common.m_headPortrait = query.value(1).toInt();
        common.shopType = query.value(2).toInt();
        common.commodityName = query.value(3).toString();
        common.manufacturers = query.value(4).toString();
        common.originalCost = query.value(5).toDouble();
        common.saleNumber = query.value(8).toInt();
        common.leftNumber = query.value(9).toInt();
        common.discountString = query.value(11).toString();
        common.sellName = query.value(12).toString();
        common.couponVec = query.value(14).toString();
        common.decreaseMoney = query.value(15).toDouble();
        common.sellAddress = query.value(16).toString();
        for (int i = 1; i < 4; i++)
        {
            common.colorList.push_back(i);
        }

        commodityVecInf.push_back(common);
    }
    if (commodityVecInf.isEmpty())
    {
        return SELLER_ALL_COMMODITY_INF_FAIL;
    }
    return SELLER_ALL_COMMODITY_INF_SUCCESS;
}


/*************************************************
Description: 查询所有商品信息
*************************************************/
int IMDatabaseCtrl::searchAllCommodityInformation(QVector<Commodity> &commodityVecInf)
{
    if (!createConnection())
    {
        return GET_ALL_COMMODITYS_FAIL;
    }
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_COMMODITY_IN_COMMODITY_INFORMATION_SQL);
    query.exec();

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_ALL_COMMODITYS_FAIL;
    }
    commodityVecInf.clear();
    while (query.next())
    {
        Commodity common;
        common.commodityID = query.value(0).toInt();
        common.m_headPortrait = query.value(1).toInt();
        common.shopType = query.value(2).toInt();
        common.commodityName = query.value(3).toString();
        common.manufacturers = query.value(4).toString();
        common.originalCost = query.value(5).toDouble();
        common.nowPrice = query.value(6).toDouble();
        common.saleNumber = query.value(8).toInt();
        common.leftNumber = query.value(9).toInt();
        common.discountString = query.value(11).toString();
        common.sellName = query.value(12).toString();
        common.couponVec = query.value(14).toString();
        common.decreaseMoney = query.value(15).toDouble();
        common.sellAddress = query.value(16).toString();
        for (int i = 1; i < 4; i++)
        {
            common.colorList.push_back(i);
        }
        commodityVecInf.push_back(common);
    }
    if (commodityVecInf.isEmpty())
    {
        return NO_THIS_TYPE_COMMODITYS;
    }
    return GET_ALL_COMMODITYS_SUCCESS;
}

///*************************************************
//Description: 通过货号查询商品信息卖家信息
//*************************************************/
//int IMDatabaseCtrl::searchCommodityInformation(Commodity &commodityINf)
//{
//    int &id = commodityINf.commodityID;
//    if (!createConnection())//检查是否连接数据库
//    {
//        return false;
//    }
//    QSqlDatabase::database().transaction();

//    QSqlQuery query(*m_db);
//    query.prepare(SEARCH_COMMODITY_IN_COMMODITY_INFORMATION_SQL);
//    query.addBindValue(id);
//    query.exec();
//    QSqlDatabase::database().commit();
//    if (!query.isActive())
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.size() <= 0)
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.next())
//    {
//        commodityINf.sellAddress = query.value(0).toString();
//        commodityINf.sellName = query.value(0).toInt();
//    }
//    return true;
//}

///*************************************************
//Description: 查询所有服装信息
//*************************************************/
//int IMDatabaseCtrl::searchAllClothInformation(Cloth &clothInf)
//{
//    int &id = clothInf.cloth.commodityID;
//    if (!createConnection())//检查是否连接数据库
//    {
//        return false;
//    }
//    QSqlDatabase::database().transaction();

//    QSqlQuery query(*m_db);
//    query.prepare(SEARCH_USER_IN_CLOTH_INFORMATION_ID_SQL);
//    query.addBindValue(id);
//    query.exec();
//    QSqlDatabase::database().commit();
//    if (!query.isActive())
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.size() <= 0)
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.next())
//    {
//        Commodity& common = clothInf.cloth;
//        clothInf.sizeList = query.value(0).toStringList();
//        clothInf.colorList = query.value(0).toStringList();
//        common.coupon = query.value(0).toString();
//        common.discount = query.value(0).toDouble();
//        common.nowPrice = query.value(0).toDouble();
//        common.shopType = query.value(0).toString();
//        common.leftNumber = query.value(0).toInt();
//        common.saleNumber = query.value(0).toInt();
//        common.totalPrice = query.value(0).toDouble();
//        common.commodityID = query.value(0).toInt();
//        common.originalCost = query.value(0).toDouble();
//    }
//    return true;
//}
///*************************************************
//Description: 查询美食信息
//*************************************************/
//int IMDatabaseCtrl::searchFoodInformation(Food &foodInf)
//{
//    int &id = foodInf.food.commodityID;
//    if (!createConnection())//检查是否连接数据库
//    {
//        return false;
//    }
//    QSqlDatabase::database().transaction();

//    QSqlQuery query(*m_db);
//    query.prepare(SEARCH_USER_IN_FOOD_INFORMATION_ID_SQL);
//    query.addBindValue(id);
//    query.exec();
//    QSqlDatabase::database().commit();
//    if (!query.isActive())
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.size() <= 0)
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.next())
//    {
//        Commodity& common = foodInf.food;
//        foodInf.tasteList = query.value(0).toStringList();
//        common.coupon = query.value(0).toString();
//        common.discount = query.value(0).toDouble();
//        common.nowPrice = query.value(0).toDouble();
//        common.shopType = query.value(0).toString();
//        common.leftNumber = query.value(0).toInt();
//        common.saleNumber = query.value(0).toInt();
//        common.totalPrice = query.value(0).toDouble();
//        common.commodityID = query.value(0).toInt();
//        common.originalCost = query.value(0).toDouble();
//    }
//    return true;
//}

///*************************************************
//Description: 查询数码信息
//*************************************************/
//int IMDatabaseCtrl::searchDigitalInformation(Digital &digitalInf)
//{
//    int &id = digitalInf.digital.commodityID;
//    if (!createConnection())//检查是否连接数据库
//    {
//        return false;
//    }
//    QSqlDatabase::database().transaction();

//    QSqlQuery query(*m_db);
//    query.prepare(SEARCH_USER_IN_DIGITAL_INFORMATION_ID_SQL);
//    query.addBindValue(id);
//    query.exec();
//    QSqlDatabase::database().commit();
//    if (!query.isActive())
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.size() <= 0)
//    {
//        m_db->close();
//        return false;
//    }
//    if (query.next())
//    {
//        Commodity& common = digitalInf.digital;
//        common.coupon = query.value(0).toString();
//        common.discount = query.value(0).toDouble();
//        common.nowPrice = query.value(0).toDouble();
//        common.shopType = query.value(0).toString();
//        common.leftNumber = query.value(0).toInt();
//        common.saleNumber = query.value(0).toInt();
//        common.totalPrice = query.value(0).toDouble();
//        common.commodityID = query.value(0).toInt();
//        common.originalCost = query.value(0).toDouble();
//    }
//    return true;
//}

