#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "IMConstant.h"
#include "IMRegisterCtrl.h"

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    QString userName;
    QString passWord;
    QString returnUserName();
    QString returnPassWord();
public slots:

    void on_surepushButton_clicked();

    void on_cancelpushButton_clicked();

private:
    UserInformation userInf;
    IMRegisterCtrl *registerCtrl;
    Ui::Register *ui;
};

#endif // REGISTER_H
