#include "IMClientSocketCtrl.h"
#include <QMultiMap>
#include <QDateTime>

IMClientSocketCtrl::IMClientSocketCtrl(QObject *parent):
    QTcpSocket(parent)
{
    qDebug() << "IMClientSocketCtrl";
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteScoket()));
    m_blockSize = 0;
    m_save.m_clientSocket = this;
}

IMClientSocketCtrl::~IMClientSocketCtrl()
{
    waitForDisconnected();
//    if (m_database.isOpen())
//        m_database.close();
}

/*************************************************
Function Name： receiveMessage()
Description: 接收用户发送的信息
*************************************************/
void IMClientSocketCtrl::receiveMessage()
{
    qDebug() << "receiveMessage";
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);
    if (m_blockSize == 0)
    {
        if (bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }

    qDebug()<<"收after"<<m_blockSize;
    if (bytesAvailable() < m_blockSize)
        return;
    bool first = true;
    qint64 totalSize = bytesAvailable();

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

        in >> m_save.m_requestKind;
        qDebug() << "receive message:"<< m_save.m_requestKind;

        switch (m_save.m_requestKind)
        {
        case LOGIN://登陆
        {
            in >> m_save.userInf;
            qDebug() << "licent socket logIN" <<  m_save.userInf.userName;
            break;
        }
        case REGISTER://注册
        {
            in >> m_save.userInf;
            break;
        }
        case CHANGE_USERINFORMATION://修改用户信息
        {
            in >> m_save.userInf;
            break;
        }
        case CHANGE_COMMODITY_INFORMATION://修改商品信息
        {
            in >> m_save.commodityInf;
            break;
        }
        case GET_ALL_COMMODITY://买家获取所有某类商品信息
        {
            in >> m_save.userInf;
            break;
        }
        case BUY_NOW://购买
        {
            in >> m_save.commodityInf >> m_save.userInf;
            break;
        }
        case JOIN_SHOPPING_CART://加入购物车
        {
            in >> m_save.commodityInf;
            break;
        }
        case REMOVE_FROM_SHOPPONG_CART:           //商品从购物车删去
        {
            in >>m_save.shoppingCart;
            break;
        }
        case CLEAR_SHOPPING_CART:            //清空购物车
        {
            in >> m_save.userInf.userName >> m_save.shoppingCart >> m_save.userInf;
            break;
        }
        case REMIND_BUYER_PAY://提醒买家付款
        {
            in >> m_save.commodityInf.buyName >> m_save.commodityInf.commodityName;
            break;
        }
        case SEND_COMMODITY://发货
        {
            in >> m_save.commodityInf;
            break;
        }
        case SELLER_ALL_COMMODITY_INF://卖家获取卖家所卖的所有商品
        {
            in >> m_save.userInf.userName;
            break;
        }
        case DELETE_SHOPPING_CART:
        {
            in >> m_save.commodityInf;
            break;
        }
        case SALES_RETURN:
        {
            in >> m_save.commodityInf;
            break;
        }

        }
        totalSize = totalSize - m_blockSize - sizeof (quint16);
        first = false;
    }


    QByteArray data = this->readAll();
    qDebug() << "leaved in socket: " << data.size();
    m_blockSize =0;
    qDebug() << "emit sendsignal!";
    emit sendSignal(m_save);
}

/*************************************************
Function Name： sendMessage()
Description: 发送信息（需要更改，发送的信息有的需要发送给买家有的需要发送给买家）
*************************************************/
void IMClientSocketCtrl::sendMessage(const SaveTmpInformation &temp)
{
    qDebug() << "client Server write begin!";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << temp.m_replyKind;
    qDebug() << "client server type" << temp.m_replyKind;
    switch (temp.m_replyKind)
    {
    case LOGIN_SUCCESS://登陆成功
    {
        out << temp.userInf;
        qDebug() << "serverClient logIn successfully!";
        break;
    }
    case REGISTER_SUCCESS://注册成功
    {
        qDebug() << "user Register success!";
        out << temp.userInf;
        break;
    }
    case CHANGE_USERINFORMATION_SUCCESS:// 修改用户信息成功
    {
        out << temp.userInf;
        break;
    }
    case CHANGE_COMMODITY_INFORMATION_SUCCESSS://修改商品信息成功
    {
        out << temp.commodityInf;
        break;
    }
    case GET_ALL_COMMODITYS_SUCCESS://获取所有商品成功
    {
        int len = temp.commodityVecInf.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.commodityVecInf[i];
        break;
    }
    case BUY_NOW_SUCCESS://成功购买
    {
        out << temp.commodityInf;
        break;
    }
    case JOIN_SHOPPING_CART_SUCCESS:
    {
        out << temp.commodityInf;
        break;
    }
    case REMOVE_FROM_SHOPPONG_CART_SUCCESS:
    {
        out << temp.commodityInf;
        break;
    }
    case REMIND_BUYER_PAY_SUCCESS:
    {
        out << temp.commodityInf.buyName << temp.commodityInf.commodityName;
        break;
    }
    case SELLER_ALL_COMMODITY_INF_SUCCESS://卖家获得卖家所卖的所有商品信息
    {
        int len = temp.commodityVecInf.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.commodityVecInf[i];
        break;
    }
    case SEND_COMMODITY_SUCCESS:
    {
        out << temp.commodityInf.commodityName;
        break;
    }
    case SALES_RETURN_SUCCESS://成功退货
    {
        out << temp.commodityInf;
        break;
    }
    case DELETE_SHOPPING_CART_SUCCESS:
    {
        qDebug() << "DELETE_SHOPPING_CART_SUCCESS" << temp.commodityInf.commodityName;
        out << temp.commodityInf;
        break;
    }
    case CLEAR_SHOPPING_CART_SUCCESS:
    {
        out << temp.shoppingCart;
        break;
    }
    case BUY_FROM_SHOP_CART_SUCCESS :
    {
        out << temp.commodityInf;
        qDebug() << "BUY_FROM_SHOP_CART_SUCCESS" << temp.commodityInf.commodityName;
        break;
    }

    }
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    write(block);
    qDebug() << "client Server write end!";
}

/*************************************************
Function Name： deleteScoket()
Description: 删除socket
*************************************************/
void IMClientSocketCtrl::deleteScoket()
{
    emit deleteSignal(m_save.userInf.userName);
    deleteLater();
}


