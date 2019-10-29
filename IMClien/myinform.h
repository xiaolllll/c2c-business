#ifndef MYINFORM_H
#define MYINFORM_H

#include <QWidget>

namespace Ui {
class myInform;
}

class IMclient;

class myInform : public QWidget
{
    Q_OBJECT

public:
    explicit myInform(IMclient *clientTemp, QString informTemp, QWidget *parent = nullptr);
    ~myInform();

private slots:

private:
    void setStatus();
    IMclient *client;
    QString inform;
    Ui::myInform *ui;
};

#endif // MYINFORM_H
