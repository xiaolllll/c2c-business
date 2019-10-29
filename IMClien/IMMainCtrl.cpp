#include "IMMainCtrl.h"
#include "IMTcpSocket.h"
#include <QMessageBox>

IMMainCtrl::IMMainCtrl(UserInformation userInformation,QObject *parent) :
    QObject(parent), userInf(userInformation)
{
    m_tcpSocket = new IMTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(requestgetAllSellCommodityInf()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(closeWindow()));
}

/*************************************************
Function Name： deleteFromShopCart()
Description: 从购物车中删去商品
*************************************************/
void IMMainCtrl::deleteFromShopCart(const Commodity &commodityInformation)
{
    qDebug() << "deleteFromShopCart" << commodityInformation.commodityName;
    qDebug() << int(DELETE_SHOPPING_CART);
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << int(DELETE_SHOPPING_CART) << commodityInformation;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： updateUserInformation()
Description: 更新用户信息
*************************************************/
void IMMainCtrl::updateUserInformation(const UserInformation &userInformation)
{
    if (nullptr == m_tcpSocket)
        return;
    if (m_tcpSocket->isConnected())
    {
        requestUpdateUserInf(userInformation);
    }
}

void IMMainCtrl::closeConnect()
{
    m_tcpSocket->close();
}

/*************************************************
Function Name： closeWindow
Description: 关闭主程序
*************************************************/
void IMMainCtrl::closeWindow()
{
    if (m_tcpSocket->flag() == 0)
    {
        QMessageBox::information(nullptr, tr("系统提示"),
                              tr("您的客户端已经与服务器断开连接，请重新登录。"));
    }
    emit closeWindowSignal();
}

/*************************************************
Function Name： buyerBuyCommodity()
Description:买家点击购买商品
*************************************************/
void IMMainCtrl::buyerBuyCommodity(const UserInformation &userInf, const Commodity &commodityInformation)
{
    if (nullptr == m_tcpSocket)
        return;
    if (m_tcpSocket->isConnected())
    {
        requestBuyerBuyCommodityInf(userInf, commodityInformation);
    }
}

/*************************************************
Function Name： buyerClearCart()
Description:买家点击购买商品信息
*************************************************/
void IMMainCtrl::buyerClearCart(const UserInformation &userInf, const QVector<Commodity> &commodityInformation)
{
    if (nullptr == m_tcpSocket)
        return;
    if (m_tcpSocket->isConnected())
    {
        requestBuyerClearPayCart(userInf, commodityInformation);
    }
}

/*************************************************
Function Name： buyerSaleReturn()
Description:买家退货
*************************************************/
void IMMainCtrl::buyerSaleReturn(const Commodity &commodityInformation)
{
    if (nullptr == m_tcpSocket)
        return;
    if (m_tcpSocket->isConnected())
    {
        requestSaleReturn(commodityInformation);
    }
}

/*************************************************
Function Name： requestSaleReturn()
Description:发送买家退货的请求
*************************************************/
void IMMainCtrl::requestSaleReturn(const Commodity &commodityInformation)
{
    qDebug() << "发送买家退货的请求";
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    qDebug() << "request type is" << SALES_RETURN;
    out << quint16(0) << int(SALES_RETURN) << commodityInformation;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： requestBuyerBuyCommodityInf()
Description:发送买家购买商品的请求
*************************************************/
void IMMainCtrl::requestBuyerBuyCommodityInf(const UserInformation &userInf, const Commodity &commodityInformation)
{
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    qDebug() << "request type is" << BUY_NOW;
    out << quint16(0) << int(BUY_NOW) << commodityInformation << userInf;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    qDebug()<<"买尺寸"<<quint16(block.size() - sizeof(quint16));
    QString tem=block;
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： requestBuyerClearPayCart()
Description:发送买家清空支付车的请求
*************************************************/
void IMMainCtrl::requestBuyerClearPayCart(const UserInformation &userInf,const QVector<Commodity> &commodityInfVec)
{
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    qDebug() << "request type is" << CLEAR_SHOPPING_CART;
    out << quint16(0) << int(CLEAR_SHOPPING_CART) << userInf.userName << commodityInfVec << userInf;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    QString tem=block;
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： requestUpdateUserInf()
Description:发送更新个人信息的请求
*************************************************/
void IMMainCtrl::requestUpdateUserInf(const UserInformation &userInformation)
{
    qDebug() << int(CHANGE_USERINFORMATION);
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << int(CHANGE_USERINFORMATION) << userInformation;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： buyerJoinShoppingCart
Description: 买家将商品加入购物车
*************************************************/
void IMMainCtrl::buyerJoinShoppingCart(const Commodity &commodityInformation)
{
    if(nullptr == m_tcpSocket)
    {
        return ;
    }
    if(!m_tcpSocket->isConnected())
    {
        m_blockSize = 0;
        m_tcpSocket->requestConnect();
    }
    requestBuyerJoinShoppingCart(commodityInformation);
}

/*************************************************
Function Name： requestBuyerJoinShoppingCart
Description: 买家将商品加入购物车
*************************************************/
void IMMainCtrl::requestBuyerJoinShoppingCart(const Commodity &commodityInforamtion)
{
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    qDebug() << "JOIN_SHOPPING_CART" << commodityInforamtion.buyName;
    out << quint16(0) << int(JOIN_SHOPPING_CART) << commodityInforamtion;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： getAllSellCommodityInformation
Description: 获取卖家所有销售的商品
*************************************************/
void IMMainCtrl::getAllCommodityInf()
{
    qDebug() << "getAllSellCommodityInformation";
    if(nullptr == m_tcpSocket)
    {
        return ;
    }
    if(!m_tcpSocket->isConnected())
    {
        m_blockSize = 0;
        m_tcpSocket->requestConnect();
    }
    requestgetAllSellCommodityInf();
}

/*************************************************
Function Name： requestgetAllSellCommodityInf
Description: 发送获取卖家所有销售的商品的请求
*************************************************/
void IMMainCtrl::requestgetAllSellCommodityInf()
{
    qDebug() << "requestgetAllSellCommodityInf" << endl;
    if(nullptr == m_tcpSocket)
        return ;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << int(GET_ALL_COMMODITY) << userInf;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    qDebug() << "quint16(block.size() - sizeof(quint16))" << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： readMessage
Description: 读取信息
*************************************************/
void IMMainCtrl::readMessage()
{
    qDebug() << "readMessage";
    if (nullptr == m_tcpSocket)
        return;
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_8);
    if (m_blockSize == 0)
    {
        if (m_tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }

    if (m_tcpSocket->bytesAvailable() < m_blockSize)
        return;
    int type;

    bool first = true;
    qint64 totalSize = m_tcpSocket->bytesAvailable();

    qDebug() << "m_blockSize" << m_blockSize;
    while(totalSize > 0)
    {
        if (totalSize < (int)sizeof(quint64))
            return;
        qDebug() << "totalSize" << totalSize;
        if(!first)//如果不是第一次传输的话需要重新读取size
        {
            in >> m_blockSize;
            qDebug() << "m_blockSize" << m_blockSize;
        }
        in >> type;
        int len;
        switch (type)
        {
        case GET_ALL_COMMODITYS_SUCCESS:
        {
            qDebug() << "get all commodity success";
            in >> len;
            commodityVec.clear();
            commodityVec.resize(len);
            for(int i=0; i<len; i++)
            {
                in >> commodityVec[i];
            }
            emit getAllCommoditySuccess(commodityVec);
            qDebug() << " emit get all commodity success";
            break;
        }
        case CHANGE_USERINFORMATION_SUCCESS:
        {
            in >> userInf;
            QMessageBox::information(nullptr, tr("修改用户信息"), tr("修改用户信息成功"));
            break;
        }
        case BUY_NOW_SUCCESS://购买成功
        {
            in >> commodityInf;
            QMessageBox::information(nullptr, tr("购买商品"), tr("购买商品成功"));
            break;
        }
        case JOIN_SHOPPING_CART_SUCCESS:
        {
            in >> commodityInf;
            QMessageBox::information(nullptr, tr("加入购物车"), tr("加入购物车成功！"));
            break;
        }
        case SALES_RETURN_SUCCESS:
        {
            in >> commodityInf;
            QMessageBox::information(nullptr, tr("退货"), tr("退货成功！"));
            break;
        }
        case REMIND_BUYER_PAY_SUCCESS:
        {
            QString userNameTemp, userBuyCommodityName;
            in >> userNameTemp >> userBuyCommodityName;
            emit addToMyMessageBox(QString("卖家提醒您尽快对%1付款！").arg(userBuyCommodityName));
    //        QMessageBox::information(nullptr, tr("提示"), tr("卖家提醒您尽快对%1付款！").arg(userBuyCommodityName));
            break;
        }
        case DELETE_SHOPPING_CART_SUCCESS:
        {
            in >> commodityInf;
            QMessageBox::information(nullptr, tr("提示"), tr("删除成功！"));
            break;
        }
        case CLEAR_SHOPPING_CART_SUCCESS:
        {
            in >> commodityVec;
            QMessageBox::information(nullptr, tr("购买商品"), tr("结算成功"));
            break;
        }
        case SEND_COMMODITY_SUCCESS:
        {
            QString temp;
            in >> temp;
            emit addToMyMessageBox(QString("卖家已对%1发货").arg(temp));
    //        QMessageBox::information(nullptr, tr("发货"), tr("卖家已对%1发货").arg(temp));
            break;
        }
        case CHANGE_COMMODITY_INFORMATION_SUCCESSS:
        {
            in >> commodityInf;
            emit sellerChangeCommodityInf(commodityInf);
            break;
        }

        }

        totalSize = totalSize - m_blockSize - sizeof (quint16);
        first = false;
    }


    m_blockSize = 0;
    QByteArray data = m_tcpSocket->readAll();
    qDebug() << "leaved in socket: " << data.size();
}
