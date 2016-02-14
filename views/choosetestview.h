#ifndef CHOOSETESTVIEW_H
#define CHOOSETESTVIEW_H

#include <QWidget>

class QListWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class ChooseTestView : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseTestView(QWidget *parent = 0);

signals:
    void chosenTestName(const QString &fileName);
    void addPath(const QString &filePath);

public slots:
    void chooseTest();
    void chooseFolder();

private:
    bool findDumlicateFile(QListWidget *itemBox, const QString &fileName);
    void fillChoiceBox(const QString &folderPath);

private:
    QHBoxLayout *m_hbox;
    QVBoxLayout *m_vbox;
    QListWidget *m_testBox;
    QPushButton *m_chooseFolder;
    QPushButton *m_startTesting;
    QString      m_absolutePath;
};

#endif // CHOOSETESTVIEW_H
