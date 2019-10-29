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
        emit closeWindowSignal();
    }
}

/*************************************************
Function Name： readMessage
Description: 读取信息
*************************************************/
void IMMainCtrl::readMessage()
{
    qDebug() << "market read message!";
    if (nullptr == m_tcpSocket)
        return;
    QDataStream in(m_tcpSocket);
    int type;
    int len;
    in.setVersion(QDataStream::Qt_4_8);
    if (m_blockSize == 0)
    {
        if (m_tcpSocket->bytesAvailable() < (int)sizeof(quint64))
            return;
        in >> m_blockSize;
    }

    if (m_tcpSocket->bytesAvailable() < m_blockSize)
        return;//考虑分包
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

        qDebug() <<"receive message type is" << type;
        switch (type)
        {
        case SELLER_ALL_COMMODITY_INF_SUCCESS:
        {
            in >> len;
            commodityVec.clear();
            commodityVec.resize(len);
            for(int i=0; i<len; i++)
            {
                in >> commodityVec[i];
            }
            emit getAllCommoditySuccess(commodityVec);
            break;
        }
        case CHANGE_COMMODITY_INFORMATION_SUCCESSS:
        {
            in >> commodityInf;
            QMessageBox::information(nullptr, tr("修改商品信息"), tr("修改商品信息成功"));
            break;
        }
        case CHANGE_USERINFORMATION_SUCCESS:
        {
            in >> userInf;
            QMessageBox::information(nullptr, tr("修改用户信息"), tr("修改用户信息成功"));
            break;
        }
        case BUY_NOW_SUCCESS:
        {
            in >> commodityInf;
            emit getBuyerHaveBuyCommodity(commodityInf);
            break;
        }
        case SEND_COMMODITY_SUCCESS:
        {
            in >> commodityInf.commodityName;
            QMessageBox::information(nullptr, tr("发货"), tr("发货成功"));
            break;
        }
        case JOIN_SHOPPING_CART_SUCCESS:
        {
            in >> commodityInf;
            emit getBuyerHaveJoinShoppingCart(commodityInf);
            break;
        }
        case REMIND_BUYER_PAY_SUCCESS:
        {
            in >> commodityInf.buyName >> commodityInf.commodityName;
            QMessageBox::information(nullptr, tr("提示"), tr("提醒对方成功！"));
            break;
        }
        case SALES_RETURN_SUCCESS:
        {
            in >> commodityInf;
            emit addToMyMessageBox(QString("有买家将%1退货！").arg(commodityInf.commodityName));
    //        QMessageBox::information(nullptr, tr("提示"), tr("有买家将%1退货！").arg(commodityInf.commodityName));

            break;
        }
        case DELETE_SHOPPING_CART_SUCCESS:
        {
            in >> commodityInf;
            emit getBuyerHaveDeleteShoppingCart(commodityInf);
            emit addToMyMessageBox(QString("有买家将%1从购物中删除！").arg(commodityInf.commodityName));
    //        QMessageBox::information(nullptr, tr("提示"), tr("有买家将%1从购物中删除！").arg(commodityInf.commodityName));
            break;
        }
        case BUY_FROM_SHOP_CART_SUCCESS:
        {
            in >> commodityInf;
            qDebug() << "commodityInf.commodityName" << commodityInf.commodityName;
            emit getBuyerHaveBuyFromCart(commodityInf);
            break;
        }

        }

        totalSize = totalSize - m_blockSize - sizeof (quint16);
        first = false;
    }

    m_blockSize = 0;
    QByteArray data = m_tcpSocket->readAll();
    QString temp;
    qDebug() << "leaved in socket: " << data.size();
}

/*************************************************
Function Name： getAllSellCommodityInformation
Description: 获取卖家所有销售的商品
*************************************************/
void IMMainCtrl::getAllSellCommodityInformation(const QString &sellName)
{
    sellerName = sellName;
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
    if(nullptr == m_tcpSocket)
        return ;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << int(SELLER_ALL_COMMODITY_INF) << sellerName;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： remindBuyerPay()
Description: 卖家提醒买家付款
*************************************************/
void IMMainCtrl::remindBuyerPay(const Commodity &commodity)
{
    if (nullptr == m_tcpSocket)
        return;
    if (m_tcpSocket->isConnected())
    {
        m_blockSize = 0;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << int(REMIND_BUYER_PAY) << commodity.buyName << commodity.commodityName;
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));
        m_tcpSocket->write(block);
    }
}

/*************************************************
Function Name： send_Commodity()
Description: 卖家发货
*************************************************/
void IMMainCtrl::send_Commodity(const Commodity &commodity)
{
    if (nullptr == m_tcpSocket)
        return;
    if (m_tcpSocket->isConnected())
    {
        m_blockSize = 0;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << quint16(0) << int(SEND_COMMODITY) << commodity;
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));
        m_tcpSocket->write(block);
    }
}

/*************************************************
Function Name： updateUserInformation()
Description: 更新用户信息
*************************************************/
void IMMainCtrl::updateUserInformation(const UserInformation &userInformation)
{
    if (nullptr == m_tcpSocket)
        return;

    if(!m_tcpSocket->isConnected())
    {
        m_blockSize = 0;
        m_tcpSocket->requestConnect();
    }
    requestUpdateUserInf(userInformation);
}

/*************************************************
Function Name： requestUpdateUserInf()
Description:发送更新个人信息的请求
*************************************************/
void IMMainCtrl::requestUpdateUserInf(const UserInformation &userInformation)
{
    qDebug() <<"correct user Information";
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
    qDebug() <<"correct user Information end";
}


/*************************************************
Function Name： updateCommodityInformation()
Description: 卖家更新商品信息
*************************************************/
void IMMainCtrl::updateCommodityInformation(const Commodity &commodityInformation)
{
    if (nullptr == m_tcpSocket)
        return;
    if (m_tcpSocket->isConnected())
    {
        requestUpdateCommodityInf(commodityInformation);
    }
}

/*************************************************
Function Name： requestUpdateCommodityInf()
Description: 发送卖家更新商品信息的请求
*************************************************/
void IMMainCtrl::requestUpdateCommodityInf(const Commodity& commodityInformation)
{
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << int(CHANGE_COMMODITY_INFORMATION) << commodityInformation;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}
