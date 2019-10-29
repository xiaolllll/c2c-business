#ifndef MYINFORM_H
#define MYINFORM_H

#include <QWidget>

namespace Ui {
class myInform;
}

class Sell;

class myInform : public QWidget
{
    Q_OBJECT

public:
    explicit myInform(QString informTemp, QWidget *parent = nullptr);
    ~myInform();

private:

    QString inform;
    Ui::myInform *ui;
    void setStatus();

};

#endif // MYINFORM_H
