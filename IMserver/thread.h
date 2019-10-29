#ifndef THREAD_H
#define THREAD_H

#include "IMClientSocketCtrl.h"
#include "QtNetwork"

class myThread : public QThread
{
    Q_OBJECT
public:
    IMClientSocketCtrl *clientSocket;
    myThread(qintptr sockDesc, QObject *parent);
    ~myThread();
signals:
    // 请求server处理
    void sendSignal(const SaveTmpInformation &temp);
    // 用户登录
    void userLoginSignal(const UserInformation &user);
    //用户断开连接了
    void deleteSignal(const QString &acc);
public slots:
    // 接收信息
    void receiveSendSignal(const SaveTmpInformation &temp);
    // 删除socket
    void receivedeleteScoket(const QString &acc);
private:
    void run();

    qintptr m_sockDesc;
};

#endif // THREAD_H
