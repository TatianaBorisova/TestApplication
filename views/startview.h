#ifndef STARTSVIEW_H
#define STARTSVIEW_H

#include <QWidget>
class QPushButton;
class QVBoxLayout;

class StartView : public QWidget
{
    Q_OBJECT

public:
    StartView(QWidget *parent = 0);
    ~StartView();

signals:
    void openTestChoice();

private:
    QVBoxLayout *m_vbox;
    QPushButton *m_openTest;
    QPushButton *m_exit;
};

#endif // STARTSVIEW_H
