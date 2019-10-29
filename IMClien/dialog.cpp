#include "dialog.h"
#include "IMclient.h"
#include "ui_dialog.h"
#include "UserRegister.h"
#include <QMessageBox>
/*
 *修改用户密码登陆，改为不能显示
*/
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_loginCtrl = new IMLoginCtrl(this);
    setWindowTitle("买家登陆");
    connect(m_loginCtrl, SIGNAL(getLoginMessgae(const UserInformation*, bool)),
            this, SLOT(dealLoginMessage(const UserInformation*, bool)));
}

Dialog::~Dialog()
{
    if(m_loginCtrl != nullptr)
    {
        delete m_loginCtrl;
    }
    delete ui;
}

//void Dialog::on_exitPushButton_clicked()//点击退出
//{
//    QString temp = tr("确定想要退出程序？");
//    QMessageBox::StandardButton rb;
//    rb = QMessageBox::question(this, tr("退出程序"), temp,
//        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
//    if(rb == QMessageBox::Yes)
//        this->close();
//}

void Dialog::dealLoginMessage(const UserInformation *me,bool isLogin)
{
    if(isLogin)
    {
        qDebug() << "dealwithLogINMessage";
        qDebug() << me->userName << endl << me->address;
        IMclient *imclient = new IMclient(*me);
        qDebug() << "进入主界面！";
        imclient->getCommodityVector();//获取卖家的商品列表
        imclient->show();
        this->close();
    }
    else//用户名和密码匹配
    {
        QMessageBox::information(this,tr("密码错误"),tr("请输入正确的密码再登陆"),QMessageBox::Ok);
        ui->passWordLineEdit_2->clear();
        ui->passWordLineEdit_2->setFocus();
    }
}

//void Dialog::on_logInPushButton_clicked()//点击登陆
//{
//    if(ui->passWordLineEdit_2->text().isEmpty())//判断用户输入密码是否为空
//    {
//        QMessageBox::information(this,tr("请输入密码"),tr("请先输入用户名再输入密码"),QMessageBox::Ok);
//    }
//    else
//    {
//        userName = ui->userNameLineEdit_2->text();
//        passWord = ui->passWordLineEdit_2->text();
//        if(nullptr == m_loginCtrl)
//        {
//            m_loginCtrl = new IMLoginCtrl(this);
//            connect(m_loginCtrl, SIGNAL(getLoginMessgae(const UserInformation*, bool)),
//                    this, SLOT(dealLoginMessage(const UserInformation*, bool)));

//        }
//        m_loginCtrl->login(userName, passWord);
//    }
//}

void Dialog::on_registerPushButton_clicked()//点击注册
{
    UserRegister* accountRegister = new UserRegister;
    this->hide();
    accountRegister->show();
}

void Dialog::on_logInPushButton_2_clicked()
{
    if(ui->passWordLineEdit_2->text().isEmpty())//判断用户输入密码是否为空
    {
        QMessageBox::information(this,tr("请输入密码"),tr("请先输入用户名再输入密码"),QMessageBox::Ok);
    }
    else
    {
        userName = ui->userNameLineEdit_2->text();
        passWord = ui->passWordLineEdit_2->text();
        if(nullptr == m_loginCtrl)
        {
            m_loginCtrl = new IMLoginCtrl(this);
        }
        m_loginCtrl->login(userName, passWord);
    }
}

void Dialog::on_exitPushButton_2_clicked()
{
    QString temp = tr("确定想要退出程序？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退出程序"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
        this->close();
}
