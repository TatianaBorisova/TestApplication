#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include "global.h"
#include "testbaseview.h"

class QLabel;
class QVBoxLayout;
class QPushButton;

class ResultView : public TestBaseView
{
    Q_OBJECT
public:
    explicit ResultView(QWidget *parent = 0);

public slots:
    void finishTestResult(const StudentResult &result);

protected:
    virtual void resize();

private:
    void back();

    QVBoxLayout *m_vbox;
    QLabel      *m_fio;
    QLabel      *m_group;
    QLabel      *m_header;
    QLabel      *m_sore;
    QPushButton *m_back;
};

#endif // RESULTVIEW_H
