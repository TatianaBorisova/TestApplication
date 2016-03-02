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

    void setFixedSize(int w, int h);

signals:
    void startConnection(const QString &ip, int port);

public slots:
    void setClientConnectionState(int error);
    void setIp(const QString &ip);
    void setPort(const QString &port);

protected:
    virtual void resize();

private slots:
    void saveConnectionData();
    void back();

private:
    QLabel      *m_host;
    QLabel      *m_port;
    QLineEdit   *m_hostBox;
    QLineEdit   *m_portBox;
    QPushButton *m_save;
    QPushButton *m_back;
    QLabel      *m_connectionState;
    QGridLayout *m_box;
};

#endif // CLIENTTABVIEW_H
