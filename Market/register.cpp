#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    registerCtrl = new IMRegisterCtrl(this);
    setWindowTitle("注册");
}

Register::~Register()
{
    delete ui;
}

void Register::on_surepushButton_clicked()
{
    if(ui->userNamelineEdit->text().isEmpty()||ui->passwordlineEdit->text().isEmpty()
     ||ui->addresslineEdit->text().isEmpty())//判断用户输入密码是否为空
    {
        QMessageBox::information(this,tr("请输入账户信息"),tr("输入不能为空"),QMessageBox::Ok);
        ui->passwordlineEdit->clear();
        ui->passwordlineEdit->setFocus();
    }
    else
    {
        userName = ui->userNamelineEdit->text();
        passWord = ui->passwordlineEdit->text();
        userInf.userName = userName;
        userInf.logInPassWord = passWord;
        userInf.userType = 0;
        userInf.image = 0;
        userInf.buyer.balance = 0;
        userInf.buyer.payPassWord = "0";
        userInf.address = ui->addresslineEdit->text();
        registerCtrl->registerAccount(userInf);
        this->hide();
    }
}

QString Register::returnUserName()
{
    return userName;
}

QString Register::returnPassWord()
{
    return passWord;
}

void Register::on_cancelpushButton_clicked()
{
    QString temp = tr("确定想要退出程序？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退出程序"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
        this->close();
}

