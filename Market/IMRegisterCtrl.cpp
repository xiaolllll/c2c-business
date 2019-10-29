#include "IMRegisterCtrl.h"
#include "IMTcpSocket.h"
#include "sell.h"
#include <QMessageBox>

IMRegisterCtrl::IMRegisterCtrl(QObject *parent) :
    QObject(parent)
{
    m_tcpSocket = new IMTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(requestRegister()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

/*************************************************
Function Name： registerAccount()
Description: 注册
*************************************************/
void IMRegisterCtrl::registerAccount(const UserInformation &user)
{
    m_user = user;
    if (!m_tcpSocket->isConnected())
    {
        m_blockSize = 0;
        m_tcpSocket->requestConnect();
    }
    requestRegister();
}

/*************************************************
Function Name： requestRegister()
Description: 请求注册
*************************************************/
void IMRegisterCtrl::requestRegister()
{
    qDebug() << "requestRegister";
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << int(REGISTER)  << m_user;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： readMessage()
Description: 读取信息
*************************************************/
void IMRegisterCtrl::readMessage()
{
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

    in >> type;
    switch (type)
    {
    case REGISTER_SUCCESS:
    {
        QMessageBox::about(nullptr, tr("注册结果"),
              tr("%1, 你好！ 恭喜,注册成功. ").arg(m_user.userName));
        Sell *sell = new Sell(m_user);
        sell->getCommodityVector();//获取卖家的商品列表
        sell->show();
        break;
    }
    case REGISTER_FAIL:
    {
        QMessageBox::about(nullptr, tr("注册结果"), tr("注册失败"));
        break;
    }
    }
}
