#ifndef SAVINGSETTINGSTABVIEW_H
#define SAVINGSETTINGSTABVIEW_H

#include "testbaseview.h"

class QLabel;
class QLineEdit;
class QGridLayout;
class QPushButton;

class SavingSettingsTabView : public TestBaseView
{
    Q_OBJECT
public:
    explicit SavingSettingsTabView(QWidget *parent = 0);

protected:
    virtual void resize();

private:
    QLabel      *m_savingPath;
    QLineEdit   *m_resDbName;
    QPushButton *m_choosePath;
    QGridLayout *m_grid;
};

#endif // SAVINGSETTINGSTABVIEW_H
