#ifndef SETTINGSTABVIEW_H
#define SETTINGSTABVIEW_H

#include "testbaseview.h"

class QTabWidget;
class ClientTabView;

class SettingsTabView : public TestBaseView
{
    Q_OBJECT
public:
    explicit SettingsTabView(QWidget *parent = 0);

signals:
    void startConnection(const QString &ip, int port);
    void refuseConnection();

public slots:
    void setClientConnectionState(int error);
    void setIp(const QString &ip);
    void setPort(int port);

protected:
    virtual void resize();

private:
    QTabWidget    *m_tab;
    ClientTabView *m_netwotkView;
};

#endif // SETTINGSTABVIEW_H
