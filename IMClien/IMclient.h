#ifndef IMCLIENT_H
#define IMCLIENT_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>
#include <QCompleter>
#include <QStringListModel>
#include "shopcartbtn.h"
#include "IMMainCtrl.h"
#include "IMConstant.h"

namespace Ui {
class IMclient;
}

class IMclient : public QWidget
{
    Q_OBJECT

public:

    explicit IMclient(const UserInformation me, QWidget *parent = nullptr);
    ~IMclient();
    void getCommodityVector();//获取卖家所卖商品的列表
    //获得具体的商品信息并显示
    void getCommodityInformation(const int& ID);
    //售后处理（将接口留给商品button类）
    void DealWithAfterBuy(const int &commodityID);
     //加入我的付款车
    void addToPayCart(const int&);
    //从我的付款车删除
    void deleteFromPayCart(const int&);
    //从我的购物车删除
    void deleteFromShoppingCart(const int&);

private slots:
    void showUserInformation();//显示个人信息

    void sellerChangeCommodity(Commodity);

    void on_pushButtonChangeUserInf_clicked();

    void on_pushButtonMyShoppingCart_clicked();

    void on_pushButtonReadHistory_clicked();

    void on_pushButtonDealWithAfterBuy_clicked();

    void on_pushButtonSearch_clicked();

    void on_sureChangeUserInfButton_clicked();

    void on_cancelChangeUserInfButton_clicked();

    void on_pushButtonFood_clicked();

    void on_pushButtonClothes_clicked();

    void on_pushButtonDigital_clicked();

    void on_ptnJoinShoppingCart_clicked();

    void on_ptnBuyNow_clicked();

    void on_ptnJoinFoodShoppingCart_clicked();

    void on_ptnBuyFoodNow_clicked();

    void on_ptnDigitalJoinShoppingCart_clicked();

    void on_DigitalptnBuyNow_clicked();

    void on_pushButtonClothSalesReturn_clicked();

    void on_pushButtonFoodSalesReturn_clicked();

    void on_pushButtonDigitalSalesReturn_clicked();

private slots:
    void judgeIfPaySuccess(bool, int);//判断是否支付成功

    void on_lineEditClothBuyNumber_textChanged(const QString &arg1);

    void on_lineEditFoodBuyNumber_textChanged(const QString &arg1);

    void on_lineEditDigitalBuyNumber_textChanged(const QString &arg1);
    //关闭窗口
    void closeWindow();

    void setCommodityVec(QVector<Commodity>& commodityVector);//设置商品列表
    void on_pushButtonClearShoppingCart_clicked();
    void addToMyInformBox(QString);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:

    double satisfyPrice;
    double decreasePrice;
    int discount1;
    int discount2;
    int discount3;
    QCompleter *completer;
    QStringList valueList;
    QStringListModel *listModel;
    QVBoxLayout *recommendLayout;
    QVBoxLayout *foodVBoxLayout;
    QVBoxLayout *clothVBoxLayout;
    QVBoxLayout *digitalVBoxLayout;
    QVBoxLayout *dealWithAfterBuy;
    QVBoxLayout *readHistoryLayout;
    QVBoxLayout *shoppingCartLayout;
    QVBoxLayout *verticalLayout;

    IMMainCtrl *m_mainCtrl;
    UserInformation userInf;
    Commodity commodityInf;
    QVector<QString> myMessageBox;
    QVector<QString> deleteBox;
    QVector<int> readCommodityHistory;
    QMap<int, Commodity*> buyCommodityHistory;
    QVector<Commodity> haveBuyCommodityVec;//已经购买的商品列表
    QVector<Commodity> commodityVec;//买家所需要的商品列表
    QVector<Commodity> clothVectorShow;//衣服展示
    QVector<Commodity> foodVectorShow;//食品展示
    QVector<Commodity> digitalVectorShow;//数码展示
    QVector<Commodity> shoopingCart;//购物车
    QVector<Commodity> payCart;//付款车
    QMap<QString, Commodity*> commodityNameInfMap;
    QMap<QString, int> commodityNameIDMap;
    QMap<int, Commodity*> commodityInformationMap;//商品ID和商品对应的map
    //type : 1 代表为立刻支付，其他代表从购物车中支付
    void payMoney(const int &type, const double &balance, const double &payMoney,const QString& userID);//用户支付
    void initIMMainWidget();//初始化
    void transferClothDiscount();//折扣展示函数
    void transferFoodDiscount();
    void transferDigitalDiscount();
    void transferClothColor();
    void transferClothSize();
    void transferClothCoupon();//折扣券展示函数
    void transferFoodCoupon();//折扣券展示函数
    void transferDigitalCoupon();//折扣券展示函数
    bool judgeInputNumberIsValid(int number);
private:
    Ui::IMclient *ui;
};

#endif // IMCLIENT_H
