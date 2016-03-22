#ifndef MAINTESTVIEW_H
#define MAINTESTVIEW_H

#include "testbaseview.h"

class QGridLayout;
class QTabWidget;
class TestTabView;

class MainTestView : public TestBaseView
{
    Q_OBJECT
public:
    explicit MainTestView(QWidget *parent = 0);

signals:
    void chosenTestDB(const QString &fileName);
    void chosenTestName(const QString &fileName);
    void readTests(const QList<TestHeaderData> &test);
    void dbError();
    void clientConnectionState(int state);
    void tryGetTestsFromServer();
    void sendDownloadedFilePath(const QString &path);

protected:
    virtual void resize();

private:
    QTabWidget    *m_tab;
    QGridLayout   *m_box;
    TestTabView   *m_testPage;
};

#endif // MAINTESTVIEW_H
