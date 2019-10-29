#include "IMCommodityListWidget.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>

IMCommodityListWidget::IMCommodityListWidget(Sell *mainWidget, QWidget *parent):
     QWidget (parent), m_mainWidget(mainWidget),m_layout(new QVBoxLayout)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_CommodityScrollArea = new QScrollArea(this);
    m_CommodityScrollArea->setWidgetResizable(true);
    m_CommodityScrollArea->setAlignment(Qt::AlignLeft);

}

/*************************************************
Function Name： addItem()
Description:  添加
*************************************************/
void IMCommodityListWidget::addItem(QWidget *item)
{

}
