#ifndef ONLYDISCOUNT_H
#define ONLYDISCOUNT_H

#include <QWidget>

class Sell;

namespace Ui {
class OnlyDiscount;
}

class OnlyDiscount : public QWidget
{
    Q_OBJECT

public:
    explicit OnlyDiscount(Sell *sellTemp, QWidget *parent = nullptr);
    ~OnlyDiscount();

private slots:
    void on_pushButton_clicked();

private:
    Sell *m_sell;
    int discount;
    Ui::OnlyDiscount *ui;
};

#endif // ONLYDISCOUNT_H
