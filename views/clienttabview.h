#ifndef CLIENTTABVIEW_H
#define CLIENTTABVIEW_H

#include "testbaseview.h"

class QPushButton;
class QGridLayout;
class QLabel;
class QLineEdit;

class ClientTabView : public TestBaseView
{
    Q_OBJECT
public:
    explicit ClientTabView(QWidget *parent = 0);

signals:
    void startConnection(const QString &ip, int port);
    void refuseConnection();

public slots:
    void setClientConnectionState(int error);
    void setIp(const QString &ip);
    void setPort(int port);

protected:
    virtual void resize();

private slots:
    void sendChosenConnectionData();
    void setDefaultConnectionData();
    void back();

private:
    QLabel      *m_host;
    QLabel      *m_port;
    QLineEdit   *m_hostBox;
    QLineEdit   *m_portBox;
    QPushButton *m_connectToChosen;
    QPushButton *m_connectToAny;
    QPushButton *m_back;
    QLabel      *m_connectionState;
    QGridLayout *m_box;
    int          m_error;
};

#endif // CLIENTTABVIEW_H
