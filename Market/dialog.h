#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include "IMLoginCtrl.h"
#include "register.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    //用户民
    QString userName;
    //登陆密码
    QString passWord;
    UserInformation userInf;
signals:

private slots:
    void dealLoginMessage(bool isLogin = false,const UserInformation * me = nullptr);
    //点击取消
    void on_exitPushButton_clicked();
    //点击登陆
    void on_logInPushButton_clicked();
    //点击注册
    void on_registerPushButton_clicked();


private:
    Ui::Dialog *ui;

    IMLoginCtrl *m_loginCtrl; // 用于控制登录
};

#endif // DIALOG_H
