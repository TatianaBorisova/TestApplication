#ifndef STARTSVIEW_H
#define STARTSVIEW_H

#include "testbaseview.h"

class QPushButton;
class QHBoxLayout;

class StartView : public TestBaseView
{
    Q_OBJECT

public:
    StartView(QWidget *parent = 0);
    ~StartView();

private slots:
    void openTestView();

protected:
    virtual void resize();

private:
    QHBoxLayout *m_hbox;
    QPushButton *m_test;
    QPushButton *m_settings;
};

#endif // STARTSVIEW_H
