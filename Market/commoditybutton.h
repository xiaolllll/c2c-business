#ifndef COMMODITYBUTTON_H
#define COMMODITYBUTTON_H

#include <QPushButton>
#include "sell.h"
#include "IMConstant.h"

class QMenu;
class QLabel;

class CommodityButton : public QPushButton
{
    Q_OBJECT
public:
    CommodityButton(const Commodity& commodity, Sell *sell, QWidget *parent = nullptr);
    void setChangedInformation(const Commodity & commodityInformation)
    {
        if (commodityInf.commodityID != commodityInformation.commodityID)
            return;
        commodityInf.m_headPortrait = commodityInformation.m_headPortrait;
        commodityInf.commodityName = commodityInformation.commodityName;
        commodityInf.originalCost = commodityInformation.originalCost;
    }

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    Commodity commodityInf;
    Sell *sell;
    QLabel *m_labelHead;
    QLabel *m_labelMes;
    void onClickButton();
    void setButtonStatus();

};


#endif // COMMODITYBUTTON_H
