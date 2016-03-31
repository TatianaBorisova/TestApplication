#include "savingsettingstabview.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QSqlTableModel>
#include <QGridLayout>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QMutex>
#include <QMutexLocker>
#include <QFileDialog>
#include <QAxObject>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

namespace {
const int margin = 50;
const QString slash = "/";
}

SavingSettingsTabView::SavingSettingsTabView(QWidget *parent) :
    TestBaseView(parent),
    m_savingPath(new QLabel(QDir::currentPath() + slash, this)),
    m_resDbName(new QLineEdit("ResultDB", this)),
    m_choosePath(new QPushButton("Выбрать путь хранения", this)),
    m_grid(new QGridLayout(this)),
    m_model(new QSqlQueryModel(this)),
    m_table(new QTableView(this)),
    m_header(new QLabel("Таблица результатов тестирования:", this)),
    m_update(new QPushButton("Обновить таблицу", this)),
    m_docSaver(new QPushButton("Выгрузить в Doc файл", this))
{
    this->setStyleSheet("QPushButton { height: 45px; }"
                        "QTextEdit { height: 45px; }"
                        "QTimeEdit { height: 45px; }"
                        "QLineEdit { height: 45px; }"
                        "QPlainTextEdit { height: 45px; }");

    connect(m_update, &QPushButton::clicked, this, &SavingSettingsTabView::loadDbModel);
    connect(m_docSaver, &QPushButton::clicked, this, &SavingSettingsTabView::saveToDocFile);

    connect(m_choosePath, &QPushButton::clicked, this, &SavingSettingsTabView::chooseResultDBPathSlot);
    connect(m_resDbName,  &QLineEdit::editingFinished, this, &SavingSettingsTabView::changeResultDbNameSlot);

    m_table->setModel(m_model);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(m_savingPath);
    hbox->addWidget(m_resDbName);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(m_update);
    vbox->addWidget(m_docSaver);

    m_grid->addLayout(hbox, 0, 0);
    m_grid->addWidget(m_choosePath, 0, 1);
    m_grid->addLayout(vbox, 2, 1);
    m_grid->addWidget(m_header, 2, 0);
    m_grid->addWidget(m_table,  3, 0);

    m_grid->setMargin(margin);
    m_grid->setSpacing(margin);

    setDbName(m_savingPath->text() + m_resDbName->text());

    setLayout(m_grid);
}

void SavingSettingsTabView::chooseResultDBPathSlot()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Choose Test Directory"), "");
    m_savingPath->setText(filePath + slash);
    setDbName(m_savingPath->text() + m_resDbName->text());
}

void SavingSettingsTabView::changeResultDbNameSlot()
{
    setDbName(m_savingPath->text() + m_resDbName->text());
}

void SavingSettingsTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void SavingSettingsTabView::resize()
{
    m_table->setFixedSize(width()*0.8, height()*0.5);

    //make table full screen
    for (int i = 0; i < m_model->columnCount(); i++) {
        m_table->setColumnWidth(i, m_table->width()/m_model->columnCount());
    }
}

void SavingSettingsTabView::setDbName(const QString &name)
{
    qDebug() << "changed to " << name;
    m_dbname = name;
    loadDbModel();
}

void SavingSettingsTabView::loadDbModel()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    QSqlDatabase db = openDb();
    if (db.isOpen()) {
        m_model->setQuery("SELECT * FROM studentresults", db);
    }
    db.close();

    //make table full screen
    for (int i = 0; i < m_model->columnCount(); i++) {
        m_table->setColumnWidth(i, m_table->width()/m_model->columnCount());
    }
}

QSqlDatabase SavingSettingsTabView::openDb()
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");

    if (m_dbname.isEmpty()) {
        QMessageBox::critical(0, "Can not open database", "Введите имя базы данных на вкладке настройки.");
        return dbPtr;
    }

    dbPtr.setDatabaseName(m_dbname);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть " + m_dbname + " базу данных.\n");
    }

    return dbPtr;
}

void SavingSettingsTabView::fillResultStructure()
{
    m_dbTable.clear();

    QSqlDatabase db = openDb();
    if (db.isOpen()) {

        QSqlQuery q_select(db);
        q_select.prepare("SELECT * FROM studentresults");

        if (q_select.exec()) {
            while (q_select.next()) {
                //testname, firstname, secondName, surname, groupname, scorevalue, maxvalue, testtime
                StudentResult table;
                table.id              = q_select.value(q_select.record().indexOf("id")).toInt();
                table.firstName       = q_select.value(q_select.record().indexOf("firstname")).toString();
                table.secondName      = q_select.value(q_select.record().indexOf("secondName")).toString();
                table.surname         = q_select.value(q_select.record().indexOf("surname")).toString();
                table.group           = q_select.value(q_select.record().indexOf("groupname")).toString();
                table.score           = q_select.value(q_select.record().indexOf("scorevalue")).toInt();
                table.maxPosibleScore = q_select.value(q_select.record().indexOf("maxvalue")).toInt();
                table.time            = getTimeString(q_select.value(q_select.record().indexOf("testtime")).toString());
                table.testName        = q_select.value(q_select.record().indexOf("testname")).toString();
                m_dbTable.append(table);
            }
        }
    }
    db.close();
}

QString SavingSettingsTabView::getTimeString(const QString time)
{
    QString result = time;
    result = result.replace("T", " ");
    return result;
}

void SavingSettingsTabView::saveToDocFile()
{
    fillResultStructure();

    if (m_dbTable.count() > 0) {
        QString file = createDocFile();

        m_wordApp = new QAxObject("Word.Application",this);
        m_wordDoc = m_wordApp->querySubObject("Documents()");
        m_wordDoc->dynamicCall("Open(QVariant, QVariant, QVariant, QVariant, QVariant, QVariant, QVariant)", file, false, false, false, "", "", false);  //querySubObject("Add()");
        m_wordApp->setProperty("Visible", true);

        //Get active document.
        QAxObject* pActiveDocument = m_wordApp->querySubObject("ActiveDocument()");

        //Get selection.
        QAxObject *pSelection = m_wordApp->querySubObject("ActiveDocument")->querySubObject("ActiveWindow")->querySubObject("Selection");

        // создание таблицы
        QAxObject* pTables = pActiveDocument->querySubObject("Tables()");
        QAxObject* pNewTable = pTables->querySubObject("Add(Id, testname, firstname, secondName, surname, groupname, scorevalue, maxvalue, testtime)", pSelection->property("Range"), 1, 8, 1, 1);

        //Align table to center.
        pNewTable->querySubObject("Rows()")->setProperty("Alignment", "wdAlignRowCenter");

        //Iterate found records.
        QAxObject *pCell = NULL, *pCellRange = NULL;
        for(int cur_row = 0; cur_row < m_dbTable.count() - 1; cur_row++) //do you know why count - 1?? :) Coz Add(data) created table with 1 row and 8 columns
        {
            //Inserting new row for each new data.
            pSelection->dynamicCall("InsertRowsBelow()");
        }

        for (int cur_row = 0; cur_row < m_dbTable.count(); cur_row++) {

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 1);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).testName);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 2);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).firstName);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 3);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).secondName);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 4);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).surname);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 5);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).group);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 6);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).score);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 7);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).maxPosibleScore);

            pCell = pNewTable->querySubObject("Cell(Row, Column)", cur_row, 8);
            pCellRange = pCell->querySubObject("Range()");
            pCellRange->dynamicCall("InsertAfter(Text)", m_dbTable.at(cur_row).time);
        }
    } else {
        QMessageBox::warning(0, "Warning", "В выбранной Вами базе нет данных.");
    }
    // Iterate found records.End.
}

QString SavingSettingsTabView::createDocFile()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save File",
                                                    "TEST.doc",
                                                    "Documents (*.doc *.docx)");

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.close();

    return file.fileName();
}

void SavingSettingsTabView::saveTestResultInDB(const StudentResult &result)
{
    //create db if it doesn't exist
    createResultTable(m_dbname);

    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(m_dbname);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.");
        return;
    }

    QSqlQuery q_select(dbPtr);
    q_select.prepare("SELECT id FROM studentresults WHERE testname=:testname AND firstname=:firstname AND secondName=:secondName AND surname=:surname AND groupname=:groupname");
    q_select.bindValue(":testname", result.testName);
    q_select.bindValue(":firstname", result.firstName);
    q_select.bindValue(":secondName", result.secondName);
    q_select.bindValue(":surname", result.surname);
    q_select.bindValue(":groupname", result.group);

    if (q_select.exec()) {
        if (q_select.next()) {
            dbPtr.close();
            return;
        }
        // populate with some data
        QSqlQuery q_insert(dbPtr);
        q_insert.prepare("INSERT INTO studentresults (testname, firstname, secondName, surname, groupname, scorevalue, maxvalue, testtime) VALUES ( :testname, :firstname, :secondName, :surname, :groupname, :scorevalue, :maxvalue, :testtime)");

        q_insert.bindValue(":testname",   result.testName);
        q_insert.bindValue(":firstname",  result.firstName);
        q_insert.bindValue(":secondName", result.secondName);
        q_insert.bindValue(":surname",    result.surname);
        q_insert.bindValue(":groupname",  result.group);
        q_insert.bindValue(":scorevalue", result.score);
        q_insert.bindValue(":maxvalue",   result.maxPosibleScore);
        q_insert.bindValue(":testtime",   QDateTime::currentDateTime());

        qDebug() << "insert data row: " << q_insert.exec() << q_insert.lastError();
    }

    QSqlQuery q_existed(dbPtr);
    q_existed.prepare("SELECT id FROM studentresults WHERE testname=:testname AND firstname=:firstname AND secondName=:secondName AND surname=:surname AND groupname=:groupname");
    q_existed.bindValue(":testname",   result.testName);
    q_existed.bindValue(":firstname",  result.firstName);
    q_existed.bindValue(":secondName", result.secondName);
    q_existed.bindValue(":surname",    result.surname);
    q_existed.bindValue(":groupname",  result.group);

    if (q_existed.exec()) {
        while (q_existed.next()) {
            int resID = q_existed.value(q_existed.record().indexOf("id")).toInt();

            for (int i = 0; i < result.answerInfo.count(); i++) {
                QSqlQuery q_fullinfo(dbPtr);
                q_fullinfo.prepare("INSERT INTO studentresultanswers (resultid, statement, chosenvar, assuarance, iscorrect) "
                                   "VALUES (:resultid, :statement, :chosenvar, :assuarance, :iscorrect)");
                q_fullinfo.bindValue(":resultid",   resID);
                q_fullinfo.bindValue(":statement",  result.answerInfo.at(i).statement);
                q_fullinfo.bindValue(":chosenvar",  result.answerInfo.at(i).chosenAnswer);
                q_fullinfo.bindValue(":assuarance", result.answerInfo.at(i).assurance);
                q_fullinfo.bindValue(":iscorrect",  result.answerInfo.at(i).isCorrectAnswer);

                q_fullinfo.exec();
            }
        }
    }

    dbPtr.close();
}

void SavingSettingsTabView::createResultTable(const QString &dbName)
{
    if (dbName.isEmpty()) {
        QMessageBox::critical(0, "Can not open database", "Введите имя базы данных на вкладке настройки.");
        return;
    }

    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(dbName);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть " + dbName + " базу данных.\n");
        return;
    }

    QSqlQuery q_testcreate = dbPtr.exec("CREATE TABLE studentresults (id integer primary key autoincrement, testname varchar(255), firstname varchar(255), secondName varchar(255), surname varchar(255), groupname varchar(255), scorevalue int, maxvalue int, testtime datetime)");
    qDebug() << "create: " << q_testcreate.lastError();

    //create full answers
    QSqlQuery q_testanswers = dbPtr.exec("CREATE TABLE studentresultanswers (id integer primary key autoincrement, resultid int, statement varchar(255), chosenvar varchar(255), assuarance int, iscorrect int)");
    qDebug() << "create: " << q_testanswers.lastError();

    dbPtr.close();
}
