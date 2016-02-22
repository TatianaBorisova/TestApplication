#ifndef CLIENTTABVIEW_H
#define CLIENTTABVIEW_H

#include "testbaseview.h"

class QPushButton;
class QGridLayout;
class QListWidget;

class ClientTabView : public TestBaseView
{
    Q_OBJECT
public:
    explicit ClientTabView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

protected:
    virtual void resize();

private:
    QGridLayout *m_box;
};

#endif // CLIENTTABVIEW_H
