#ifndef UPDATEDISCOUNT_H
#define UPDATEDISCOUNT_H

#include <QWidget>

namespace Ui {
class UpdateDiscount;
}

class Sell;

class UpdateDiscount : public QWidget
{
    Q_OBJECT

public:
    explicit UpdateDiscount(Sell * sell, QWidget *parent = nullptr);
    ~UpdateDiscount();

signals:
private slots:
    void on_pushButton_clicked();

private:
    int discount1;
    int discount2;
    int discount3;
    Sell *m_sell;
    Ui::UpdateDiscount *ui;
};

#endif // UPDATEDISCOUNT_H
