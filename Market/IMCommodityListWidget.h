#ifndef IMCOMMODITYLISTWIDGET_H
#define IMCOMMODITYLISTWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QScrollArea;
class QMenu;
class Sell;

class IMCommodityListWidget : public QWidget
{
    Q_OBJECT
public:
    IMCommodityListWidget(Sell *mainWidget, QWidget *parent = nullptr);
    void addItem(QWidget *item);

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_CommodityScrollArea;
    Sell *m_mainWidget;
};

#endif // IMCOMMODITYLISTWIDGET_H
