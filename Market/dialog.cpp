#include "dialog.h"
#include "sell.h"
#include "ui_dialog.h"
/*
 *修改用户密码登陆，改为不能显示
*/
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    m_loginCtrl = new IMLoginCtrl(this);
    setWindowTitle("卖家登陆");
    ui->setupUi(this);
    connect(m_loginCtrl, SIGNAL(getLoginMessgae(bool,const UserInformation *)),
            this, SLOT(dealLoginMessage(bool,const UserInformation *)));
}

Dialog::~Dialog()
{
    if(m_loginCtrl != nullptr)
    {
        delete m_loginCtrl;
    }
    delete ui;
}

void Dialog::on_exitPushButton_clicked()//点击退出
{
    QString temp = tr("确定想要退出程序？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退出程序"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
        this->close();
}

void Dialog::dealLoginMessage(bool isLogin,const UserInformation * me)
{
    if(isLogin)
    {
        Sell *sell = new Sell(*me);
        sell->getCommodityVector();//获取卖家的商品列表
        this->hide();
        sell->show();
    }
    else//用户名和密码匹配
    {
        QMessageBox::information(this,tr("密码错误"),tr("请输入正确的密码再登陆"),QMessageBox::Ok);
        ui->passWordLineEdit->clear();
        ui->passWordLineEdit->setFocus();
    }
}

void Dialog::on_logInPushButton_clicked()//点击登陆
{
    if(ui->passWordLineEdit->text().isEmpty())//判断用户输入密码是否为空
    {
        QMessageBox::information(this,tr("请输入密码"),tr("请先输入用户名再输入密码"),QMessageBox::Ok);
    }
    else
    {
        userName = ui->userNameLineEdit->text();
        passWord = ui->passWordLineEdit->text();
        if(nullptr == m_loginCtrl)
        {
            m_loginCtrl = new IMLoginCtrl(this);
        }
        m_loginCtrl->login(userName, passWord);
    }
}

void Dialog::on_registerPushButton_clicked()//点击注册
{
    Register* accountRegister = new Register;
    this->hide();
    accountRegister->show();
}
