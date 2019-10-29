#ifndef IMTCPSOCKET_H
#define IMTCPSOCKET_H

/*************************************************
Class Name： IMTcpSocket
Description: 客户端tcpsocket类
*************************************************/
#include <QTcpSocket>
#include "IMConstant.h"

class IMTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    IMTcpSocket(QObject *parent = nullptr);
    // 请求连接
    void requestConnect();
    // 是否连接
    bool isConnected();

    void setFlag(int);
    int flag();

    //获取当前的日期和时间
    static QString getCurrentDateTime();

    static QHostAddress s_hostAddress;
    static int s_hostPort;

signals:
    void showConnectionStatus(const QString &, bool isLogin = false);

private slots:
    // 连接被客户端关闭
    void connectionClosed();
    // 连接创建
    void connectionCreate();
    // 显示错误
    void dispalyError(QAbstractSocket::SocketError);

private:
    // 链接信号与槽
    void linkSignalWithSlot();


    bool m_isConnected;

    int m_flag;

};

#endif // IMTCPSOCKET_H
