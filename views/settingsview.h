#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include "testbaseview.h"

class QTabWidget;
class ClientTabView;
class SavingSettingsTabView;

class SettingsView : public TestBaseView
{
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = 0);

    void setFixedSize(int w, int h);

signals:
    void startConnection(const QString &ip, int port);
    void refuseConnection();
    void finishTestResult(const StudentResult &result);

public slots:
    void setClientConnectionState(int error);
    void setIp(const QString &ip);
    void setPort(int port);

protected:
    virtual void resize();

private:
    QTabWidget            *m_tab;
    ClientTabView         *m_netwotkView;
    SavingSettingsTabView *m_saveView;
};

#endif // SETTINGSVIEW_H
