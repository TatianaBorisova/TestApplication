#ifndef STUDENTINFOVIEW_H
#define STUDENTINFOVIEW_H

#include "global.h"

#include <QWidget>

class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class StudentInfoView : public QWidget
{
    Q_OBJECT
public:
    explicit StudentInfoView(QWidget *parent = 0);

public slots:
    void nextButtonPressed();

signals:
    void nextStep(const StudentResult &info);

private:
    QGridLayout *m_centerBox;
    QLabel      *m_nameLabel;
    QLabel      *m_secondNameLabel;
    QLabel      *m_surnameLabel;
    QLabel      *m_groupLabel;
    QLineEdit   *m_nameField;
    QLineEdit   *m_secondField;
    QLineEdit   *m_surnameField;
    QLineEdit   *m_groupField;
    QLabel      *m_info;
    QPushButton *m_nextButton;
};

#endif // STUDENTINFOVIEW_H
