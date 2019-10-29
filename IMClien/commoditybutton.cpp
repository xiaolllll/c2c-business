#include "commoditybutton.h"
#include <QMenu>
#include <QBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

CommodityButton::CommodityButton(const Commodity& commodity, IMclient *sellInf, const int typeTemp, QWidget *parent):
    QPushButton (parent), type(typeTemp)
{
    commodityInf = commodity;
    sell = sellInf;
    QVBoxLayout *layout = new QVBoxLayout;
    m_labelHead = new QLabel("图像暂无");
    m_labelHead->setFixedSize(250, 250);
    m_labelMes = new QLabel;
    m_labelMes->setFixedSize(250, 40);
    setButtonStatus();//初始化商品的信息
    layout->setMargin(0);
    layout->addWidget(m_labelHead);
    layout->addWidget(m_labelMes);
    layout->addStretch();
    setLayout(layout);
//    setFixedSize(400, 190);
    setMinimumSize(QSize(250,360));
}

/*************************************************
Function Name： setButtonStatus()
Description:  设置button显示信息
*************************************************/
void CommodityButton::setButtonStatus()
{//:/new/icon/srcs/movieclicked.png
    //设置头像头像的图片从文件中修改
    QString str = QString("C:/Users/Administrator/Desktop/image/%1.jpg").
        arg(commodityInf.commodityID);
    m_labelHead->setPixmap(QPixmap(str));
    QString text = QString("<font color=red>%1<br>￥%2<br>原价：￥%3</font>").
            arg(commodityInf.commodityName).arg(commodityInf.originalCost).arg(commodityInf.originalCost*1.5);
    m_labelMes->setText(text);
}

/*************************************************
Function Name： mousePressEvent
Description: 鼠标单击事件
*************************************************/
void CommodityButton::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mouse Event";
    if(event->button() == Qt::LeftButton)
    {
        onClickButton();
    }
}

/*************************************************
Function Name： onClickButton()
Description: 点击鼠标，点击之后显示具体的信息
*************************************************/
void CommodityButton::onClickButton()
{
    switch (type)
    {
    case 0://购买商品
    {
        sell->getCommodityInformation(commodityInf.commodityID);
        break;
    }
    case 1://售后处理
    {
        sell->DealWithAfterBuy(commodityInf.commodityID);
        break;
    }

    }
}
