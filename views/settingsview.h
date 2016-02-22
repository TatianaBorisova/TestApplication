#ifndef SETTINGSTVIEW_H
#define SETTINGSTVIEW_H

#include "testbaseview.h"

class QGridLayout;
class QTabWidget;
class TestTabView;
class ClientTabView;

class SettingsView : public TestBaseView
{
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = 0);

signals:
    void chosenTestName(const QString &fileName);

protected:
    virtual void resize();

private:
    QTabWidget    *m_tab;
    QGridLayout   *m_box;
    TestTabView   *m_testPage;
    ClientTabView *m_clientSettingsPage;
};

#endif // SETTINGSTVIEW_H
