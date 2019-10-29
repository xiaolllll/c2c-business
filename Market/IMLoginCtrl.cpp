#include "IMLoginCtrl.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QDataStream>
#include <QDateTime>
#include <QtNetwork>
#include <QInputDialog>

IMLoginCtrl::IMLoginCtrl(QObject *parent) :
    QObject(parent)
{
    m_tcpSocket = new IMTcpSocket(this);
    m_tcpSocket->setFlag(1);

    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(requestLogin()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

IMLoginCtrl::~IMLoginCtrl()
{

}

/*************************************************
Function Name： login()
Description: 发送登录信息
Input： const QString &id： 用户帐号, const QString &pwd：用户密码
*************************************************/
void IMLoginCtrl::login(const QString &id, const QString &pwd)
{
    m_kind = LOGIN;
    m_myself.userName = id;
    m_myself.logInPassWord = pwd;
    if (!m_tcpSocket->isConnected())
    {
        m_blockSize = 0;
        m_tcpSocket->requestConnect();
    }
    requestLogin();
}

/*************************************************
Function Name： requestLogin()
Description: 请求登录
*************************************************/
void IMLoginCtrl::requestLogin()
{
    if (nullptr == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << int(LOGIN) << m_myself;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

/*************************************************
Function Name： readMessage()
Description: 读取信息
*************************************************/
void IMLoginCtrl::readMessage()
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
    if(type == LOGIN_SUCCESS)//登陆成功
    {
        in >> m_myself;
        emit getLoginMessgae(true, &m_myself);
    }
    else //登陆失败
    {
        emit getLoginMessgae(false);
    }
    QByteArray data = m_tcpSocket->readAll();
    qDebug() << "leaved in socket: " << data.size();
    m_blockSize = 0;
}
