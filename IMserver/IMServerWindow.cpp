#include "IMServerWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include "IMServerMessageCtrl.h"
#include <QListView>

IMServerWindow::IMServerWindow(QWidget *parent)
    : QWidget(parent)
{
    m_serverCtrl = new IMServerMessageCtrl(this);
    m_serverCtrl->runServer();

    QHBoxLayout *viewLayout = new QHBoxLayout;
    viewLayout->addWidget(m_serverCtrl->getIDListView());
    viewLayout->addWidget(m_serverCtrl->getAddressListView());

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *labelTitle = new QLabel(tr("连接服务器用户帐号列表:"));
    layout->addWidget(labelTitle);
    layout->addLayout(viewLayout);

    setLayout(layout);
}

IMServerWindow::~IMServerWindow()
{
    if (m_serverCtrl != nullptr)
    {
        delete m_serverCtrl;
    }
}
