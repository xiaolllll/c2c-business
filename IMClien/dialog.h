#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "register.h"
#include "IMConstant.h"
#include "IMLoginCtrl.h"


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
private slots:
    void dealLoginMessage(const UserInformation *, bool isLogIn);
//    //点击取消
//    void on_exitPushButton_clicked();
//    //点击登陆
//    void on_logInPushButton_clicked();
    //点击注册
    void on_registerPushButton_clicked();


    void on_logInPushButton_2_clicked();

    void on_exitPushButton_2_clicked();

private:
    Ui::Dialog *ui;

    IMLoginCtrl *m_loginCtrl; // 用于控制登录
};

#endif // DIALOG_H
