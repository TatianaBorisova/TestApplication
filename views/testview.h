#ifndef TESTVIEW_H
#define TESTVIEW_H

#include "global.h"
#include "testbaseview.h"

class QLabel;
class QPushButton;
class QGridLayout;
class QRadioButton;
class QVBoxLayout;
class QStringList;
class AssureView;

class TestView : public TestBaseView
{
    Q_OBJECT
public:
    explicit TestView(QWidget *parent = 0);

    void setTestData(const TestStructure &question);

signals:
    void answeredResult(const AnswersVector &answer);

private slots:
    void setAnsweredState();
    void yesVariant();
    void noVariant();

protected:
    virtual void resize();

private:
    void createRadioBtnList(const QStringList &list);
    void clearRadioBtnList();
    void hideEntry();
    void showEntry();
    void setImg(const QString &imgPath);

private:
    QLabel              *m_header;
    QLabel              *m_questionEntry;
    QLabel              *m_questionImg;
    QVBoxLayout         *m_mainBox;
    QPushButton         *m_nextBtn;
    QStringList          m_answersList;
    QString              m_trueAnswer;
    QList<QRadioButton*> m_createdBtn;
    QGridLayout         *m_radioBtnList;
    AssureView          *m_anssureView;
    AnswersVector        m_answer;
};

#endif // TESTVIEW_H
