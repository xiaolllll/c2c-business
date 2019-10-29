#include "IMTcpServer.h"
#include "thread.h"

IMTcpServer::IMTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    // 初始化一下数据库
}

IMTcpServer::~IMTcpServer()
{

}

/*************************************************
Function Name： clientDisconnected()
Description: 客户端断开连接
*************************************************/
void IMTcpServer::clientDisconnected(const QString &id)
{
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(id);
    if(iter != m_userMap.end())
    {
        m_userMap.remove(id);
    }
    emit haveLeftID(id);
}

/*************************************************
Function Name： sendMessage()
Description: 处理请求，要求clientSocket发送信息
*************************************************/
void IMTcpServer::serverSendMessage(const SaveTmpInformation &save)
{
    m_save.m_requestKind = save.m_requestKind;
    qDebug() << "server deal with message" << "type is " << m_save.m_requestKind;
    switch (m_save.m_requestKind)
    {
    case LOGIN://登录
    {
        qDebug() << "server sendmessage logIn";
        m_save.userInf = save.userInf;
        m_save.m_replyKind = m_database.searchUserInformation(m_save.userInf);
        save.m_clientSocket->sendMessage(m_save);//write信息
        break;
    }
    case REGISTER: // 注册(之后可以增加userID根据注册的时间生成)
    {
        qDebug() << "register!";
        m_save.userInf = save.userInf;
        m_save.m_clientSocket = save.m_clientSocket;
        m_save.userInf.image = 0;
        // 访问数据库 注册
        m_save.m_replyKind = m_database.addUserInformation(m_save.userInf);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case CHANGE_USERINFORMATION://修改用户信息
   {
        qDebug() <<"TCP serverCHANGE_USERINFORMATION";
        m_save.userInf = save.userInf;
        m_save.m_replyKind = m_database.updateUserInformation(m_save.userInf);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case CHANGE_COMMODITY_INFORMATION://卖家修改商品信息
    {
        qDebug() << "CHANGE_COMMODITY_INFORMATION";
        m_save.commodityInf = save.commodityInf;
        m_save.m_replyKind = m_database.updateCommodityInformation(m_save.commodityInf);
        save.m_clientSocket->sendMessage(m_save);
        //这里需要修改，因为卖家修改商品信息，需要对所有的买家全部都要修改商品信息
        if(CHANGE_COMMODITY_INFORMATION_SUCCESSS == m_save.m_replyKind)
        {
            qDebug() << "tellBuyerChange";
            tellBuyerChange();
        }
        break;
    }
    case GET_ALL_COMMODITY://获取所有商品
    {
        m_save.m_clientSocket = save.m_clientSocket;
        if(!m_userMap.contains(m_save.userInf.userName))
        {
            m_userMap.insert(m_save.userInf.userName,m_save.m_clientSocket);
            emit haveNewID(m_save.userInf.userName);
        }
        m_save.m_replyKind = m_database.searchAllCommodityInformation(m_save.commodityVecInf);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case SELLER_ALL_COMMODITY_INF://卖家获取他所卖的所有商品
    {
        m_save.m_clientSocket = save.m_clientSocket;
        if(!m_userMap.contains(m_save.userInf.userName))
        {
            m_userMap.insert(m_save.userInf.userName,m_save.m_clientSocket);
            emit haveNewID(m_save.userInf.userName);
        }
        m_save.m_replyKind = m_database.searchSellerSellAllCommodityInf(m_save.userInf.userName,m_save.commodityVecInf);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case BUY_NOW://立即购买，这里也同样是，买家购买商品和加入购物车之后需要根据商品对卖家进行通知
    {
        //既要更新商品信息也要更新用户信息
        m_save.commodityInf = save.commodityInf;
        m_save.userInf = save.userInf;
        m_database.updateUserInformation(m_save.userInf);
        m_database.updateCommodityInformation(m_save.commodityInf);
        m_save.m_replyKind = BUY_NOW_SUCCESS;
        save.m_clientSocket->sendMessage(m_save);
        tellSellerBuy(m_save.commodityInf.sellName);
        break;
    }
    case JOIN_SHOPPING_CART://加入购物车
    {
        m_save.commodityInf = save.commodityInf;
        m_save.m_replyKind = m_database.addToShoppingCart(m_save.commodityInf);
        save.m_clientSocket->sendMessage(m_save);
        if(JOIN_SHOPPING_CART_SUCCESS == m_save.m_replyKind)
        {
            tellSellerJoinShopingCart(m_save.commodityInf.sellName);
        }
        break;
    }
    case REMOVE_FROM_SHOPPONG_CART:           //商品从购物车删去
    {
        m_save.commodityInf = save.commodityInf;
        m_save.m_replyKind = m_database.removeFormShoppingCart(m_save.commodityInf);
        if(REMOVE_FROM_SHOPPONG_CART_SUCCESS == m_save.m_replyKind)
        {
            tellSelleRemoveFromShoppingCart(m_save.commodityInf.sellName);
        }
        break;
    }
    case CLEAR_SHOPPING_CART:            //清空购物车
    {
        m_save.shoppingCart = save.shoppingCart;
        m_save.userInf.userName = save.userInf.userName;
        m_save.userInf = save.userInf;
        m_database.updateUserInformation(m_save.userInf);
        QVector<Commodity>::iterator iter;
        m_save.m_replyKind = m_database.clearShopingCart(m_save.shoppingCart, m_save.userInf.userName);
        for (iter=m_save.shoppingCart.begin();iter!=m_save.shoppingCart.end();iter++)
        {
            m_save.commodityInf = *iter;
            m_save.m_replyKind = BUY_FROM_SHOP_CART_SUCCESS;
            tellSellerBuyFromCart(iter->sellName);
        }
        break;
    }
    case REMIND_BUYER_PAY://提醒对方付款
    {
        m_save.commodityInf.commodityName = save.commodityInf.commodityName;
        m_save.commodityInf.buyName = save.commodityInf.buyName;
        m_save.m_replyKind = REMIND_BUYER_PAY_SUCCESS;
        m_save.m_clientSocket->sendMessage(m_save);
        sellerRemindBuyerPay(m_save.commodityInf.buyName);
        break;
    }
    case SEND_COMMODITY:                 // 发货
    {
        m_save.m_replyKind = SEND_COMMODITY_SUCCESS;
        m_save.commodityInf.buyName = save.commodityInf.buyName;
        m_save.commodityInf.commodityName = save.commodityInf.commodityName;
        save.m_clientSocket->sendMessage(m_save);
        tellSellerSendCommodity(m_save.commodityInf.buyName);
        break;
    }
    case SALES_RETURN:                   //退货（需要在服务器中对对应类型商品的数量进行增加）
    {
        m_save.commodityInf = save.commodityInf;
        m_database.updateCommodityInformation(save.commodityInf);
        m_save.m_replyKind = SALES_RETURN_SUCCESS;
        m_save.m_clientSocket->sendMessage(m_save);
        tellSellerSaleReturn(m_save.commodityInf);
        break;
    }
    case DELETE_SHOPPING_CART:
    {
        m_save.commodityInf = save.commodityInf;
        m_save.m_replyKind = m_database.removeFormShoppingCart(save.commodityInf);
        m_save.m_clientSocket->sendMessage(m_save);
        tellSelleRemoveFromShoppingCart(m_save.commodityInf.sellName);
        break;
    }

    }
    qDebug() << "TCPServer sendMessage End!";
}

/*************************************************
Function Name： tellSellerSendCommodity()
Description:  通知卖家卖家发货
*************************************************/
void IMTcpServer::tellSellerSendCommodity(const QString &buyName)
{
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(buyName);
    if(iter != m_userMap.end())
    {
        iter.value()->sendMessage(m_save);
    }
}

/*************************************************
Function Name： tellSellerSaleReturn()
Description:  通知卖家卖家退货
*************************************************/
void IMTcpServer::tellSellerSaleReturn(const Commodity &commodityInf)
{
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(commodityInf.sellName);
    if(iter != m_userMap.end())
    {
        iter.value()->sendMessage(m_save);
    }
}

/*************************************************
Function Name： tellSellerJoinShopingCart()
Description:  通知卖家买家加入购物车的商品信息
*************************************************/
void IMTcpServer::sellerRemindBuyerPay(const QString &sellName)
{
    m_save.m_replyKind = REMIND_BUYER_PAY_SUCCESS;
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(sellName);
    if(iter != m_userMap.end())
    {
        iter.value()->sendMessage(m_save);
    }
}

/*************************************************
Function Name： tellSellerJoinShopingCart()
Description:  通知卖家买家加入购物车的商品信息
*************************************************/
void IMTcpServer::tellSellerJoinShopingCart(const QString& sellName)
{
    m_save.m_replyKind = JOIN_SHOPPING_CART_SUCCESS;
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(sellName);
    if(iter != m_userMap.end())
    {
        iter.value()->sendMessage(m_save);
    }
}

/*************************************************
Function Name： tellSelleRemoveFromShoppingCart()
Description:  通知卖家买家将商品从购物车去除
*************************************************/
void IMTcpServer::tellSelleRemoveFromShoppingCart(const QString& sellName)
{
    m_save.m_replyKind = DELETE_SHOPPING_CART_SUCCESS;
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(sellName);
    if(iter != m_userMap.end())
    {
        iter.value()->sendMessage(m_save);
    }
}

/*************************************************
Function Name： tellBuyerChange()
Description:  通知买家，卖家改变商品信息
*************************************************/
void IMTcpServer::tellBuyerChange()
{
    m_save.m_replyKind = CHANGE_COMMODITY_INFORMATION_SUCCESSS;
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    for (iter = m_userMap.begin(); iter != m_userMap.end(); iter++)
    {
        qDebug() <<  "iter.key()" << iter.key();
        bool kind = m_database.judgeIfBuyer(iter.key());
        qDebug() << ((kind == true) ? "true" : "false");
        if(kind)
        {
            qDebug() <<  "iter.key()->sendMessage(m_save);" << iter.key();
            iter.value()->sendMessage(m_save);
        }
    }
}

/*************************************************
Function Name： tellSellerBuy()
Description:  通知卖家，买家购买商品
*************************************************/
void IMTcpServer::tellSellerBuyFromCart(const QString& sellName)
{
    m_save.m_replyKind = BUY_FROM_SHOP_CART_SUCCESS;
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(sellName);
    if(iter != m_userMap.end())
    {
        qDebug() << "tellSellerBuyFromCart" << m_save.commodityInf.commodityName;
        iter.value()->sendMessage(m_save);
    }
}

/*************************************************
Function Name： tellSellerBuy()
Description:  通知卖家，买家购买商品
*************************************************/
void IMTcpServer::tellSellerBuy(const QString& sellName)
{
    //通过货号在数据库中寻找商家信息，商家再sendmessage
    m_save.m_replyKind = BUY_NOW_SUCCESS;
    QMap<QString, IMClientSocketCtrl*>::iterator iter;
    iter = m_userMap.find(sellName);
    if(iter != m_userMap.end())
    {
        iter.value()->sendMessage(m_save);
    }
}

/*************************************************
Function Name： incomingConnection()
Description: 当有新的连接时，会调用此函数。用于创建新的线程去控制socket通信
Input： int socketDescriptor：socket标识，用于创建socket
*************************************************/
void IMTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "have a new client connect！";
    //建立新连接，并连接相应的信号糟
//    myThread* t = new myThread(socketDescriptor, this);
//    qDebug() << "mythread end";
//    t->start();
//    qDebug() << " thread start end";
    IMClientSocketCtrl *clientSocket = new IMClientSocketCtrl(this);
    clientSocket->setSocketDescriptor(socketDescriptor);
    connect(clientSocket, SIGNAL(deleteSignal(const QString &)),
        this, SLOT(clientDisconnected(const QString &)));
    connect(clientSocket, SIGNAL(sendSignal(const SaveTmpInformation &)),
        this, SLOT(serverSendMessage(const SaveTmpInformation &)));
//     qDebug() <<"new client IP:" << t->clientSocket->peerAddress();
//     qDebug() << "userMap-size:" << m_userMap.size();
}
