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
class CustomButton;

class TestView : public TestBaseView
{
    Q_OBJECT
public:
    explicit TestView(QWidget *parent = 0);

    void setTestData(const TestQuestions &question);
    void setFixedSize(int w, int h);

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
    void setImg(QByteArray img);
    QString addUpperSymbol(const QString &str);

private:
    QLabel              *m_header;
    QLabel              *m_questionEntry;
    QLabel              *m_questionImg;
    QVBoxLayout         *m_mainBox;
    QPushButton         *m_nextBtn;
    QStringList          m_answersList;
    QString              m_trueAnswer;
    QGridLayout         *m_radioBtnList;
    AssureView          *m_anssureView;
    AnswersVector        m_answer;
};

#endif // TESTVIEW_H
