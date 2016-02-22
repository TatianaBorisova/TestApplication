#ifndef ASSUREVIEW_H
#define ASSUREVIEW_H

#include "testbaseview.h"

class QPushButton;
class QHBoxLayout;
class QLabel;

class AssureView : public TestBaseView
{
    Q_OBJECT
public:
    explicit AssureView(QWidget *parent = 0);

signals:
    void yesButton();
    void noButton();

protected:
    virtual void resize();

private:
    QLabel      *m_assure;
    QHBoxLayout *m_hbox;
    QPushButton *m_yesBtn;
    QPushButton *m_noBtn;
};

#endif // ASSUREVIEW_H
