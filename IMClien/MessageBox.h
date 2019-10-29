#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>

namespace Ui {
class m_MessageBox;
}

class m_MessageBox : public QWidget
{
    Q_OBJECT

public:
    explicit m_MessageBox(QWidget *parent = nullptr);
    ~m_MessageBox();

private slots:
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::m_MessageBox *ui;
};

#endif // MESSAGEBOX_H
