#ifndef TESTTABVIEW_H
#define TESTTABVIEW_H

#include "testbaseview.h"

class QPushButton;
class QGridLayout;
class QListWidget;
class QVBoxLayout;

class TestTabView : public TestBaseView
{
    Q_OBJECT
public:
    explicit TestTabView(QWidget *parent = 0);
    void setFixedSize(int w, int h);

signals:
    void chosenTestDB(const QString &fileName);
    void chosenTestName(const QString &fileName);
    void addPath(const QString &filePath);

public slots:
    void chooseTestDb();
    void chooseFolder();
    void chooseTest();
    void fillTestVariants(const QList<TestHeaderData> &test);
    void dbError();
    void setClientConnectionState(int state);

private slots:
    void back();

private:
    bool findDumlicateFile(QListWidget *itemBox, const QString &fileName);
    void fillChoiceBox(QString folderPath);
    void addToChoiceBox(const QString &filepath);
    void checkConnectionState();

protected:
    virtual void resize();

private:
    QGridLayout *m_box;
    QVBoxLayout *m_vbox;
    QListWidget *m_testBox;
    QPushButton *m_chooseTestBD;
    QPushButton *m_downloadTest;
    QPushButton *m_chooseFolder;
    QPushButton *m_chooseTest;
    QPushButton *m_back;
    int          m_connectionState;
};

#endif // TESTTABVIEW_H

