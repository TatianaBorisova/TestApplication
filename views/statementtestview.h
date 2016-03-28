#ifndef STATEMENTTESTVIEW_H
#define STATEMENTTESTVIEW_H

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

class StatementTestView : public TestBaseView
{
    Q_OBJECT
public:
    explicit StatementTestView(QWidget *parent = 0);

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

private:
    QLabel              *m_header;
    QLabel              *m_questionEntry;
    QLabel              *m_questionImg;
    QVBoxLayout         *m_mainBox;
    QPushButton         *m_answerBtn;
    QStringList          m_answersList;
    QString              m_trueAnswer;
    QWidget             *m_answersView;
    QGridLayout         *m_radioBtnList;
    AssureView          *m_anssureView;
    AnswersVector        m_answer;
};

#endif // STATEMENTTESTVIEW_H
