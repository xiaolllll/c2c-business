#include "thread.h"

myThread::myThread(qintptr sockDesc, QObject *parent):
    QThread (parent), m_sockDesc(sockDesc)
{

}

myThread::~myThread()
{
    deleteLater();
}

void myThread::run()
{
    qDebug() << "run";
    clientSocket = new IMClientSocketCtrl();
    clientSocket->setSocketDescriptor(m_sockDesc);
    connect(clientSocket, SIGNAL(deleteSignal(const QString &)),
        this, SLOT(receivedeleteScoket(const QString &)),Qt::BlockingQueuedConnection);
    connect(clientSocket, SIGNAL(sendSignal(const SaveTmpInformation &)),
        this, SLOT(receiveSendSignal(const SaveTmpInformation &)),Qt::BlockingQueuedConnection);

}

void myThread::receiveSendSignal(const SaveTmpInformation &temp)
{
    emit sendSignal(temp);
}
// 删除socket
void myThread::receivedeleteScoket(const QString &acc)
{
    emit deleteSignal(acc);
}
