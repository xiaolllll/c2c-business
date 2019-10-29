#ifndef COMMODITYBUTTON_H
#define COMMODITYBUTTON_H

#include <QPushButton>
#include "IMclient.h"
#include "IMConstant.h"

class QMenu;
class QLabel;

class CommodityButton : public QPushButton
{
    Q_OBJECT
public:
    CommodityButton(const Commodity& commodity, IMclient *sell, const int type =0, QWidget *parent = nullptr);
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
    int type;//0代表购买， 1代表售后处理
    Commodity commodityInf;
    IMclient *sell;
    QLabel *m_labelHead;
    QLabel *m_labelMes;
    void onClickButton();
    void setButtonStatus();

};


#endif // COMMODITYBUTTON_H
