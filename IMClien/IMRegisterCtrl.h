/*************************************************
Class Name： IMLoginCtrl
Description: 用于控制注册的类
*************************************************/
#ifndef IMREGISTERCTRL_H
#define IMREGISTERCTRL_H

#include <QObject>
#include "IMTcpSocket.h"

class IMTcpSocket;

class IMRegisterCtrl:public QObject
{
    Q_OBJECT
public:
    explicit IMRegisterCtrl(QObject *parent = nullptr);
    // 注册
    void registerAccount(const UserInformation &);

private slots:
    // 读取信息
    void readMessage();
    //请求注册
    void requestRegister();

signals:
//    void getRegisterMessgae(const UserInformation me,bool isRegister);

private:
    UserInformation m_user;
    quint16 m_blockSize;
    IMTcpSocket *m_tcpSocket;
};

#endif // IMREGISTERCTRL_H
