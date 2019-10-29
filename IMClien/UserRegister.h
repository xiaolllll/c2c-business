#ifndef USERREGISTER_H
#define USERREGISTER_H

#include <QMainWindow>
#include "IMRegisterCtrl.h"
#include "IMConstant.h"

namespace Ui {
class UserRegister;
}

class UserRegister : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserRegister(QWidget *parent = nullptr);
    ~UserRegister();
    QString userName;
    QString passWord;
    QString returnUserName();
    QString returnPassWord();
private slots:

    void on_surepushButton_clicked();

    void on_cancelpushButton_clicked();

private:
    IMRegisterCtrl *registerCtrl;
    UserInformation userInf;
    Ui::UserRegister *ui;
};

#endif // USERREGISTER_H
