#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include "global.h"

#include <QWidget>

class QLabel;
class QVBoxLayout;
class ResultView : public QWidget
{
    Q_OBJECT
public:
    explicit ResultView(QWidget *parent = 0);

public slots:
    void finishTestResult(const StudentResult &result);

private:
    QVBoxLayout *m_vbox;
    QLabel      *m_fio;
    QLabel      *m_group;
    QLabel      *m_header;
    QLabel      *m_sore;
};

#endif // RESULTVIEW_H
