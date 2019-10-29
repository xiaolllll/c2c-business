#include "UserRegister.h"
#include "ui_UserRegister.h"
#include <QMessageBox>
#include "IMclient.h"

UserRegister::UserRegister(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserRegister)
{
    ui->setupUi(this);
    registerCtrl = new IMRegisterCtrl(this);
    setWindowTitle("注册");
}

UserRegister::~UserRegister()
{
    delete ui;
}

/***********************************
discrption:点击确定
***********************************/
void UserRegister::on_surepushButton_clicked()
{
    if(ui->userNamelineEdit->text().isEmpty()||ui->passwordlineEdit->text().isEmpty()
     ||ui->addresslineEdit->text().isEmpty()||ui->lineEditBalance->text().isEmpty()
            ||ui->lineEditPayPsd->text().isEmpty())//判断用户输入密码是否为空
    {
        QMessageBox::information(this,tr("请输入账户信息"),tr("输入不能为空"),QMessageBox::Ok);
        ui->passwordlineEdit->clear();
        ui->passwordlineEdit->setFocus();
    }
    else
    {
        userInf.buyer.balance = ui->lineEditBalance->text().toDouble();
        userInf.buyer.payPassWord = ui->lineEditPayPsd->text();
        userName = ui->userNamelineEdit->text();
        passWord = ui->passwordlineEdit->text();
        userInf.userType = 1;//代表买家
        userInf.image = 0;
        userInf.userName = userName;
        userInf.logInPassWord = passWord;
        userInf.address = ui->addresslineEdit->text();
        registerCtrl->registerAccount(userInf);
        this->hide();
    }
}

/***********************************/
//discrption:点击取消
/***********************************/
void UserRegister::on_cancelpushButton_clicked()
{
    QString temp = tr("确定想要退出程序？");
    QMessageBox::StandardButton rb;
    rb = QMessageBox::question(this, tr("退出程序"), temp,
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
        this->close();
}
