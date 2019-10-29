#include "IMTcpSocket.h"
#include <QMessageBox>
#include <QtNetwork>
#include <QDebug>

QHostAddress IMTcpSocket::s_hostAddress = QHostAddress::Any;
int IMTcpSocket::s_hostPort = 1111;

IMTcpSocket::IMTcpSocket(QObject *parent):
    QTcpSocket(parent)
{
    m_flag = 0;
    m_isConnected = false;
    linkSignalWithSlot();
}

/*************************************************
Function Name： requestConnect()
Description: 请求连接
*************************************************/
void IMTcpSocket::requestConnect()
{
    qDebug() << "requestConnect";
    if (!m_isConnected)
    {
        QString IP;
        int port;
        //获取IP地址
        QList<QHostAddress> list = QNetworkInterface::allAddresses();
        foreach (QHostAddress address, list)
        {
            if(address.protocol() == QAbstractSocket::IPv4Protocol)
            {
                if(address.toString().contains("127.0.")) continue;

                 IP = address.toString();
            }
            else if (address.isNull())  // 主机地址为空
                continue;
        }

        //获取端口号
        port = 1111;

        //取消已有的连接
        abort();
        IP = "127.0.0.1";
        //连接服务器
        qDebug()<<"fuwuqi"<<IP;
        connectToHost(IP, port);
//        abort();
//        connectToHost(IMTcpSocket::s_hostAddress, IMTcpSocket::s_hostPort);
        qDebug() << "requestConnectend\n";
    }
}

/*************************************************
Function Name： isConnected()
Description: 是否连接
*************************************************/
bool IMTcpSocket::isConnected()
{
    return m_isConnected;
}

/*************************************************
Function Name： setFlag()
Description: 是否标识
*************************************************/
void IMTcpSocket::setFlag(int flag)
{
    m_flag = flag;
}

/*************************************************
Function Name： flag()
Description: 获取标识
*************************************************/
int IMTcpSocket::flag()
{
    return m_flag;
}

/*************************************************
Function Name： getCurrentDateTime()
Description: 获取当前的日期和时间
*************************************************/
QString IMTcpSocket::getCurrentDateTime()
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();

    return QString("%1  %2").arg(date.toString(Qt::ISODate))
        .arg(time.toString(Qt::ISODate));
}

/*************************************************
Function Name： connectionClosedByServer()
Description: 连接被客户端关闭
*************************************************/
void IMTcpSocket::connectionClosed()
{
    m_isConnected = false;
    qDebug("连接断开");
    deleteLater();
}

/*************************************************
Function Name： connectionCreate()
Description: 连接创建
*************************************************/
void IMTcpSocket::connectionCreate()
{
    m_isConnected = true;
}

/*************************************************
Function Name： dispalyError()
Description: 显示错误
Input： 错误信息
*************************************************/
void IMTcpSocket::dispalyError(QAbstractSocket::SocketError socketError)
{
    m_isConnected = false;
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        emit showConnectionStatus(tr("链接失败.可能是因为服务器关闭."));
        break;
    case QAbstractSocket::HostNotFoundError:
        emit showConnectionStatus(tr("链接失败.可能是因为找不到服务器"));
        QMessageBox::information(nullptr, tr("IM Client"),
                                 tr("This host was not found.Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit showConnectionStatus(tr("链接失败.可能是因为连接被拒绝"));
        QMessageBox::information(nullptr, tr("连接失败"),
                                 tr("The connection was refused by the peer."
                                    "Make sure the IM server is running,"
                                    "and check that the host name and port"
                                    "settings are correct."));
        break;
    default:
        emit showConnectionStatus(tr("链接失败: %1.").arg(errorString()));
        QMessageBox::information(nullptr, tr("IM Client"),
                                 tr("The following errot occurred: %1.")
                                 .arg(errorString()));
    }
}

/*************************************************
Function Name： linkSignalWithSlot()
Description: 链接信号与槽
*************************************************/
void IMTcpSocket::linkSignalWithSlot()
{
    connect(this, SIGNAL(connected()), this, SLOT(connectionCreate()));
    connect(this, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(dispalyError(QAbstractSocket::SocketError)));
}

