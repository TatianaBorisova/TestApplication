#ifndef SAVINGSETTINGSTABVIEW_H
#define SAVINGSETTINGSTABVIEW_H

#include "testbaseview.h"

class QLabel;
class QLineEdit;
class QGridLayout;
class QPushButton;
class QSqlQueryModel;
class QTableView;
class QAxObject;
class QSqlDatabase;

class SavingSettingsTabView : public TestBaseView
{
    Q_OBJECT
public:
    explicit SavingSettingsTabView(QWidget *parent = 0);

public slots:
    void setDbName(const QString &name);
    void loadDbModel();
    void saveTestResultInDB(const StudentResult &result);
    void setFixedSize(int w, int h);

protected:
    virtual void resize();

private slots:
    void saveToDocFile();
    void fillResultStructure();
    void back();

    QString createDocFile();

    QString getTimeString(const QString time);

    void chooseResultDBPathSlot();
    void changeResultDbNameSlot();

private:
    QSqlDatabase openDb();
    void createResultTable(const QString &dbName);

    QLabel      *m_savingPath;
    QLineEdit   *m_resDbName;
    QPushButton *m_choosePath;
    QGridLayout *m_grid;

    QSqlQueryModel *m_model;
    QTableView     *m_table;
    QString         m_dbname;
    QLabel         *m_header;
    QPushButton    *m_update;
    QPushButton    *m_docSaver;
    QPushButton    *m_back;
    QAxObject      *m_wordApp;
    QAxObject      *m_wordDoc;
    QList<StudentResult> m_dbTable;

};

#endif // SAVINGSETTINGSTABVIEW_H
