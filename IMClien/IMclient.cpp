/*************************************************
代码写的太丑了，想个方法改进一下QAQ
*************************************************/
#include "IMclient.h"
#include "ui_imclient.h"
#include "commoditybutton.h"
#include "IMPayMoney.h"
#include <QScrollArea>
#include <QDebug>
#include <QCompleter>
#include <QVBoxLayout>
#include <QMessageBox>
#include <myinform.h>
#include <QStringListModel>

IMclient::IMclient(const UserInformation me, QWidget *parent) :
    QWidget(parent), userInf(me),
    ui(new Ui::IMclient)
{
    ui->setupUi(this);
    userInf.userType = 1;//代表买家
    qDebug() << userInf.userName << endl;
    m_mainCtrl = new IMMainCtrl(userInf);
    commodityInf.buyName = userInf.userName;
    commodityInf.buyAddress = userInf.address;
    ui->label->setVisible(false);
    setWindowTitle("买家窗口");
    initIMMainWidget();
    ui->stackedWidget->setCurrentIndex(6);
}

IMclient::~IMclient()
{
    delete ui;
}

/*************************************************
Function Name： on_pushButtonSearch_clicked
Description: 点击搜索
*************************************************/
void IMclient::on_pushButtonSearch_clicked()
{
    QString commodityName = ui->lineEditSearch->text();
    QMap<QString, Commodity*>::iterator iter;
    iter = commodityNameInfMap.find(commodityName);
    if(iter != commodityNameInfMap.end())
    {
        int commodityID;
        QMap<QString, int>::iterator  iterName;
        iterName = commodityNameIDMap.find(commodityName);
        if(iterName != commodityNameIDMap.end())
        {
            commodityID = *iterName;
        }
        getCommodityInformation(commodityID);//转到详细商品信息页
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("无此商品！"), QMessageBox::Ok);
        return;
    }
    ui->lineEditSearch->clear();
}

/*************************************************
Function Name： on_pushButtonClearShoppingCart_clicked
Description: 点击购物车结算
*************************************************/
void IMclient::on_pushButtonClearShoppingCart_clicked()
{
    double totalPrice = 0;
    for (QVector<Commodity>::iterator i=payCart.begin(); i!=payCart.end(); i++)
    {
        totalPrice += (i)->totalPrice;
    }
    payMoney(0,userInf.buyer.balance, totalPrice, userInf.buyer.payPassWord);//点击支付
}

/*************************************************
Function Name： on_pushButton_2_clicked
Description: 点击消息盒子中的删除
*************************************************/
void IMclient::on_pushButton_2_clicked()
{
    myMessageBox.clear();
    on_pushButton_clicked();
}

/*************************************************
Function Name： addToMyInformBox
Description: 添加到消息盒子中
*************************************************/
void IMclient::addToMyInformBox(QString temp)
{
    myMessageBox.push_back(temp);
    ui->label->setVisible(true);
}

/*************************************************
Function Name： on_pushButton_clicked
Description: 点击我的通知，消息盒子功能
*************************************************/
void IMclient::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
    ui->label->setVisible(false);
    int n = verticalLayout->count();
    while(0 != n)
    {
        QWidget *p=verticalLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        verticalLayout->removeWidget(p);
        delete p;
        n--;
    }
    int len = myMessageBox.size();
    qDebug() << "myMessageBox.size()" << myMessageBox.size();
    if(len  == 0)
    {
        QString common = "暂无通知！";
        myInform *m_inform = new myInform(this, common);
        verticalLayout->addWidget(m_inform);
//        QMessageBox::information(nullptr, tr("提示"), tr("暂无通知！"), QMessageBox::Ok);
        return;
    }
    for(int i=0; i<len; i++)
    {
        QString common = myMessageBox[i];
        qDebug() << "myMessageBox[i]" << myMessageBox[i];
        myInform *m_inform = new myInform(this, common);
        verticalLayout->addWidget(m_inform);
    }
}

/*************************************************
Function Name： on_pushButtonMyShoppingCart_clicked
Description: 点击我的购物车
*************************************************/
void IMclient::on_pushButtonMyShoppingCart_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "shoppingCartLayout->count()" << shoppingCartLayout->count();
    int n = shoppingCartLayout->count();
    while(0 != n)
    {
        QWidget *p=shoppingCartLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        shoppingCartLayout->removeWidget(p);
        delete p;
        n--;
    }
    qDebug() << "shoppingCartLayout->count()" << shoppingCartLayout->count();
    int len = shoopingCart.size();
    qDebug() << "shopping Cart size" << len;
    if(len  == 0)
    {
        QString common = "购物车中暂无记录！";
        myInform *m_inform = new myInform(this, common);
        shoppingCartLayout->addWidget(m_inform);
//        QMessageBox::information(nullptr, tr("提示"), tr("购物车中暂无记录！"), QMessageBox::Ok);
        return;
    }
    for(int i=0; i<len; i++)
    {
        Commodity common = shoopingCart[i];
        shopCartBtn *shopCartWidget = new shopCartBtn(this, common.commodityID, common.commodityName,
                                                      common.buyNumber, common.totalPrice);
        shoppingCartLayout->addWidget(shopCartWidget);
    }
}

/*************************************************
Function Name： addToPayCart
Description: 加入我的付款车
*************************************************/
void IMclient::addToPayCart(const int& commodity)
{
    qDebug() << "addToPayCart";
    for (QVector<Commodity>::iterator i=shoopingCart.begin(); i!=shoopingCart.end(); i++)
    {
        if(i->commodityID == commodity)
        {
            qDebug() << "payCart.push_back(*iter)" << i->commodityName;
            payCart.push_back(*i);
        }
    }
}

/*************************************************
Function Name： deleteFromPayCart
Description: 从我的付款车删除
*************************************************/
void IMclient::deleteFromPayCart(const int& commodity)
{
    for (QVector<Commodity>::iterator i=payCart.begin(); i!=payCart.end(); i++)
    {
        if((i)->commodityID == commodity)
        {
            qDebug() << "payCart.erase(i)" << (i)->commodityName;
            payCart.erase(i);
            break;
        }
    }
}

/*************************************************
Function Name： deleteFromShoppingCart
Description: 从我的购物车删除(此时需要通知卖家)
*************************************************/
void IMclient::deleteFromShoppingCart(const int& commodity)
{
    qDebug() << "deleteFromShoppingCart";
    for (QVector<Commodity>::iterator i=shoopingCart.begin(); i!=shoopingCart.end(); i++)
    {
        if((i)->commodityID == commodity)
        {
            qDebug() << "shoopingCart.erase(i)" << (i)->commodityName;
            shoopingCart.erase(i);
            m_mainCtrl->deleteFromShopCart(*i);
            break;
        }
    }
    on_pushButtonMyShoppingCart_clicked();
}

/*************************************************
Function Name： on_pushButtonReadHistory_clicked
Description: 点击浏览历史，点击之后为购买商品详细信息具体展示页面
这个地方存在的bug:在商品展示界面上,展示的信息会存在相同的情况
*************************************************/
void IMclient::on_pushButtonReadHistory_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    int n = readHistoryLayout->count();
    while(0 != n)
    {
        QWidget *p=readHistoryLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        readHistoryLayout->removeWidget(p);
        delete p;
        n--;
    }
    int len = readCommodityHistory.size();
    qDebug() << "readCommodityHistory.size()" << len;
    if(len  == 0)
    {
        QString common = "暂无浏览记录！";
        myInform *m_inform = new myInform(this, common);
        readHistoryLayout->addWidget(m_inform);
//        QMessageBox::information(this, tr("提示"), tr("暂无浏览记录！"), QMessageBox::Ok);
        return;
    }
    QVector<Commodity> readCommodityIDHistory;//浏览商品历史
    for (int i = 0; i < readCommodityHistory.size(); i++)
    {
        QMap<int, Commodity*>::iterator iter;
        iter = commodityInformationMap.find(readCommodityHistory[i]);
        if(iter != commodityInformationMap.end())
        {
            qDebug() << iter.value()->commodityName;
            readCommodityIDHistory.push_back(*iter.value());
        }
    }
    int j = 0;
    while(len > 0)
    {
        qDebug() << "sssssssssssssssssssssssss" << len;
        QWidget *m_livewidget = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setAlignment(Qt::AlignLeft);
        layout->setSpacing(100);
//        layout->setAlignment(Qt::AlignLeft);
        m_livewidget->setFixedHeight(300);
        m_livewidget->setFixedWidth(1000);
        m_livewidget->setLayout(layout);
        for (int i = 0; i < (len>=3?3:len); i++)
        {
            qDebug() << "readCommodityIDHistory[i]" << readCommodityIDHistory[i + j].commodityName;
            CommodityButton *button = new CommodityButton(readCommodityIDHistory[i + j], this, 0,this);
            layout->addWidget(button);
        }
        qDebug() << "3 number is over";
        len-=3;
        j+=3;
        readHistoryLayout->addWidget(m_livewidget);
    }
}

/*************************************************
Function Name： on_pushButtonDealWithAfterBuy_clicked
Description: 点击售后处理(这里的鼠标事件需要修改)
bug:这里的图片展示存在问题
*************************************************/
void IMclient::on_pushButtonDealWithAfterBuy_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    int n = dealWithAfterBuy->count();
    while(0 != n)
    {
        QWidget *p=dealWithAfterBuy->itemAt(0)->widget();
        p->setParent (nullptr);
        dealWithAfterBuy->removeWidget(p);
        delete p;
        n--;
    }
    int len = haveBuyCommodityVec.size();
    if(len  == 0)
    {
        QString common = "暂无商品购买记录！";
        myInform *m_inform = new myInform(this, common);
        dealWithAfterBuy->addWidget(m_inform);
//        QMessageBox::information(nullptr, tr("提示"), tr("暂无商品购买记录！"), QMessageBox::Ok);
        return;
    }
    int j = 0;
    while(len > 0)
    {
        QWidget *m_livewidget = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(50);
        layout->setAlignment(Qt::AlignLeft);
        m_livewidget->setFixedHeight(300);
        m_livewidget->setFixedWidth(1000);
        m_livewidget->setLayout(layout);
        for (int i = 0; i < (len>=4?4:len); i++)
        {
            CommodityButton *button = new CommodityButton(haveBuyCommodityVec[i+j], this, 1,this);
            layout->addWidget(button);
        }
        len-=4;
        j+=4;
        dealWithAfterBuy->addWidget(m_livewidget);
    }
}

/*************************************************
Function Name： DealWithAfterBuy
Description: 售后处理（将接口留给商品button类）
*************************************************/
void IMclient::DealWithAfterBuy(const int &commodityID)
{
    for (QVector<Commodity>::iterator i=haveBuyCommodityVec.begin(); i!=haveBuyCommodityVec.end(); i++)
    {
        if(i->commodityID == commodityID)
        {
            qDebug() << "payCart.push_back(*iter)" << i->commodityName;
            commodityInf = *i;
        }
    }
    commodityInf.buyName = userInf.userName;
    commodityInf.buyAddress = userInf.address;
    qDebug() << "DealWithAfterBuy" << commodityInf.buyName;
    switch (commodityInf.shopType)//判断商品类型,0代表服装类，1代表食品类，2代表数码类
    {
    case 0://服装类
    {
        ui->stackedWidget->setCurrentIndex(11);
        QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
            arg(commodityInf.commodityID);
        ui->labelAfterSaleClothImage->setPixmap(QPixmap(str));//设置图像
        ui->labelAfterSaleClothName->setText(commodityInf.commodityName);
        ui->labelAfterSaleClothPrice->setText(QString::number(commodityInf.nowPrice));
        ui->labelAfterSaleClothBuyNumber->setText(QString::number(commodityInf.buyNumber));
        break;
    }
    case 1:
    {
        ui->stackedWidget->setCurrentIndex(12);
        QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
            arg(commodityInf.commodityID);
        ui->labelAfterSaleFoodImage->setPixmap(QPixmap(str));//设置图像
        ui->labelAfterSaleFoodName->setText(commodityInf.commodityName);
        ui->labelAfterSaleFoodPrice->setText(QString::number(commodityInf.nowPrice));
        ui->labelAfterSaleFoodBuyNumber->setText(QString::number(commodityInf.buyNumber));
        break;
    }
    case 2:
    {
        ui->stackedWidget->setCurrentIndex(11);
        QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
            arg(commodityInf.commodityID);
        ui->labelAfterSaleDigitalImage->setPixmap(QPixmap(str));//设置图像
        ui->labelAfterSaleDigitalName->setText(commodityInf.commodityName);
        ui->labelAfterSaleDigitalPrice->setText(QString::number(commodityInf.nowPrice));
        ui->labelAfterSaleDigitalBuyNumber->setText(QString::number(commodityInf.buyNumber));
        break;
    }
    }
}

/*************************************************
Function Name： payMoney
Description: 用户支付
*************************************************/
void IMclient::payMoney(const int &type, const double &balance, const double &payMoney,const QString &userPayPsd)
{
    IMPayMoney *pay = new IMPayMoney(type, balance, payMoney, userPayPsd);
    connect(pay,SIGNAL(paySuccess(bool, int)), this, SLOT(judgeIfPaySuccess(bool, int)));
    pay->show();
}

/*************************************************
这里需要修改需要增加清空payCart的功能
bug::需要将购物车表中的信息删去
*************************************************/
void IMclient::judgeIfPaySuccess(bool paySuccess, int type)
{
    if(paySuccess)//支付成功
    {
        if(type == 1)
        {
            commodityInf.saleNumber = ui->labelClothSellNumber->text().toInt() + commodityInf.buyNumber;//销量需要增加
            commodityInf.leftNumber = ui->labelClothLeftNumber->text().toInt() - commodityInf.buyNumber;//剩余数量需要减少
            for (int i = 0; i < commodityVec.size(); i++)
            {
                if(commodityInf.commodityID == commodityVec[i].commodityID)
                {
                    commodityVec[i].saleNumber = commodityInf.saleNumber;
                    commodityVec[i].leftNumber = commodityInf.leftNumber;
                    break;
                }
            }
            userInf.buyer.balance -= commodityInf.totalPrice;
            commodityInf.buyAddress = userInf.address;
            commodityInf.buyName = userInf.userName;
            m_mainCtrl->buyerBuyCommodity(userInf, commodityInf);
            ui->labelClothSellNumber->setText(tr("%1").arg(commodityInf.saleNumber));//销量
            ui->labelClothLeftNumber->setText(tr("%1").arg(commodityInf.leftNumber));//剩余数量
            haveBuyCommodityVec.push_back(commodityInf);//将已经购买的商品添加到已购买商品列表中
        }
        else
        {
            qDebug() << "结算!";
            for (QVector<Commodity>::iterator i=payCart.begin(); i!=payCart.end(); i++)
            {
                commodityInf = *i;
                commodityInf.saleNumber = ui->labelClothSellNumber->text().toInt() + commodityInf.buyNumber;//销量需要增加
                commodityInf.leftNumber = ui->labelClothLeftNumber->text().toInt() - commodityInf.buyNumber;//剩余数量需要减少
                for (int i = 0; i < commodityVec.size(); i++)
                {
                    if(commodityInf.commodityID == commodityVec[i].commodityID)
                    {
                        commodityVec[i].saleNumber = commodityInf.saleNumber;
                        commodityVec[i].leftNumber = commodityInf.leftNumber;
                        break;
                    }
                }
                userInf.buyer.balance -= commodityInf.totalPrice;
                commodityInf.buyAddress = userInf.address;
                commodityInf.buyName = userInf.userName;
                for (QVector<Commodity>::iterator i=shoopingCart.begin(); i!=shoopingCart.end(); i++)
                {
                    if(i->commodityID == commodityInf.commodityID)
                    {
                        qDebug() << "从购物中删除" << i->commodityName;
                        shoopingCart.erase(i);
                        break;
                    }
                }
                haveBuyCommodityVec.push_back(commodityInf);//将已经购买的商品添加到已购买商品列表中
            }
            m_mainCtrl->buyerClearCart(userInf, payCart);
            payCart.clear();//清空付款车
            on_pushButtonMyShoppingCart_clicked();
        }
    }
}

/*************************************************
Function Name： on_ptnJoinFoodShoppingCart_clicked
Description: Food点击加入购物车
*************************************************/
void IMclient::on_ptnJoinFoodShoppingCart_clicked()
{
    for (QVector<Commodity>::iterator i=shoopingCart.begin(); i!=shoopingCart.end(); i++)
    {
        if((i)->commodityID == commodityInf.commodityID)
        {
            QMessageBox::information(this,tr("加入购物车"),tr("购物车中已有此商品"),QMessageBox::Ok);
            return;
        }
    }
    commodityInf.buyNumber = ui->lineEditFoodBuyNumber->text().toInt();//购买数量
    if(commodityInf.buyNumber <= 0)
    {
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量不能为0！"));
        return ;
    }
    if(commodityInf.buyNumber > ui->labelFoodLeftNumber->text().toInt())//如果购买数量超过剩余数量
    {
        ui->lineEditFoodBuyNumber->clear();
        ui->lineEditFoodBuyNumber->setFocus();
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量超过剩余数量！"));
        return ;
    }
    commodityInf.saleNumber = ui->labelFoodSellNumber->text().toInt() + commodityInf.buyNumber;//销量需要增加
    commodityInf.leftNumber = ui->labelFoodLeftNumber->text().toInt() - commodityInf.buyNumber;//剩余数量需要减少
    for (int i = 0; i < commodityVec.size(); i++)
    {
        if(commodityInf.commodityID == commodityVec[i].commodityID)
        {
            commodityVec[i].saleNumber = commodityInf.saleNumber;
            commodityVec[i].leftNumber = commodityInf.leftNumber;
            break;
        }
    }
    shoopingCart.push_back(commodityInf);
    m_mainCtrl->buyerJoinShoppingCart(commodityInf);//加入购物车
}

/*************************************************
Function Name： on_ptnBuyFoodNow_clicked
Description: Food点击立即购买
*************************************************/
void IMclient::on_ptnBuyFoodNow_clicked()
{
    commodityInf.buyNumber = ui->lineEditFoodBuyNumber->text().toInt();//购买数量
    if(commodityInf.buyNumber <= 0)
    {
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量不能为0！"));
        return ;
    }
    if(commodityInf.buyNumber > ui->labelFoodLeftNumber->text().toInt())//如果购买数量超过剩余数量
    {
        ui->lineEditFoodBuyNumber->clear();
        ui->lineEditFoodBuyNumber->setFocus();
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量超过剩余数量！"));
        return ;
    }
    payMoney(1,userInf.buyer.balance, commodityInf.totalPrice, userInf.buyer.payPassWord);//点击支付
}

/*************************************************
Function Name： on_ptnDigitalJoinShoppingCart_clicked
Description: digital点击加入购物车
*************************************************/
void IMclient::on_ptnDigitalJoinShoppingCart_clicked()
{
    for (QVector<Commodity>::iterator i=shoopingCart.begin(); i!=shoopingCart.end(); i++)
    {
        if((i)->commodityID == commodityInf.commodityID)
        {
            QMessageBox::information(this,tr("加入购物车"),tr("购物车中已有此商品"),QMessageBox::Ok);
            return;
        }
    }
    commodityInf.buyNumber = ui->lineEditDigitalBuyNumber->text().toInt();//购买数量
    if(commodityInf.buyNumber <= 0)
    {
        ui->lineEditClothBuyNumber->clear();
        ui->lineEditClothBuyNumber->setFocus();
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量不能为0！"));
        return ;
    }
    if(commodityInf.buyNumber > ui->labelDigitalLeftNumber->text().toInt())//如果购买数量超过剩余数量
    {
        ui->lineEditDigitalBuyNumber->clear();
        ui->lineEditDigitalBuyNumber->setFocus();
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量超过剩余数量！"));
        return ;
    }
    commodityInf.saleNumber = ui->labelDigitalSellNumber->text().toInt() + commodityInf.buyNumber;//销量需要增加
    commodityInf.leftNumber = ui->labelDigitalLeftNumber->text().toInt() - commodityInf.buyNumber;//剩余数量需要减少
    for (int i = 0; i < commodityVec.size(); i++)
    {
        if(commodityInf.commodityID == commodityVec[i].commodityID)
        {
            commodityVec[i].saleNumber = commodityInf.saleNumber;
            commodityVec[i].leftNumber = commodityInf.leftNumber;
            break;
        }
    }
    shoopingCart.push_back(commodityInf);
    m_mainCtrl->buyerJoinShoppingCart(commodityInf);//加入购物车
}

/*************************************************
Function Name： on_DigitalptnBuyNow_clicked
Description: digital点击立即购买
*************************************************/
void IMclient::on_DigitalptnBuyNow_clicked()
{
    commodityInf.buyNumber = ui->lineEditDigitalBuyNumber->text().toInt();//购买数量
    if(commodityInf.buyNumber <= 0)
    {
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量不能为0！"));
        return ;
    }
    if(commodityInf.buyNumber > ui->labelDigitalLeftNumber->text().toInt())//如果购买数量超过剩余数量
    {
        ui->lineEditDigitalBuyNumber->clear();
        ui->lineEditDigitalBuyNumber->setFocus();
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量超过剩余数量！"));
        return ;
    }
    payMoney(1,userInf.buyer.balance, commodityInf.totalPrice, userInf.buyer.payPassWord);//点击支付
}

/*************************************************
Function Name： on_ptnJoinShoppingCart_clicked
Description: Cloth点击加入购物车
*************************************************/
void IMclient::on_ptnJoinShoppingCart_clicked()
{
    for (QVector<Commodity>::iterator i=shoopingCart.begin(); i!=shoopingCart.end(); i++)
    {
        if((i)->commodityID == commodityInf.commodityID)
        {
            QMessageBox::information(this,tr("加入购物车"),tr("购物车中已有此商品"),QMessageBox::Ok);
            return;
        }
    }
    commodityInf.color = ui->comboBoxClothColor->currentText().toInt();//颜色
    commodityInf.buyNumber = ui->lineEditClothBuyNumber->text().toInt();//购买数量
    if(commodityInf.buyNumber <= 0)
    {
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量不能为0！"));
        return ;
    }
    if(commodityInf.buyNumber > ui->labelClothLeftNumber->text().toInt())//如果购买数量超过剩余数量
    {
        ui->lineEditClothBuyNumber->clear();
        ui->lineEditClothBuyNumber->setFocus();
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量超过剩余数量！"));
        return ;
    }
    commodityInf.saleNumber = ui->labelClothSellNumber->text().toInt() + commodityInf.buyNumber;//销量需要增加
    commodityInf.leftNumber = ui->labelClothLeftNumber->text().toInt() - commodityInf.buyNumber;//剩余数量需要减少
    for (int i = 0; i < commodityVec.size(); i++)
    {
        if(commodityInf.commodityID == commodityVec[i].commodityID)
        {
            commodityVec[i].saleNumber = commodityInf.saleNumber;
            commodityVec[i].leftNumber = commodityInf.leftNumber;
            break;
        }
    }
    shoopingCart.push_back(commodityInf);
    qDebug() << "m_mainCtrl->buyerJoinShoppingCart(commodityInf)" << commodityInf.buyName;
    m_mainCtrl->buyerJoinShoppingCart(commodityInf);//加入购物车
}

/*************************************************
Function Name： on_ptnReturnTheFirst_clicked
Description: 点击返回首页
bug:好货推荐功能是否继续写,不写了，写几个广告页得了
*************************************************/

bool IMclient::judgeInputNumberIsValid(int number)
{
    if(number > 0)
        return true;
    return  false;
}

/*************************************************
Function Name： on_ptnBuyNow_clicked
Description: Cloth点击立即购买
*************************************************/
void IMclient::on_ptnBuyNow_clicked()
{
    commodityInf.buyNumber = ui->lineEditClothBuyNumber->text().toInt();//购买数量
    if(commodityInf.buyNumber <= 0)
    {
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量不能为0！"));
        return ;
    }
    if(commodityInf.buyNumber <= 0)
    {
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量不能为0！"));
        return ;
    }
    if(commodityInf.buyNumber > ui->labelClothLeftNumber->text().toInt())//如果购买数量超过剩余数量
    {
        ui->lineEditClothBuyNumber->clear();
        ui->lineEditClothBuyNumber->setFocus();
        QMessageBox::critical(nullptr, tr("购买商品"), tr("商品购买数量超过剩余数量！"));
        return ;
    }
    payMoney(1,userInf.buyer.balance, commodityInf.totalPrice, userInf.buyer.payPassWord);//点击支付
}

/*************************************************
Function Name： on_lineEditClothBuyNumber_textChanged
Description: 当lineEDit改变时需要修改相应的总价
*************************************************/
void IMclient::on_lineEditClothBuyNumber_textChanged(const QString &arg1)
{
    double res = 0;
    double nowPriceTemp = commodityInf.nowPrice;
    int numberTemp = arg1.toInt();
    if(numberTemp == 1)
    {
        if(discount1 != 0)
        {
           res += nowPriceTemp*discount1/10;
        }
    }
    else if(numberTemp == 2)
    {
        if(discount1 != 0)
        {
            res += nowPriceTemp*discount1/10;
            if(discount2 != 0)
            {
                res += nowPriceTemp*discount2/10;
            }
            else
            {
                res += nowPriceTemp;
            }
        }
    }
    else if(numberTemp >= 3)
    {
        if(discount1 != 0)
        {
            res += nowPriceTemp * discount1 / 10;
            if(0 != discount2)
            {
                res += nowPriceTemp * discount2 /10;
                if(discount3 != 0)
                {
                    res += nowPriceTemp * discount3 /10 * (numberTemp - 2);
                }
                else
                {
                    res += nowPriceTemp * (numberTemp - 2);
                }
            }
            else
            {
                res += nowPriceTemp * (numberTemp - 1);
            }
        }
    }
    if(res > satisfyPrice)
    {
        res -= decreasePrice;
    }
    commodityInf.totalPrice = res;
    ui->labelClothTotalPrice->setText(QString::number(res));
}

void IMclient::on_lineEditFoodBuyNumber_textChanged(const QString &arg1)
{
    double res = 0;
    double nowPriceTemp = commodityInf.nowPrice;
    int numberTemp = arg1.toInt();
    if(numberTemp == 1)
    {
        if(discount1 != 0)
        {
           res += nowPriceTemp*discount1/10;
        }
    }
    else if(numberTemp == 2)
    {
        if(discount1 != 0)
        {
            res += nowPriceTemp*discount1/10;
            if(discount2 != 0)
            {
                res += nowPriceTemp*discount2/10;
            }
            else
            {
                res += nowPriceTemp;
            }
        }
    }
    else if(numberTemp >= 3)
    {
        if(discount1 != 0)
        {
            res += nowPriceTemp * discount1 / 10;
            if(0 != discount2)
            {
                res += nowPriceTemp * discount2 /10;
                if(discount3 != 0)
                {
                    res += nowPriceTemp * discount3 /10 * (numberTemp - 2);
                }
                else
                {
                    res += nowPriceTemp * (numberTemp - 2);
                }
            }
            else
            {
                res += nowPriceTemp * (numberTemp - 1);
            }
        }
    }
    if(res > satisfyPrice)
    {
        res -= decreasePrice;
    }
    commodityInf.totalPrice = res;
    ui->labelFoodTotalPrice->setText(QString::number(res));
}

void IMclient::on_lineEditDigitalBuyNumber_textChanged(const QString &arg1)
{
    double res = 0;
    double nowPriceTemp = commodityInf.nowPrice;
    int numberTemp = arg1.toInt();
    if(numberTemp == 1)
    {
        if(discount1 != 0)
        {
           res += nowPriceTemp*discount1/10;
        }
    }
    else if(numberTemp == 2)
    {
        if(discount1 != 0)
        {
            res += nowPriceTemp*discount1/10;
            if(discount2 != 0)
            {
                res += nowPriceTemp*discount2/10;
            }
            else
            {
                res += nowPriceTemp;
            }
        }
    }
    else if(numberTemp >= 3)
    {
        if(discount1 != 0)
        {
            res += nowPriceTemp * discount1 / 10;
            if(0 != discount2)
            {
                res += nowPriceTemp * discount2 /10;
                if(discount3 != 0)
                {
                    res += nowPriceTemp * discount3 /10 * (numberTemp - 2);
                }
                else
                {
                    res += nowPriceTemp * (numberTemp - 2);
                }
            }
            else
            {
                res += nowPriceTemp * (numberTemp - 1);
            }
        }
    }
    if(res > satisfyPrice)
    {
        res -= decreasePrice;
    }
    commodityInf.totalPrice = res;
    ui->labelDigitalTotalPrice->setText(QString::number(res));
}

/*************************************************
Function Name： transfer
Description: 折扣转换函数
折扣格式：
1：统一折扣
之后的数字为打几折
2：不统一折扣
2988
*************************************************/
void IMclient::transferClothDiscount()
{
    QString temp = commodityInf.discountString;
    qDebug() << "commodityInf.discountString" << temp;
    if(temp.size() == 0)
    {
        ui->labelClothDiscount->setText("此商品暂无折扣！");
        return ;
    }
    if(temp[0] == '1')//统一折扣
    {
        discount1 = (temp.mid(1).toInt());//省略第二个参数表示从position开始截取到末尾
        discount2 = discount3 = discount1;
        ui->labelClothDiscount->setText(tr("打%1折").arg(discount1));//commodityInf.discountString);//展示折扣
    }
    else //不统一折扣
    {
        discount1 = (temp.mid(1).toInt())/100;//省略第二个参数表示从position开始截取到末尾
        discount2 = (temp.mid(2).toInt())/10;
        discount3 = (temp.mid(3).toInt());
        if(discount2 == discount3)
        {
            //展示折扣
            ui->labelClothDiscount->setText(tr("第一件打%1折超过一件打%2折").arg(discount1).arg(discount2));
        }
        else
        {
            //展示折扣
            ui->labelClothDiscount->setText(tr("第一件打%1折第二件打%2折三件以上打%3折").arg(discount1).arg(discount2).arg(discount3));
        }
    }
}
void IMclient::transferFoodDiscount()
{
    QString temp = commodityInf.discountString;
    if(temp.size() == 0)
    {
        ui->labelFoodDiscount->setText("此商品暂无折扣！");
        return ;
    }
    if(temp[0] == '1')//统一折扣
    {
        discount1 = (temp.mid(1).toInt());//省略第二个参数表示从position开始截取到末尾
        discount2 = discount3 = 0;
        ui->labelFoodDiscount->setText(tr("打%1折").arg(discount1));//commodityInf.discountString);//展示折扣
    }
    else //不统一折扣
    {
        discount1 = (temp.mid(1).toInt())/100;//省略第二个参数表示从position开始截取到末尾
        discount2 = (temp.mid(2).toInt())/10;
        discount3 = (temp.mid(3).toInt());
        if(discount2 == discount3)
        {
            //展示折扣
            ui->labelFoodDiscount->setText(tr("第一件打%1折超过一件打%2折").arg(discount1).arg(discount2));
        }
        else
        {
            //展示折扣
            ui->labelFoodDiscount->setText(tr("第一件打%1折第二件打%2折三件以上打%3折").arg(discount1).arg(discount2).arg(discount3));
        }
    }
}
void IMclient::transferDigitalDiscount()
{
    QString temp = commodityInf.discountString;
    if(temp.size() == 0)
    {
        ui->labelDigitalDiscount->setText("此商品暂无折扣！");
        return ;
    }
    if(temp[0] == '1')//统一折扣
    {
        discount1 = (temp.mid(1).toInt());//省略第二个参数表示从position开始截取到末尾
        discount2 = discount3 = 0;
        ui->labelDigitalDiscount->setText(tr("打%1折").arg(discount1));//commodityInf.discountString);//展示折扣
    }
    else //不统一折扣
    {
        discount1 = (temp.mid(1).toInt())/100;//省略第二个参数表示从position开始截取到末尾
        discount2 = (temp.mid(2).toInt())/10;
        discount3 = (temp.mid(3).toInt());
        if(discount2 == discount3)
        {
            //展示折扣
            ui->labelDigitalDiscount->setText(tr("第一件打%1折超过一件打%2折").arg(discount1).arg(discount2));
        }
        else
        {
            //展示折扣
            ui->labelDigitalDiscount->setText(tr("第一件打%1折第二件打%2折三件以上打%3折").arg(discount1).arg(discount2).arg(discount3));
        }
    }
}

/*************************************************
Function Name： transferClothColor
Description: 衣服颜色转换函数
*************************************************/
void IMclient::transferClothColor()
{
    QString res;
    for (int i = 0; i < commodityInf.colorList.size(); i++)
    {
        int temp = commodityInf.colorList[i];
        switch (temp)
        {
        case 0:
        {
            res = "红";
            break;
        }
        case 1:
        {
            res = "黑";
            break;
        }
        case 2:
        {
            res = "蓝";
            break;
        }
        case 3:
        {
            res = "黄";
            break;
        }

        }
        ui->comboBoxClothColor->addItem(res);//设置颜色
    }
}

/*************************************************
Function Name： transferCoupon
Description:券转换函数
*************************************************/
void IMclient::transferClothCoupon()
{
    if(commodityInf.couponVec == "0" || commodityInf.couponVec.size() == 0)//无优惠券
    {
        ui->labelCloth->setText("暂无优惠券");
        return ;
    }
    QString str = commodityInf.couponVec;
    QStringList coupon = str.split("-");
    satisfyPrice = coupon.at(0).toDouble();
    decreasePrice = coupon.at(1).toDouble();
    ui->labelCloth->setText(tr("满%1减%2").arg(satisfyPrice).arg(decreasePrice));
}

void IMclient::transferFoodCoupon()
{
    if(commodityInf.couponVec == "0" || commodityInf.couponVec.size() == 0)//无优惠券
    {
        ui->labelFood->setText("暂无优惠券");
        return ;
    }
    QString str = commodityInf.couponVec;
    QStringList coupon = str.split("-");
    satisfyPrice = coupon.at(0).toDouble();
    decreasePrice = coupon.at(1).toDouble();
    ui->labelFood->setText(tr("满%1减%2").arg(satisfyPrice).arg(decreasePrice));

}

void IMclient::transferDigitalCoupon()
{
    if(commodityInf.couponVec == "0" || commodityInf.couponVec.size() == 0)//无优惠券
    {
        ui->labelDigital->setText("暂无优惠券");
        return ;
    }
    QString str = commodityInf.couponVec;
    QStringList coupon = str.split("-");
    satisfyPrice = coupon.at(0).toDouble();
    decreasePrice = coupon.at(1).toDouble();
    ui->labelDigital->setText(tr("满%1减%2").arg(satisfyPrice).arg(decreasePrice));

}

/*************************************************
Function Name： getCommodityInformation
Description: 获取商品信息并在界面上展示
bug::退货之后需要对数据库中信息进行修改
*************************************************/
void IMclient::getCommodityInformation(const int& commodityID)
{
    qDebug() << "getCommodityInformation";
    QMap<int, Commodity*>::iterator iter;
    iter = commodityInformationMap.find(commodityID);
    if(iter != commodityInformationMap.end())
    {
        commodityInf = **iter;//得到的商品信息需要进行展示
    }
    commodityInf.buyName = userInf.userName;
    commodityInf.buyAddress = userInf.address;
    bool judge = true;
    for (int i = 0; i < readCommodityHistory.size(); i++)
    {
        if(commodityID == readCommodityHistory[i])
        {
            judge = false;
            break;
        }
    }
    if(judge)
    {
        qDebug() << "readCommodityHistory.push_back(commodityID)" << commodityID;//将查看的商品添加到商品记录中
        readCommodityHistory.push_back(commodityID);//将查看的商品添加到商品记录中
    }
    switch (commodityInf.shopType)//判断商品类型
    {
    case 0://商品类型，0代表服装类
    {
        ui->stackedWidget->setCurrentIndex(8);
        qDebug() <<"cloth";
        ui->comboBoxClothColor->clear();
        qDebug() << "commodityInf.colorList.size()" << commodityInf.colorList.size();
        ui->labelClothName->setText(commodityInf.commodityName);//商品名字
        QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
            arg(commodityInf.commodityID);
        ui->labelClothImage->setPixmap(QPixmap(str));
        ui->labelClothOriginalPrice->setText(tr("%1").arg(commodityInf.originalCost));//商品价格
        ui->labelClothSellNumber->setText(tr("%1").arg(commodityInf.saleNumber));//销量
        ui->labelClothLeftNumber->setText(tr("%1").arg(commodityInf.leftNumber));//剩余数量
        ui->lineEditClothBuyNumber->setText(QString::number(0));//购买数量
        transferClothCoupon();
        transferClothDiscount();
        transferClothColor();
        double nowPriceTemp = commodityInf.nowPrice;
        ui->labelClothNowPrice->setText(QString::number(nowPriceTemp));//设置现在的价格
        ui->labelClothTotalPrice->setText(QString::number(0));
        break;
    }
    case 1://商品类型1代表食品类
    {
        ui->stackedWidget->setCurrentIndex(9);
        ui->labelFoodName->setText(commodityInf.commodityName);//商品名字
        QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
            arg(commodityInf.commodityID);
        ui->labelFoodIMage->setPixmap(QPixmap(str));
        ui->labelFoodOriginalPrice->setText(tr("%1").arg(commodityInf.originalCost));//商品价格
        ui->labelFoodSellNumber->setText(tr("%1").arg(commodityInf.saleNumber));//销量
        ui->labelFoodLeftNumber->setText(tr("%1").arg(commodityInf.leftNumber));//剩余数量
        ui->lineEditFoodBuyNumber->setText(QString::number(0));//购买数量
        transferFoodCoupon();
        transferFoodDiscount();
        double nowPriceTemp = commodityInf.nowPrice;
        ui->labelFoodNowPrice->setText(QString::number(nowPriceTemp));//设置现在的价格
        ui->labelFoodTotalPrice->setText(QString::number(0));
        break;
    }
    case 2://商品类型2代表数码类
    {
        ui->stackedWidget->setCurrentIndex(10);
        ui->labelDigitalName->setText(commodityInf.commodityName);//商品名字
        QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
            arg(commodityInf.commodityID);
        ui->labelDigitallabel->setPixmap(QPixmap(str));
        ui->labelDigitalOriginalPrice->setText(tr("%1").arg(commodityInf.originalCost));//商品价格
        ui->labelDigitalSellNumber->setText(tr("%1").arg(commodityInf.saleNumber));//销量
        ui->labelDigitalLeftNumber->setText(tr("%1").arg(commodityInf.leftNumber));//剩余数量
        ui->lineEditDigitalBuyNumber->setText(QString::number(0));//购买数量
        transferDigitalCoupon();
        transferDigitalDiscount();
        double nowPriceTemp = commodityInf.nowPrice;
        ui->labelDigitalNowPrice->setText(QString::number(nowPriceTemp));//设置现在的价格
        ui->labelDigitalTotalPrice->setText(QString::number(0));
        break;
    }

    }
}

/*************************************************
Function Name： on_pushButtonFood_clicked
Description: 点击食品按钮
*************************************************/
void IMclient::on_pushButtonFood_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    int n = foodVBoxLayout->count();
    while(0 != n)
    {
        QWidget *p=foodVBoxLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        foodVBoxLayout->removeWidget(p);
        delete p;
        n--;
    }
    int len = foodVectorShow.size();
    qDebug() << "len" << len;
    if(len  == 0)
    {
        QMessageBox::information(nullptr, tr("提示"), tr("暂无此类商品！"), QMessageBox::Ok);
        return;
    }
    int j = 0;
    while(len > 0)
    {
//        qDebug() << foodVectorShow[i].commodityName;
        QWidget *m_livewidget = new QWidget(ui->stackedWidget);
        QHBoxLayout *layout = new QHBoxLayout;
//        layout->setAlignment(Qt::AlignLeft);
        layout->setSpacing(50);
        layout->setAlignment(Qt::AlignLeft);
        m_livewidget->setFixedHeight(300);
        m_livewidget->setFixedWidth(1000);
        m_livewidget->setLayout(layout);
        for (int i = 0; i < (len>=4?4:len); i++)
        {
            CommodityButton *button = new CommodityButton(foodVectorShow[i+j], this, 0,this);
            layout->addWidget(button);
        }
        len-=4;
        j+=4;
        foodVBoxLayout->addWidget(m_livewidget);
    }
}

/*************************************************
Function Name： on_pushButtonClothes_clicked
Description: 点击服装按钮
*************************************************/
void IMclient::on_pushButtonClothes_clicked()
{
    qDebug() << "clothes button!";
    ui->stackedWidget->setCurrentIndex(5);
    int n = clothVBoxLayout->count();
    while(0 != n)
    {
        QWidget *p=clothVBoxLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        clothVBoxLayout->removeWidget(p);
        delete p;
        n--;
    }
    int len = clothVectorShow.size();
    if(len  == 0)
    {
        QMessageBox::information(nullptr, tr("提示"), tr("暂无此类商品！"), QMessageBox::Ok);
        return;
    }
    qDebug() << "len" << len;
    int j = 0;
    while(len > 0)
    {
//        qDebug() << clothVectorShow[i].commodityName;
        QWidget *m_livewidget = new QWidget(ui->stackedWidget);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(50);
        layout->setAlignment(Qt::AlignLeft);
        m_livewidget->setFixedHeight(300);
        m_livewidget->setFixedWidth(1000);
        m_livewidget->setLayout(layout);
        for (int i = 0; i < (len>=4?4:len); i++)
        {
            CommodityButton *button = new CommodityButton(clothVectorShow[i+j], this, 0,this);
            layout->addWidget(button);
        }
        len-=4;
        j+=4;
        clothVBoxLayout->addWidget(m_livewidget);
    }
}

/*************************************************
Function Name： on_pushButtonDigital_clicked
Description: 点击数码按钮
*************************************************/
void IMclient::on_pushButtonDigital_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    int n = digitalVBoxLayout->count();
    while(0 != n)
    {
        QWidget *p=digitalVBoxLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        digitalVBoxLayout->removeWidget(p);
        delete p;
        n--;
    }
    int len = digitalVectorShow.size();
    if(len  == 0)
    {
        QMessageBox::information(nullptr, tr("提示"), tr("暂无此类商品！"), QMessageBox::Ok);
        return;
    }
    int j = 0;
    while(len > 0)
    {
//        qDebug() << digitalVectorShow[i].commodityName;
        QWidget *m_livewidget = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(50);
        layout->setAlignment(Qt::AlignLeft);
        m_livewidget->setFixedHeight(300);
        m_livewidget->setFixedWidth(1000);
        m_livewidget->setLayout(layout);
        for (int i = 0; i < (len>=4?4:len); i++)
        {
            CommodityButton *button = new CommodityButton(digitalVectorShow[i+j], this, 0,this);
            layout->addWidget(button);
        }
        len-=4;
        j+=4;
        digitalVBoxLayout->addWidget(m_livewidget);
    }
}

/*************************************************
Function Name： setCommodityVec
Description: 设置商品列表
*************************************************/
void IMclient::setCommodityVec(QVector<Commodity>& commodityVector)
{
    QLineEdit *SearchLineEdit = ui->lineEditSearch;
    completer = new QCompleter();
    completer->setFilterMode(Qt::MatchStartsWith);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    for (int i = 0 ; i < commodityVector.size(); i++)
    {
        valueList << commodityVector[i].commodityName;
    }
    listModel = new QStringListModel(valueList, this);
    completer->setModel(listModel);
    SearchLineEdit->setCompleter(completer);

    qDebug() << "set commodityList";
    commodityVec = commodityVector;//商品类型，0代表服装类，1代表食品类，2代表数码类
    for (int i = 0; i < commodityVector.size(); i++)
    {
        commodityVector[i].buyName = userInf.userName;
        commodityVector[i].buyAddress = userInf.address;
    }
    for(int i = 0; i < commodityVector.size(); i++)
    {
        commodityNameInfMap.insert(commodityVector[i].commodityName, &commodityVector[i]);
        commodityNameIDMap.insert(commodityVector[i].commodityName, commodityVector[i].commodityID);
        commodityInformationMap.insert(commodityVector[i].commodityID, &commodityVector[i]);
        qDebug() << "commodityVector[i].commodityName" << commodityVector[i].commodityName;
        if(commodityVector[i].shopType == 0)
        {
            clothVectorShow.push_back(commodityVector[i]);
        }
        if(commodityVector[i].shopType == 1)
        {
            foodVectorShow.push_back(commodityVector[i]);
        }
        if(commodityVector[i].shopType == 2)
        {
            digitalVectorShow.push_back(commodityVector[i]);
        }
    }
    on_pushButtonClothes_clicked();
}

/*************************************************
Function Name： getCommodityVector()
Description: 获取卖家所卖商品列表（发送至服务器）
*************************************************/
void IMclient::getCommodityVector()
{
    if (nullptr != m_mainCtrl)
    {
        m_mainCtrl->getAllCommodityInf();
    }
}

/*************************************************
Function Name： initIMMainWidget
Description: 初始化
*************************************************/
void IMclient::initIMMainWidget()
{
    foodVBoxLayout = new QVBoxLayout;
    clothVBoxLayout = new QVBoxLayout;
    digitalVBoxLayout = new QVBoxLayout;
    dealWithAfterBuy = new QVBoxLayout;
    readHistoryLayout = new QVBoxLayout;
    recommendLayout = new QVBoxLayout;
    shoppingCartLayout = new QVBoxLayout;
    verticalLayout = new QVBoxLayout;
    shoppingCartLayout->setAlignment(Qt::AlignTop);
    foodVBoxLayout->setAlignment(Qt::AlignTop);
    clothVBoxLayout->setAlignment(Qt::AlignTop);
    digitalVBoxLayout->setAlignment(Qt::AlignTop);
    dealWithAfterBuy->setAlignment(Qt::AlignTop);
    readHistoryLayout->setAlignment(Qt::AlignTop);
    recommendLayout->setAlignment(Qt::AlignTop);
    verticalLayout->setAlignment(Qt::AlignTop);
    foodVBoxLayout->setMargin(0);
    clothVBoxLayout->setMargin(0);
    digitalVBoxLayout->setMargin(0);
    dealWithAfterBuy->setMargin(0);
    readHistoryLayout->setMargin(0);
    recommendLayout->setMargin(0);
    shoppingCartLayout->setMargin(0);
    verticalLayout->setMargin(0);
    ui->scrollAreaWidgetContents->setLayout(readHistoryLayout);
    ui->scrollAreaWidgetContents_3->setLayout(foodVBoxLayout);
    ui->scrollAreaWidgetContents_4->setLayout(clothVBoxLayout);
    ui->scrollAreaWidgetContents_5->setLayout(digitalVBoxLayout);
    ui->scrollAreaWidgetContents_2->setLayout(dealWithAfterBuy);
    ui->scrollAreaWidgetContents_6->setLayout(recommendLayout);
    ui->scrollAreaWidgetContents_7->setLayout(shoppingCartLayout);
    ui->scrollAreaWidgetContents_8->setLayout(verticalLayout);
    connect(m_mainCtrl, SIGNAL(closeWindowSignal()),this, SLOT(closeWindow()));
    connect(m_mainCtrl, SIGNAL(getAllCommoditySuccess(QVector<Commodity> &)),
            this, SLOT(setCommodityVec(QVector<Commodity> &)));
    connect(m_mainCtrl, SIGNAL(addToMyMessageBox(QString)),this, SLOT(addToMyInformBox(QString)));
    connect(m_mainCtrl, SIGNAL(sellerChangeCommodityInf(Commodity)),this, SLOT(sellerChangeCommodity(Commodity)));
}

/*************************************************
Function Name： sellerChangeCommodity
Description: 卖家修改商品信息
*************************************************/
void IMclient::sellerChangeCommodity(Commodity commodityInformation)
{
    for(int i = 0; i < commodityVec.size(); i++)
    {
        if(commodityVec[i].commodityID == commodityInformation.commodityID)
        {
            qDebug() << "sellerChangeCommodity!!!!!!!";
            commodityVec[i] = commodityInformation;
            break;
        }
    }

}

/*************************************************
Function Name： on_pushButtonChangeUserInf_clicked
Description: 点击修改用户信息
*************************************************/
void IMclient::on_pushButtonChangeUserInf_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    showUserInformation();
}

/*************************************************
Function Name： closeWindow
Description: 关闭主窗口
*************************************************/
void IMclient::closeWindow()
{
    m_mainCtrl->closeConnect();
    close();
}

/*************************************************
Function Name： on_sureChangeUserInfButton_clicked
Description: 点击确定修改个人信息
*************************************************/
void IMclient::on_sureChangeUserInfButton_clicked()
{
    if(ui->lineEditAddress_4->text().isEmpty())
//    || ui->passwordlineEdit_4->text().isEmpty() || ui->surelineEdit_4->text().isEmpty())
    {
        QMessageBox::critical(nullptr, tr("修改信息"), tr("输入信息不能为空"));
        return ;
    }
    userInf.userName = ui->userNamelabel_4->text();
    userInf.address = ui->lineEditAddress_4->text();
    userInf.buyer.balance = ui->lineEdit->text().toDouble();
    if(nullptr != m_mainCtrl)
    {
        m_mainCtrl->updateUserInformation(userInf);
    }
//    if(userInf.logInPassWord == ui->passwordlineEdit_4->text())
//    {
//        if(ui->passwordlineEdit_4->text() == ui->surelineEdit_4->text())
//        {

//            userInf.buyer.payPassWord = ui->lineEdit_2->text();

//            userInf.logInPassWord = ui->surelineEdit_4->text();

//            if(nullptr != m_mainCtrl)
//            {
//                m_mainCtrl->updateUserInformation(userInf);
//            }
//        }
//        else
//        {
//            ui->passwordlineEdit_4->clear();
//            ui->surelineEdit_4->clear();
//            ui->passwordlineEdit_4->setFocus();
//            QMessageBox::critical(nullptr, tr("修改信息"), tr("两次输入的密码不相同！"));
//        }
//    }
//    else
//    {
//        ui->surelineEdit_4->clear();
//        ui->passwordlineEdit_4->clear();
//        ui->passwordlineEdit_4->setFocus();
//        QMessageBox::critical(nullptr, tr("修改信息"), tr("原密码错误！"));
//    }
}

/*************************************************
Function Name： on_cancelChangeUserInfButton_clicked
Description: 点击取消修改个人信息
*************************************************/
void IMclient::on_cancelChangeUserInfButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);//返回到首页
}

/*************************************************
Description: 显示个人信息
*************************************************/
void IMclient::showUserInformation()
{
//    ui->primPassWordlineEdit_4->setText(userInf.logInPassWord);
//    ui->passwordlineEdit_4->setText(userInf.logInPassWord);
//    ui->surelineEdit_4->setText(userInf.logInPassWord);
    ui->lineEditAddress_4->setText(userInf.address);
    ui->userNamelabel_4->setText(userInf.userName);
    ui->lineEdit->setText(QString::number(userInf.buyer.balance));
//    ui->lineEdit_2->setText(userInf.buyer.payPassWord);
//    ui->lineEdit_3->setText(userInf.buyer.payPassWord);
//    ui->lineEdit_4->setText(userInf.buyer.payPassWord);
}

/*************************************************
Description: CLoth点击退货
*************************************************/
void IMclient::on_pushButtonClothSalesReturn_clicked()
{
    QString temp = tr("确定想要退货？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退货"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        qDebug() << "rb == QMessageBox::Yes" << "确定想要退货？";
        m_mainCtrl->buyerSaleReturn(commodityInf);
    }
    for (int i = 0; i < commodityVec.size(); i++)
    {
        if(commodityInf.commodityID == commodityVec[i].commodityID)
        {
            commodityVec[i].saleNumber -= ui->labelAfterSaleClothBuyNumber->text().toInt();
            commodityVec[i].leftNumber += ui->labelAfterSaleClothBuyNumber->text().toInt();
            break;
        }
    }
    for (QVector<Commodity>::iterator i=haveBuyCommodityVec.begin(); i!=haveBuyCommodityVec.end(); i++)
    {
        if(i->commodityID == commodityInf.commodityID)
        {
            haveBuyCommodityVec.erase(i);
            break;
        }
    }
}

/*************************************************
Description: Food点击退货
*************************************************/
void IMclient::on_pushButtonFoodSalesReturn_clicked()
{
    QString temp = tr("确定想要退货？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退货"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        m_mainCtrl->buyerSaleReturn(commodityInf);
    }
    for (int i = 0; i < commodityVec.size(); i++)
    {
        if(commodityInf.commodityID == commodityVec[i].commodityID)
        {
            commodityVec[i].saleNumber -= ui->labelAfterSaleFoodBuyNumber->text().toInt();
            commodityVec[i].leftNumber += ui->labelAfterSaleFoodBuyNumber->text().toInt();
            break;
        }
    }
    for (QVector<Commodity>::iterator i=haveBuyCommodityVec.begin(); i!=haveBuyCommodityVec.end(); i++)
    {
        if(i->commodityID == commodityInf.commodityID)
        {
            haveBuyCommodityVec.erase(i);
            break;
        }
    }
}

/*************************************************
Description: Digital点击退货
*************************************************/
void IMclient::on_pushButtonDigitalSalesReturn_clicked()
{
    QString temp = tr("确定想要退货？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退货"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        m_mainCtrl->buyerSaleReturn(commodityInf);
    }
    for (int i = 0; i < commodityVec.size(); i++)
    {
        if(commodityInf.commodityID == commodityVec[i].commodityID)
        {
            commodityVec[i].saleNumber -= ui->labelAfterSaleDigitalBuyNumber->text().toInt();
            commodityVec[i].leftNumber += ui->labelAfterSaleDigitalBuyNumber->text().toInt();
            break;
        }
    }
    for (QVector<Commodity>::iterator i=haveBuyCommodityVec.begin(); i!=haveBuyCommodityVec.end(); i++)
    {
        if(i->commodityID == commodityInf.commodityID)
        {
            haveBuyCommodityVec.erase(i);
            break;
        }
    }
}
