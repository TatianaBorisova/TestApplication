#ifndef TESTTABVIEW_H
#define TESTTABVIEW_H

#include "testbaseview.h"

class QPushButton;
class QGridLayout;
class QListWidget;

class TestTabView : public TestBaseView
{
    Q_OBJECT
public:
    explicit TestTabView(QWidget *parent = 0);
    void setFixedSize(int w, int h);

signals:
    void chosenTestName(const QString &fileName);
    void addPath(const QString &filePath);

public slots:
    void chooseTest();
    void chooseFolder();

private:
    bool findDumlicateFile(QListWidget *itemBox, const QString &fileName);
    void fillChoiceBox(QString folderPath);
    void AddToChoiceBox(const QString &filepath);

protected:
    virtual void resize();

private:
    QGridLayout *m_box;
    QListWidget *m_testBox;
    QPushButton *m_chooseTestFile;
    QPushButton *m_downloadTest;
    QPushButton *m_chooseFolder;
};

#endif // TESTTABVIEW_H

