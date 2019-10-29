#include "IMServerMessageCtrl.h"
#include "IMTcpServer.h"
#include "IMClientSocketCtrl.h"
#include <QMessageBox>
#include <QDateTime>
#include <QStringListModel>
#include <QListView>
#include <QStringList>

IMServerMessageCtrl::IMServerMessageCtrl(QObject *parent) :
    QObject(parent)
{
    m_modelID = new QStringListModel(this);
    m_modelID->setStringList(m_listID);
    m_listViewID = new QListView;
    m_listViewID->setModel(m_modelID);
    // 不可编辑
    m_listViewID->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_modelAddress = new QStringListModel(this);
    m_modelAddress->setStringList(m_listAddress);
    m_listViewAddress = new QListView;
    m_listViewAddress->setModel(m_modelAddress);
    // 不可编辑
    m_listViewAddress->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

IMServerMessageCtrl::~IMServerMessageCtrl()
{
    if (m_listViewID != nullptr)
    {
        delete m_listViewID;
        m_listViewID = nullptr;
    }
    if (m_listViewAddress != nullptr)
    {
        delete m_listViewAddress;
        m_listViewAddress = nullptr;
    }
}

/*************************************************
Function Name： runServer()
Description: 用于启动服务器
*************************************************/
void IMServerMessageCtrl::runServer()
{
   m_tcpServer = new IMTcpServer(this);
   connect(m_tcpServer, SIGNAL(haveNewID(QString)),
           this, SLOT(insertIDData(QString)));
   connect(m_tcpServer, SIGNAL(haveNewAddress(QString)),
           this, SLOT(insertAddressData(QString)));

   connect(m_tcpServer, SIGNAL(haveLeftID(QString)),
           this, SLOT(deleteIDData(QString)));
   connect(m_tcpServer, SIGNAL(haveLeftAddress(QString)),
           this, SLOT(deleteAddressData(QString)));

   if (!m_tcpServer->listen(QHostAddress::Any, 1111))
   {
       QMessageBox::critical(nullptr, tr("IM Server"),
                             tr("Unable to start the server: %1")
                             .arg(m_tcpServer->errorString()));
       exit(EXIT_FAILURE);
   }
}

/*************************************************
Function Name： getIDListView()
Description: 获取listiew
*************************************************/
QListView * IMServerMessageCtrl::getIDListView()
{
    return m_listViewID;
}

/*************************************************
Function Name： getAddressListView()
Description: 获取listiew
*************************************************/
QListView * IMServerMessageCtrl::getAddressListView()
{
    return m_listViewAddress;
}

/*************************************************
Function Name： insertIDData()
Description: 插入数据
*************************************************/
void IMServerMessageCtrl::insertIDData(const QString & id)
{
//    int row = m_modelID->rowCount();
//    m_modelID->insertRows(row, 1);
//    QModelIndex index = m_modelID->index(row);
//    m_modelID->setData(index, id);
//    m_listViewID->setCurrentIndex(index);
    m_listID.append(id);
    m_modelID->setStringList(m_listID);
    insertAddressData("localHost");
}

/*************************************************
Function Name： deleteIDData()
Description: 删除数据
*************************************************/
void IMServerMessageCtrl::deleteIDData(const QString &id)
{
//    if(m_modelID->rowCount() > 1)
//    {
//        m_modelID->removeRows(m_listViewID->currentIndex().row(), 1);
//    }
    int index = m_listID.indexOf(id);
    m_listID.removeAt(index);
    m_modelID->removeRow(index);
    qDebug() << "deleteIDData" << id;
    if(index != -1)
    {
        QString temp = "localHost";
        deleteAddressData(temp);
    }
}

/*************************************************
Function Name： insertIDData()
Description: 插入数据
*************************************************/
void IMServerMessageCtrl::insertAddressData(const QString &address)
{
//    int row = m_modelAddress->rowCount();
//    m_modelAddress->insertRows(row, 1);
//    QModelIndex index = m_modelAddress->index(row);
//    m_modelAddress->setData(index, address);
//    m_listViewAddress->setCurrentIndex(index);

    m_listAddress.append(address);
    m_modelAddress->setStringList(m_listAddress);

}

/*************************************************
Function Name： deleteIDData()
Description: 删除数据
*************************************************/
void IMServerMessageCtrl::deleteAddressData(const QString & address)
{
    int index = m_listAddress.indexOf(address);
    m_listAddress.removeAt(index);
    m_modelAddress->removeRow(index);
}
