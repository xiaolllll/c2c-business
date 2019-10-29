#include "sell.h"
#include "ui_sell.h"
#include "commoditybutton.h"
#include "myinform.h"
#include "updatediscount.h"
#include "onlydiscount.h"
#include <QMessageBox>

Sell::Sell(const UserInformation me,QWidget *parent) :
    QMainWindow(parent),userInf(me),
    ui(new Ui::Sell)
{
    ui->setupUi(this);
    userInf.buyer.balance = 0;
    userInf.buyer.payPassWord = "";
//    creatXml();
//    writeXml();
    userInf.userType = 0;//代表卖家
    m_mainCtrl = new IMMainCtrl(userInf);
    setFixedSize(1111, 708);//设定固定大小
    initIMMainWidget();
    ui->label_8->setVisible(false);
    setWindowTitle("卖家窗口");
    on_buttonSellManage_clicked();//设定stack当前页
    fileName = userInf.userName + ".xml";
}

Sell::~Sell()
{
    delete ui;
}

/*************************************************
Function Name： addToMyInformBox
Description: 添加到消息盒子中
*************************************************/
void Sell::addToMyInformBox(QString temp)
{
    myMessageBox.push_back(temp);
    ui->label_8->setVisible(true);
}

/*************************************************
Function Name： on_pushButton_clicked
Description: 点击我的通知，消息盒子功能
*************************************************/
void Sell::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->label_8->setVisible(false);
    int n = ui->verticalLayout->count();
    while(0 != n)
    {
        QWidget *p=ui->verticalLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        ui->verticalLayout->removeWidget(p);
        delete p;
        n--;
    }
    int len = myMessageBox.size();
    qDebug() << "myMessageBox.size()" << myMessageBox.size();
    if(len  == 0)
    {
        QString common = "暂无通知！";
        myInform *m_inform = new myInform(common);
        ui->verticalLayout->addWidget(m_inform);
//        QMessageBox::information(nullptr, tr("提示"), tr("暂无通知！"), QMessageBox::Ok);
        return;
    }
    for(int i=0; i<len; i++)
    {
        QString common = myMessageBox[i];
        qDebug() << "myMessageBox[i]" << myMessageBox[i];
        myInform *m_inform = new myInform(common);
        ui->verticalLayout->addWidget(m_inform);
    }
}

/*************************************************
Function Name： on_pushButton_2_clicked
Description: 点击消息盒子中的删除
*************************************************/
void Sell::on_pushButton_2_clicked()
{
    myMessageBox.clear();
    on_pushButton_clicked();
}

/*************************************************
Function Name： getCommodityVector()
Description: 获取卖家所卖商品列表（发送至服务器）
*************************************************/
void Sell::getCommodityVector()
{
    if (nullptr != m_mainCtrl)
    {
        m_mainCtrl->getAllSellCommodityInformation(userInf.userName);
    }
}

void Sell::on_send_clicked()//点击发货
{
    qDebug() << "on_send_clicked";
    int showSendCommodityID = ui->sendData->currentText().toInt();
    for (QVector<Commodity>::iterator iter = sendVec.begin(); iter!=sendVec.end(); iter++)
    {
        if(showSendCommodityID == iter->commodityID)
        {
            m_mainCtrl->send_Commodity(*iter);
            sendVec.erase(iter);
            break;
        }
    }
    on_buttonSellManage_clicked();
    //并将页面恢复到初始状态
}

void Sell::on_remindPay_clicked()//点击提醒对方付款
{
    int showSendCommodityID = ui->payData->currentText().toInt();
    for (QVector<Commodity>::iterator iter = remindBuyerPayVec.begin(); iter!=remindBuyerPayVec.end(); iter++)
    {
        if(showSendCommodityID == iter->commodityID)
        {
            m_mainCtrl->remindBuyerPay(*iter);
            break;
        }
    }
}

void Sell::on_buttonReadInform_clicked()//修改商品信息
{
    ui->stackedWidget->setCurrentIndex(2);
    int n = commodityLayout->count();
    while(0 != n)
    {
        QWidget *p=commodityLayout->itemAt(0)->widget();
        p->setParent (nullptr);
        commodityLayout->removeWidget(p);
        delete p;
        n--;
    }//删除去原来的组件
    int len = commodityVec.size();
    if(len  == 0)
    {
        QString common = "暂无商品信息！";
        myInform *m_inform = new myInform(common);
        commodityLayout->addWidget(m_inform);
//        QMessageBox::information(this, tr("提示"), tr("暂无商品记录！"), QMessageBox::Ok);
        return;
    }
    for(int i=0; i<len; i++)
    {
//        if(m_commodityMap.contains(commodityVec[i].commodityID))
//            continue;
        QWidget *m_livewidget = new QWidget(this);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(20);
        layout->setAlignment(Qt::AlignLeft);
        m_livewidget->setFixedHeight(300);
        m_livewidget->setFixedWidth(1000);
        m_livewidget->setLayout(layout);
        for (int i = 0; i < (len>=4?4:len); i++)
        {
            CommodityButton *button = new CommodityButton(commodityVec[i], this, this);
            layout->addWidget(button);
        }
        len-=4;
        commodityLayout->addWidget(m_livewidget);
//        m_commodityVectorShow.push_back(button);
//        m_commodityMap.insert(commodityVec[i].commodityID, button);
        commodityInformationMap.insert(commodityVec[i].commodityID, &commodityVec[i]);
    }
}

void Sell::on_buttonCorrectInform_clicked()//修改个人信息
{
    ui->stackedWidget->setCurrentIndex(4);
    showUserInformation();//展示个人信息
}

void Sell::on_buttonSellRecord_clicked()//销售记录
{
    ui->stackedWidget->setCurrentIndex(3);
    showDailyList();//展示日销售单
}

void Sell::on_payData_currentIndexChanged(const QString &arg1)
{
//    for (int i = 0; i < remindBuyerPayVec.size(); i++)
//    {
//        QString temp = QString::number(remindBuyerPayVec[i].commodityID);
//        ui->payData->addItem(temp);
//    }
    int showRemindCommodityID = arg1.toInt();
    for (QVector<Commodity>::iterator i=remindBuyerPayVec.begin(); i!=remindBuyerPayVec.end(); i++)
    {
        if(i->commodityID == showRemindCommodityID)
        {
            Commodity *temp = i;
            ui->payName->setText(temp->commodityName);
            ui->payNumber->setText(temp->buyName);
            ui->labelPayLeft->setText(tr("剩余数量：%1").arg(temp->leftNumber));//剩余数量
            ui->lineEditPayBuyerAddress->setText(temp->buyAddress);
            break;
        }
    }
}

void Sell::on_sendData_currentIndexChanged(const QString &arg1)
{
//    for (int i = 0; i < sendVec.size(); i++)
//    {
//        QString temp = QString::number(sendVec[i].commodityID);
//        ui->sendData->addItem(temp);
//    }
    qDebug() << "on_sendData_currentIndexChanged" << arg1;
    int showSendCommodityID = arg1.toInt();
    qDebug() << "showSendCommodityID" << showSendCommodityID;
    for (QVector<Commodity>::iterator i=sendVec.begin(); i!=sendVec.end(); i++)
    {
        if(i->commodityID == showSendCommodityID)
        {
            Commodity *temp = i;
            ui->sendName->setText(temp->commodityName);
            ui->sendNumber->setText(temp->buyName);
            ui->sendLeft->setText(tr("剩余数量：%1").arg(temp->leftNumber));//剩余数量
            ui->sendNumber->setText(QString::number(temp->buyNumber));
            ui->sendPrice->setText(QString::number(temp->nowPrice));
            ui->sendSum->setText(QString::number(temp->totalPrice));
            ui->sendAddress->setText(temp->buyAddress);
            break;
        }
    }
}

void Sell::on_buttonSellManage_clicked()//销售管理
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->sendData->clear();
    ui->sendName->clear();
    ui->sendNumber->clear();
    ui->sendLeft->setText(tr("剩余数量：%1").arg(0));//剩余数量
    ui->sendNumber->clear();
    ui->sendPrice->clear();
    ui->sendSum->clear();
    ui->sendAddress->clear();
    QMap<int, Commodity*>::iterator iter;
    if(sendVec.size() > 0)
    {
        for (int i = 0; i < sendVec.size(); i++)
        {
            QString temp = QString::number(sendVec[i].commodityID);
            ui->sendData->addItem(temp);
        }
        int showSendCommodityID = ui->sendData->currentText().toInt();
        for (QVector<Commodity>::iterator i=sendVec.begin(); i!=sendVec.end(); i++)
        {
            if(i->commodityID == showSendCommodityID)
            {
                Commodity *temp = i;
                ui->sendName->setText(temp->commodityName);
                ui->sendNumber->setText(temp->buyName);
                ui->sendLeft->setText(tr("剩余数量：%1").arg(temp->leftNumber));//剩余数量
                ui->sendNumber->setText(QString::number(temp->buyNumber));
                ui->sendPrice->setText(QString::number(temp->nowPrice));
                ui->sendSum->setText(QString::number(temp->totalPrice));
                ui->sendAddress->setText(temp->buyAddress);
            }
        }
    }
    else
    {
        QString temp = "暂无记录";
        ui->sendData->addItem(temp);
    }
    ui->payData->clear();
    ui->payName->clear();
    ui->payNumber->clear();
    ui->labelPayLeft->setText(tr("剩余数量：%1").arg(0));//剩余数量
    ui->lineEditPayBuyerAddress->clear();
    if(remindBuyerPayVec.size() > 0)
    {
        for (int i = 0; i < remindBuyerPayVec.size(); i++)
        {
            QString temp = QString::number(remindBuyerPayVec[i].commodityID);
            ui->payData->addItem(temp);
        }
        int showRemindCommodityID = ui->payData->currentText().toInt();
        for (QVector<Commodity>::iterator i=remindBuyerPayVec.begin(); i!=remindBuyerPayVec.end(); i++)
        {
            if(i->commodityID == showRemindCommodityID)
            {
                Commodity *temp = i;
                ui->payName->setText(temp->commodityName);
                ui->payNumber->setText(temp->buyName);
                ui->labelPayLeft->setText(tr("剩余数量：%1").arg(temp->leftNumber));//剩余数量
                ui->lineEditPayBuyerAddress->setText(temp->buyAddress);
            }
        }
    }
    else
    {
        QString temp = "暂无记录";
        ui->payData->addItem(temp);
    }
}

void Sell::on_buttonQiut_clicked()//退出
{
    QString temp = tr("确定想要退出程序？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退出程序"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
        this->close();
}

/*************************************************
Description: 显示个人信息
*************************************************/
void Sell::showUserInformation()
{
    ui->primPassWordlineEdit->setText(userInf.logInPassWord);
    ui->passwordlineEdit->setText(userInf.logInPassWord);
    ui->surelineEdit->setText(userInf.logInPassWord);
    ui->lineEditAddress->setText(userInf.address);
    ui->userNamelineEdit->setText(userInf.userName);
}

/*************************************************
Description: 点击确认修改个人信息
*************************************************/
void Sell::on_sureChangeUserInfBtn_clicked()
{
    if(ui->lineEditAddress->text().isEmpty() || ui->userNamelineEdit->text().isEmpty()
    || ui->passwordlineEdit->text().isEmpty() || ui->surelineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, tr("修改信息"), tr("输入信息不能为空"));
        return ;
    }
    if(userInf.logInPassWord == ui->passwordlineEdit->text())
    {
        if(ui->passwordlineEdit->text() == ui->surelineEdit->text())
        {
            userInf.userName = ui->userNamelineEdit->text();
            userInf.logInPassWord = ui->surelineEdit->text();
            userInf.address = ui->lineEditAddress->text();
            if(nullptr != m_mainCtrl)
            {
                m_mainCtrl->updateUserInformation(userInf);
            }
        }
        else
        {
            ui->passwordlineEdit->clear();
            ui->surelineEdit->clear();
            ui->passwordlineEdit->setFocus();
            QMessageBox::critical(this, tr("修改信息"), tr("两次输入的密码不相同！"));
        }
    }
    else
    {
        ui->surelineEdit->clear();
        ui->passwordlineEdit->clear();
        ui->passwordlineEdit->setFocus();
        QMessageBox::critical(this, tr("修改信息"), tr("原密码错误！"));
    }
}

/*************************************************
Function Name： receiveBuyerHaveBuy
Description: 收到买家已经购买商品的通知
*************************************************/
void Sell::receiveBuyerHaveBuy(Commodity& commodityInformation)
{
    commodityInf = commodityInformation;
    sendVec.push_back(commodityInformation);
    commodityInformationMap[commodityInformation.commodityID] = &commodityInformation;
    on_buttonSellManage_clicked();
    QString temp = QString(tr("亲，又有买家购买了%1哦！\n快去看看吧！")).arg(commodityInformation.commodityName);
//    QMessageBox::information(this, tr("消息"), temp);
    addToMyInformBox(temp);
    writeXml();
}

/*************************************************
Function Name： receiveBuyerHaveBuyFromCart
Description: 收到买家已经将商品从购物车中付款的通知
*************************************************/
void Sell::receiveBuyerHaveBuyFromCart(Commodity& commodityInformation)
{
    commodityInf = commodityInformation;
    qDebug() << "receiveBuyerHaveBuyFromCart" << commodityInformation.commodityName;
    sendVec.push_back(commodityInformation);
    commodityInformationMap[commodityInformation.commodityID] = &commodityInformation;
    for (QVector<Commodity>::iterator iter = remindBuyerPayVec.begin(); iter!=remindBuyerPayVec.end(); iter++)
    {
        if(commodityInformation.commodityID == iter->commodityID)
        {
            remindBuyerPayVec.erase(iter);
            break;
        }
    }
    on_buttonSellManage_clicked();
    QString temp = QString(tr("亲，又有买家将购物车中的%1付款了哦！\n快去看看吧！")).arg(commodityInformation.commodityName);
    addToMyInformBox(temp);
    writeXml();
}

/*************************************************
Function Name： receiveBuyerHaveJoinShoopingCart
Description: 收到买家已经将商品加入购物车的通知
*************************************************/
void Sell::receiveBuyerHaveJoinShoopingCart(Commodity &commodityInformation)
{
    remindBuyerPayVec.push_back(commodityInformation);
    commodityInformationMap[commodityInformation.commodityID] = &commodityInformation;
    on_buttonSellManage_clicked();
    QString temp = QString(tr("亲，又有买家将%1加入购物车了哦！\n快去看看吧！")).arg(commodityInformation.commodityName);
    addToMyInformBox(temp);
//    QMessageBox::information(this, tr("消息"), temp);
    on_buttonSellManage_clicked();
}

/*************************************************
Function Name： receiveBuyerHaveDeleteShoopingCart
Description: 收到买家已经将商品从购物车去除的通知
*************************************************/
void Sell::receiveBuyerHaveDeleteShoopingCart(Commodity &commodityInformation)
{
    for (QVector<Commodity>::iterator i=remindBuyerPayVec.begin(); i!=remindBuyerPayVec.end(); i++)
    {
        if((i)->commodityID == commodityInformation.commodityID)
        {
            qDebug() << "shoopingCart.erase(i)" << (i)->commodityName;
            remindBuyerPayVec.erase(i);
            break;
        }
    }
    on_buttonSellManage_clicked();
}


/*************************************************
Description: 点击取消修改个人信息
*************************************************/
void Sell::on_cancelChangeUserInfBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);//回到首页
}

/*************************************************
Function Name： transferUIToCoupon
Description: 将用户输入的信息转换为优惠券
*************************************************/
void Sell::transferUIToCoupon()
{
    QString satisfy = ui->lineEditGreaterThanPrice->text();
    QString decrease = ui->lineEditMinusPrice->text();
    commodityInf.couponVec = satisfy + "-" + decrease;
}

/*************************************************
Function Name： transferUIToDiscount
Description: 将用户输入的信息转换为折扣
*************************************************/
void Sell::transferUIToDiscount()
{
    QString res;
    if(0 == discount2)
    {
        res = "1" + QString::number(discount1);
        qDebug() << "discount" << res;
        commodityInf.discountString = res;
        return;
    }
    res = "2"+QString::number(discount1)+QString::number(discount2)
            +QString::number(discount3);
    qDebug() << "discount" << res;
    commodityInf.discountString = res;
}

/*************************************************
Function Name： on_pushButton_3_clicked
Description: 点击修改统一折扣
*************************************************/
void Sell::on_pushButton_3_clicked()
{
    OnlyDiscount *onlyDiscount = new OnlyDiscount(this);
    onlyDiscount->show();
}

/*************************************************
Function Name： on_pushButton_4_clicked
Description: 点击修改不统一折扣
*************************************************/
void Sell::on_pushButton_4_clicked()
{
    UpdateDiscount *update = new UpdateDiscount(this);
    update->show();
}

/*************************************************
Function Name： setOnlyDiscount
Description: 从ui 界面读取折扣
*************************************************/
void Sell::setOnlyDiscount(int discount1Temp)
{
    discount1 = discount1Temp;
    discount2 = discount3 = 0;
    qDebug() << "discount1" << discount1 << "discount2" << discount2
             << "discount3" << discount3;
    transferUIToDiscount();
    showDiscountToUI();
}

/*************************************************
Function Name： setDiscount
Description: 从ui 界面读取折扣
*************************************************/
void Sell::setDiscount(int discount1Temp, int discount2Temp, int discount3Temp)
{
    discount1 = discount1Temp;
    discount2 = discount2Temp;
    discount3 = discount3Temp;
    qDebug() << "discount1" << discount1 << "discount2" << discount2
             << "discount3" << discount3;
    transferUIToDiscount();
    showDiscountToUI();
}

/*************************************************
Function Name： on_btnCorrectCommodityINform_clicked
Description: 修改商品信息
*************************************************/
void Sell::on_btnCorrectCommodityINform_clicked()
{
//    double discoutTemp = ui->labelDiscount->text().toDouble();
//    while(!(discoutTemp > 0 && discoutTemp < 1))
//    {
//        QMessageBox::information(this, tr("提示"), tr("输入折扣应该小于1"));
//        discoutTemp = ui->labelDiscount->text().toDouble();
//    }
//    commodityInf.discountString = discoutTemp;//设置折扣
    transferUIToCoupon();
    transferUIToDiscount();
    commodityInf.originalCost = ui->lineEditPrice_2->text().toDouble();//重新设置商品价格
    commodityInf.leftNumber = ui->lineEditLeftNumber->text().toInt();//重新设置剩余数量
    satisfyPrice = ui->lineEditMinusPrice->text().toDouble();
    decreasePrice = ui->lineEditGreaterThanPrice->text().toDouble();
    if(nullptr != m_mainCtrl)
    {
        m_mainCtrl->updateCommodityInformation(commodityInf);
    }
}

/*************************************************
Function Name： closeWindow
Description: 关闭主窗口
*************************************************/
void Sell::closeWindow()
{
    m_mainCtrl->closeConnect();
    close();
}

/*************************************************
Function Name： setCommodityVec
Description: 设置商品列表
*************************************************/
void Sell::setCommodityVec(const QVector<Commodity>& commodityVector)
{
    commodityVec = commodityVector;
    for(int i = 0; i < commodityVector.size(); i++)
    {
        commodityInformationMap.insert(commodityVector[i].commodityID, &commodityVec[i]);
    }
}

/*************************************************
Function Name： showCouponToUI
Description:券转换函数
*************************************************/
void Sell::showCouponToUI()
{
    QString str = commodityInf.couponVec;
    QStringList coupon = str.split("-");
    satisfyPrice = coupon.at(0).toDouble();
    decreasePrice = coupon.at(1).toDouble();
    ui->lineEditGreaterThanPrice->setText(QString::number(satisfyPrice));//优惠券中满
    ui->lineEditMinusPrice->setText(QString::number(decreasePrice));//优惠券中减
}

/*************************************************
Function Name： showDiscountToUI
Description:折扣转换函数
折扣格式：
1：统一折扣
之后的数字为打几折
2：不统一折扣
2988
*************************************************/
void Sell::showDiscountToUI()
{
    QString temp = commodityInf.discountString;
    if(temp.size() == 0)
    {
        ui->labelDiscount->setText("此商品暂无折扣！");
        return ;
    }
    if(temp[0] == '1')//统一折扣
    {
        discount1 = (temp.mid(1).toInt());//省略第二个参数表示从position开始截取到末尾
        discount2 = discount3 = 0;
        ui->labelDiscount->setText(tr("打%1折").arg(discount1));//commodityInf.discountString);//展示折扣
    }
    else //不统一折扣
    {
        discount1 = (temp.mid(1).toInt())/100;//省略第二个参数表示从position开始截取到末尾
        discount2 = (temp.mid(2).toInt())/10;
        discount3 = (temp.mid(3).toInt());
        if(discount2 == discount3)
        {
            //展示折扣
            ui->labelDiscount->setText(tr("第一件打%1折超过一件打%2折").arg(discount1).arg(discount2));
        }
        else
        {
            //展示折扣
            ui->labelDiscount->setText(tr("第一件打%1折第二件打%2折三件以上打%3折").arg(discount1).arg(discount2).arg(discount3));
        }
    }
}


/*************************************************
Function Name： getCommodityInformation
Description: 获取商品信息并在界面上展示
*************************************************/
void Sell::getCommodityInformation(const int& commodityID)
{
    QMap<int, Commodity*>::iterator iter;
    iter = commodityInformationMap.find(commodityID);
    if(iter != commodityInformationMap.end())
    {
        commodityInf = **iter;//得到的商品信息需要进行展示
    }
    ui->stackedWidget->setCurrentIndex(1);
    QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
        arg(commodityInf.commodityID);
    ui->label_6->setPixmap(QPixmap(str));
    ui->labelCommodityName->setText(commodityInf.commodityName);//商品名字
    ui->lineEditPrice_2->setText(tr("%1").arg(commodityInf.originalCost));//商品价格
    ui->labelSellData->setText(tr("%1").arg(commodityInf.saleNumber));//销量
    ui->lineEditLeftNumber->setText(tr("%1").arg(commodityInf.leftNumber));//剩余数量
    showCouponToUI();
    showDiscountToUI();
}

/*************************************************
Function Name： initIMMainWidget
Description: 初始化
*************************************************/
void Sell::initIMMainWidget()
{
    commodityLayout = new QVBoxLayout;
    commodityLayout->setAlignment(Qt::AlignTop);
    ui->verticalLayout->setAlignment(Qt::AlignTop);
    ui->scrollAreaWidgetContents->setLayout(commodityLayout);
    connect(m_mainCtrl, SIGNAL(closeWindowSignal()),this, SLOT(closeWindow()));
    connect(m_mainCtrl, SIGNAL(getAllCommoditySuccess(QVector<Commodity>)),
            this, SLOT(setCommodityVec(QVector<Commodity>)));
    connect(m_mainCtrl, SIGNAL(getBuyerHaveBuyCommodity(Commodity &)),
            this, SLOT(receiveBuyerHaveBuy(Commodity &)));
    connect(m_mainCtrl, SIGNAL(getBuyerHaveBuyFromCart(Commodity &)),
            this, SLOT(receiveBuyerHaveBuyFromCart(Commodity &)));
    connect(m_mainCtrl, SIGNAL(getBuyerHaveJoinShoppingCart(Commodity &)),
            this, SLOT(receiveBuyerHaveJoinShoopingCart(Commodity &)));
    connect(m_mainCtrl, SIGNAL(getBuyerHaveDeleteShoppingCart(Commodity &)),
            this, SLOT(receiveBuyerHaveDeleteShoopingCart(Commodity &)));
    connect(m_mainCtrl, SIGNAL(addToMyMessageBox(QString)),this, SLOT(addToMyInformBox(QString)));
}

//每当服务器传送信息时需要将信息写入xml文件中
bool Sell::creatXml()//创建xml文件
{
    qDebug() << "creatXml";
    QFile file(fileName);//创建文件
    if(file.exists())//判断是否存在
        return true;
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))//判断是否打开文件，用QIODevice，Truncate表示清空原来的内容
        return  false;
    QDomDocument doc;//可以访问这个对象中的每一个节点，每一个节点对应XML文件里的一个标记
//    QDomProcessingInstruction instruction;
//    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
//    doc.appendChild(instruction);//添加处理命令
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true); // 自动格式化
    writer.writeStartDocument();  // 开始文档（XML 声明,）系统会自动定义xml的版本
    writer.writeEndDocument();  // 结束文档
    file.close();  // 关闭文件
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))// 重新打开文件
        return false;
    QDomElement root = doc.createElement(QString("dailyList"));//根节点
    qDebug() << "write dailyList";
    doc.appendChild(root);//添加第一个子节点和子元素
    QTextStream out(&file);
    doc.save(out,4);//保存操作
    file.close();//关闭文件
    return true;
}

QString Sell::getDateTime(DateTimeType type)//判断需要时间类型，并返回合适的时间
{
    QDateTime datetime = QDateTime::currentDateTime();//获取现在时间
    QString date = datetime.toString("yyyy-MM-dd");
    QString time = datetime.toString("hh::mm");
    QString dateAndTime = datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type == Date)
        return  date;
    else if (type == Time)
        return  time;
    else
        return dateAndTime;
}

bool Sell::docRead()//读取xml文件
{
    qDebug() << "docRead";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))//判断是否打开文件
        return false;
    if(!doc.setContent(&file))//获取文本
    {
        qDebug() << "doc.setContent(&file)";
        file.close();
        return false;
    }
    file.close();
    return true;
}
bool Sell::docWrite()//写入xml文件并保存
{
    qDebug() << "docWrite";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))//判断是否打开文件
        return false;
    QTextStream out(&file);
    doc.save(out,4);//这里的4是缩进4格
    file.close();
    return true;
}
void Sell::writeXml()//修改xml文件
{
    qDebug() << "writeXml";
    if(docRead())//从文件中读取
    {
        QString currentDate = getDateTime(Date);//获取时间，类型为date
        QDomElement root = doc.documentElement();//获取根节点
        //根据是否有日期节点进行处理
        if(!root.hasChildNodes())
        {
            qDebug() << "!root.hasChildNodes()";
            QDomElement date = doc.createElement(QString("日期"));//添加元素
            QDomAttr curDate = doc.createAttribute("date");//添加属性
            curDate.setValue(currentDate);//设置属性值
            date.setAttributeNode(curDate);//设置属性节点
            root.appendChild(date);//添加到根节点的子节点
        }
            QDomElement date = root.lastChild().toElement();
            //根据是否已经有今天的日期进行处理
            if(date.attribute("date") == currentDate)
            {
                creatNodes(date);
            }
            else
            {
                QDomElement date = doc.createElement(QString("日期"));
                QDomAttr curDate = doc.createAttribute("date");
                curDate.setValue(currentDate);
                date.setAttributeNode(curDate);
                root.appendChild(date);
                creatNodes(date);
            }
//        }
    }
    docWrite();//写入文件
}
void Sell::creatNodes(QDomElement &date)//创建节点
{
    qDebug() << "creatNodes";
    QDomElement time = doc.createElement(QString("时间"));
    QDomAttr curTime = doc.createAttribute("time");
    curTime.setValue(getDateTime(Time));
    time.setAttributeNode(curTime);
    date.appendChild(time);
    QDomElement commodityName = doc.createElement(QString("商品名"));
    QDomElement price = doc.createElement(QString("价格"));
    QDomElement number = doc.createElement(QString("数量"));
    QDomElement sum = doc.createElement(QString("金额"));
    QDomText text;
    int numberSql = commodityInf.buyNumber;//获取商品的信息
    double priceSql = commodityInf.nowPrice, sumSql = commodityInf.totalPrice;//获取商品的信息
    QString nameSql = commodityInf.commodityName;//获取商品的信息
    text = doc.createTextNode(QString("%1").arg(nameSql));//设置name
    commodityName.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(priceSql));//设置price
    price.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(numberSql));//设置number
    number.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(sumSql));//设置sum
    sum.appendChild(text);
    time.appendChild(commodityName);
    time.appendChild(price);
    time.appendChild(number);
    time.appendChild(sum);

}
void Sell::showDailyList()//显示列表
{
    qDebug() << "showDailyList";
    ui->dailyList->clear();
    if(docRead())
    {
        qDebug() << "showDailyList docRead";
        QDomElement root = doc.documentElement();
        QString title = root.tagName();
        QListWidgetItem *titleItem = new QListWidgetItem;
        titleItem->setText(QString("-----%1-----").arg(title));
        titleItem->setTextAlignment(Qt::AlignCenter);
        ui->dailyList->addItem(titleItem);
        if(root.hasChildNodes())
        {
            QString currentDate = getDateTime(Date);
            QDomElement dateElement = root.lastChild().toElement();
            QString date = dateElement.attribute("date");
            if(date == currentDate)
            {
                ui->dailyList->addItem("");
                ui->dailyList->addItem(QString(QObject::tr("日期:")+"%1").arg(date));
                ui->dailyList->addItem("");
                QDomNodeList children = dateElement.childNodes();

                //遍历当日销售的所有商品
                for(int i = 0;i<children.count();i++)
                {
                    QDomNode node = children.at(i);
                    QString time = node.toElement().attribute("time");
                    QDomNodeList list = node.childNodes();
                    QString commodityName = list.at(0).toElement().text();
                    QString price = list.at(1).toElement().text();
                    QString number = list.at(2).toElement().text();
                    QString sum = list.at(3).toElement().text();
                    QString str = time + QObject::tr("出售") + commodityName + " " +
                            number + QObject::tr("个，") + QObject::tr("商品价格：")
                            + price + QObject::tr("元,买家付款") + sum + QObject::tr("元");
                    QListWidgetItem *tempItem = new QListWidgetItem;
                    tempItem->setText("************************************************************************************");
                    tempItem->setTextAlignment(Qt::AlignCenter);
                    ui->dailyList->addItem(tempItem);
                    ui->dailyList->addItem(str);
                }
            }
        }
    }
}
