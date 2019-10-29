/*************************************************
Class Name： IMLoginCtrl
Description: 用于控制登录的类
*************************************************/
#ifndef IMLOGINCTRL_H
#define IMLOGINCTRL_H

#include <QObject>
#include "IMTcpSocket.h"

class IMTcpSocket;

class IMLoginCtrl : public QObject
{
    Q_OBJECT
public:
    explicit IMLoginCtrl(QObject *parent = nullptr);

    ~IMLoginCtrl();

    // 发送登录信息
    void login(const QString &id ,const QString & pwd = "");
    IMTcpSocket *m_tcpSocket;

signals:
    void getLoginMessgae(const UserInformation* me,bool isLogin);

public slots:


private slots:
    // 发送登录请求
    void requestLogin();

    // 读取信息
    void readMessage();

private:

private:
    QVector<Commodity> commodityVec;
    int m_kind;
    quint16 m_blockSize;

    UserInformation m_myself;
    QString m_id;
};

#endif // IMLOGINCTRL_H
