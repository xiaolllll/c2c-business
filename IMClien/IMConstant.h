#ifndef IMCONSTANT_H
#define IMCONSTANT_H

#include <QFont>
#include <QColor>
#include <QDataStream>
#include <QDateTime>
#include <QVector>
#include <QString>
#include <QObject>
class IMClientSocketCtrl;

enum MessageType//信息类型
{
    /***********************buyer to server************************/
    CHECK_CONNECTION,               // 检查是否能与服务器连接
    LOGIN,                          // 登录
    REGISTER,                       // 注册
    CHANGE_USERINFORMATION,         // 修改用户信息
    QUIT,                           // 退出

    GET_ALL_COMMODITY,              // 获取所有商品信息
    GET_COMMODITY_INFORMATION,      //获取单个商品信息
    SEARCH_COMMODITY_INFORMATION,   //搜索商品(在vector中寻找即可)
    BUY_NOW,                        //立即购买
    BUY_FROM_SHOP_CART,             //从购物车中购买
    JOIN_SHOPPING_CART,             //加入购物车
    DELETE_SHOPPING_CART,           //从购物车中删去
    REMOVE_FROM_SHOPPONG_CART,      //商品从购物车删去
    CLEAR_SHOPPING_CART,            //清空购物车
    SALES_RETURN,                   //退货
    REPLACE_COMMODITY,              //更换商品
    /***********************seller to server************************/
//    CHECK_CONNECTION,             // 检查是否能与服务器连接
//    LOGIN,                        // 登录
//    REGISTER,                     // 注册
//    CHANGE_PASSWORD,              // 修改密码
//    QUIT,                         // 退出
    //以上五条与buyer重复
    CHANGE_COMMODITY_INFORMATION,   // 修改商品信息
    SELLER_ALL_COMMODITY_INF,       //卖家获得卖家销售的商品
    SEND_COMMODITY,                 // 发货
    REMIND_BUYER_PAY,               //提醒对方付款

    /***********************server to user************************/
    LOGIN_SUCCESS,                          // 登录成功
    LOGIN_FAIL,                             // 登录失败
    REGISTER_SUCCESS,                       // 注册成功
    REGISTER_FAIL,                          // 注册失败
    CHANGE_USERINFORMATION_SUCCESS,         // 修改用户信息
    CHANGE_USERINFORMATION_FAIL,            // 修改用户信息
    SEARCH_USER_INFORMATION_SUCCESS,        //查询用户信息
    SEARCH_USER_INFORMATION_FAIL,           //查询用户信息

    GET_ALL_COMMODITYS_SUCCESS,             // 获取所有商品成功
    GET_ALL_COMMODITYS_FAIL,                // 获取所有商品失败
    NO_THIS_TYPE_COMMODITYS,                // 无某类商品
    GET_COMMODITY_INFORMATION_SUCCESS,      //获取单个商品信息
    GET_COMMODITY_INFORMATION_FAIL,         //获取单个商品信息
    SEARCH_INFORMATION_SUCCESS,             //搜索商品
    SEARCH_INFORMATION_FAIL,                //搜索商品
    BUY_NOW_SUCCESS,                        //立即购买
    BUY_NOW_FAIL,                           //立即购买
    BUY_FROM_SHOP_CART_SUCCESS,             //从购物中购买
    BUY_FROM_SHOP_CART_FAIL,                //从购物中购买
    JOIN_SHOPPING_CART_SUCCESS,             //加入购物车
    JOIN_SHOPPING_CART_FAIL,                //加入购物车
    DELETE_SHOPPING_CART_SUCCESS,           //从购物车中删去
    DELETE_SHOPPING_CART_FAIL,               //从购物车中删去
    REMOVE_FROM_SHOPPONG_CART_SUCCESS,      //商品从购物车删去
    REMOVE_FROM_SHOPPONG_CART_FAIL,         //商品从购物车删去
    CLEAR_SHOPPING_CART_SUCCESS,            //清空购物车
    CLEAR_SHOPPING_CART_FAIL,               //清空购物车
    SELLER_ALL_COMMODITY_INF_SUCCESS,       //获取某个商家卖的所有商品
    SELLER_ALL_COMMODITY_INF_FAIL,          //获取某个商家卖的所有商品
    SALES_RETURN_SUCCESS,                   //退货
    SALES_RETURN_FAIL,                      //退货

    /***********************server to seller************************/
    //相同的代码同上
    CHANGE_COMMODITY_INFORMATION_SUCCESSS,  //修改商品信息
    CHANGE_COMMODITY_INFORMATION_FAIL,      //修改商品信息
    SEND_COMMODITY_SUCCESS,                 // 发货
    SEND_COMMODITY_FAIL,                    // 发货
    REMIND_BUYER_PAY_SUCCESS,               //提醒对方付款
    REMIND_BUYER_PAY_FAIL                   //提醒对方付款

};

struct Buyer//买家信息
{
    QString payPassWord;                    //支付密码
    double balance;                         //账户余额
};

struct UserInformation//用户信息
{
    QString address;                        //地址
    QString userName;                       //用户名
    QString logInPassWord;                  //密码
    int userType;                           //0代表卖家， 1代表买家
    int image;                              //头像
    Buyer buyer;                            //买家
    friend QDataStream &operator<<(QDataStream &out, const UserInformation &user)
    {
       out << user.address << user.userName << user.logInPassWord << user.userType
           << user.image << user.buyer.balance << user.buyer.payPassWord;
       return out;
    }
    friend QDataStream &operator>>(QDataStream &in, UserInformation &user)
    {
        in >> user.address >> user.userName >> user.logInPassWord>> user.userType
           >> user.image >> user.buyer.balance >> user.buyer.payPassWord;
       return in;
    }
};

struct Commodity//商品
{
    /***********共有***************/
    int commodityID;                        //货号
    int m_headPortrait;                     //头像
    int shopType;                           //商品类型，0代表服装类，1代表食品类，2代表数码类
    QString commodityName;                  //名字
    QString manufacturers;                  //厂商
    double originalCost;                    //原价
    double nowPrice;                        //现价
    double totalPrice;                      //总价
    int saleNumber;                         //销量
    int leftNumber;                         //剩余数量
    int buyNumber;                          //购买数量
    QString discountString;                 //折扣string
    QString sellName;                       //卖家用户名
    QString buyName;                        //买家用户名
    QString couponVec;                      //优惠券
    double decreaseMoney;                   //降价
    QString sellAddress;                    //卖家地址
    QString buyAddress;                     //买家地址
    /***********服装特有***************/
    int color;                              //颜色
    int size;                               //尺寸
    QVector<int> colorList;                 //颜色列表
    QVector<int> sizeList;                  //尺寸列表
    /***********食品特有***************/
    int taste;                              //口味
    QVector<int> tasteList;                 //口味列表

    friend QDataStream &operator<<(QDataStream &out, const Commodity &user)
    {
       out << user.commodityID << user.m_headPortrait << user.shopType << user.commodityName
           << user.manufacturers << user.originalCost << user.nowPrice
           << user.totalPrice << user.saleNumber << user.leftNumber
           << user.buyNumber << user.discountString
           << user.sellName << user.buyName
           << user.couponVec << user.decreaseMoney << user.sellAddress
           << user.buyAddress << user.color << user.size << user.colorList << user.sizeList
           << user.taste << user.tasteList;
       return out;
    }
    friend QDataStream &operator>>(QDataStream &in, Commodity &user)
    {
        in >> user.commodityID >> user.m_headPortrait >> user.shopType >> user.commodityName
           >> user.manufacturers >> user.originalCost >> user.nowPrice
           >> user.totalPrice >> user.saleNumber >> user.leftNumber
           >> user.buyNumber >> user.discountString
           >> user.sellName >> user.buyName
           >> user.couponVec >> user.decreaseMoney >> user.sellAddress
           >> user.buyAddress >> user.color >> user.size >> user.colorList >> user.sizeList
           >> user.taste >> user.tasteList;
       return in;
    }

};

struct SaveTmpInformation                   //存储临时信息的结构体，用于信号/糟
{
    int m_requestKind;                      //处理种类
    int m_replyKind;                        //回复种类

    int commodityTypeInf;                   //获取商品种类
    UserInformation userInf;                //用户信息
    Commodity commodityInf;                 //商品信息
    QVector<Commodity> commodityVecInf;     //商品信息列表
    QVector<Commodity> shoppingCart;        //购物车
    IMClientSocketCtrl *m_clientSocket;
};
#endif // IMCONSTANT_H
