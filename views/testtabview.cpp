#include "testtabview.h"
#include "maintestview.h"

#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>

namespace {
const QLatin1String slash = QLatin1String("/");
const int btnWidth = 250;
const int minHeight = 20;
}

TestTabView::TestTabView(QWidget *parent) :
    TestBaseView(parent),
    m_box(new QGridLayout(this)),
    m_vbox(new QVBoxLayout(this)),
    m_testBox(new QListWidget(this)),
    m_chooseTestBD(new QPushButton("Открыть тестовую базу", this)),
    m_downloadTest(new QPushButton("Загрузить тесты с сервера", this)),
    m_chooseFolder(new QPushButton("Выбрать папку с тест-базами", this)),
    m_chooseTest(new QPushButton("Выбрать тест из базы", this)),
    m_back(new QPushButton("Вернуться на главную", this)),
    m_connectionState(-1)
{
    connect(m_chooseTestBD, &QPushButton::clicked, this, &TestTabView::chooseTestDb);
    connect(m_chooseFolder, &QPushButton::clicked, this, &TestTabView::chooseFolder);
    connect(m_chooseTest, &QPushButton::clicked, this, &TestTabView::chooseTest);
    connect(m_downloadTest, &QPushButton::clicked, this, &TestTabView::getTestsFromServer);
    connect(m_back, &QPushButton::clicked, this, &TestTabView::back);
    connect(this, &TestTabView::addPath, this, &TestTabView::addToChoiceBox);

    QFont wdgFont("Times", 11);
    m_testBox->setFont(wdgFont);

    this->setStyleSheet("QPushButton { height: 45px; }");

    m_vbox->addWidget(m_downloadTest);
    m_vbox->addWidget(m_chooseFolder);
    m_vbox->addWidget(m_chooseTestBD);
    m_vbox->addWidget(m_chooseTest);
    m_vbox->addWidget(m_back);

    m_box->setSpacing(2*minHeight);
    m_box->addWidget(m_testBox, 0, 0);
    m_box->addLayout(m_vbox,    0, 1);

    m_box->setAlignment(m_testBox, Qt::AlignTop);
    m_box->setMargin(10);

    m_chooseTest->setEnabled(false);
    setLayout(m_box);
}

void TestTabView::back()
{
    emit showView(TestStartView);
}

void TestTabView::getTestsFromServer()
{
    if (m_connectionState != 0) {
        QMessageBox::warning(0, "Внимание", "Ошибка скачивания. Для того, чтобы скачать файлы подключитесь к серверу.");
        return;
    }

    emit tryGetTestsFromServer();
}

void TestTabView::resize()
{
    setFixedSize(width(), height());

    m_testBox->setFixedSize(width() - btnWidth - 3*minHeight, height()*0.5);

    fillChoiceBox(QDir::currentPath() + "/test/");
}

void TestTabView::checkConnectionState()
{
    if (m_connectionState != 0) {
        QMessageBox::warning(0, "Внимание", "Внимание, Вы не подключены к сети. Вы можете выбрать и пройти тест, но результаты тестирования не будут сохранены на сервере. Данные сохранятся локально.");
    }
}

void TestTabView::chooseTestDb()
{
    checkConnectionState();
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item) {
            emit chosenTestDB(item->text());
            qDebug() << "test db " << item->text();
            m_chooseTest->setEnabled(true);
            m_chooseTestBD->setEnabled(false);
        } else {
            QMessageBox::warning(0, "Ошибка", "Выберите файл базы данных из списка.");
        }
    } else {
        QMessageBox::warning(0, "Ошибка", "Выберите путь к папке с тестами, либо скачайте тесты с сервера.");
    }
}

void TestTabView::chooseFolder()
{
    checkConnectionState();
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"));
    qDebug() << "chooseFolder" << filePath;
    emit addPath(filePath);
    m_chooseTest->setEnabled(false);
    m_chooseTestBD->setEnabled(true);
}

void TestTabView::chooseTest()
{
    checkConnectionState();
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item) {
            emit chosenTestName(item->text());
            m_chooseTest->setEnabled(false);
            m_chooseTestBD->setEnabled(true);
        } else {
            QMessageBox::warning(0, "Ошибка", "Выберите тест из списка.");
        }
    }
}

void TestTabView::fillTestVariants(const QList<TestHeaderData> &test)
{
    if (test.count() > 0)
        m_testBox->clear();

    for (int i = 0; i < test.count(); i++) {
        QListWidgetItem *newItem = new QListWidgetItem();

        newItem->setText(test.at(i).testName);
        m_testBox->insertItem(m_testBox->count(), newItem);
    }
}

void TestTabView::dbError()
{
    m_chooseTest->setEnabled(false);
    m_chooseTestBD->setEnabled(true);
}

void TestTabView::setClientConnectionState(int state)
{
    m_connectionState = state;
}

void TestTabView::addToChoiceBox(const QString &filepath)
{
    QString file = filepath;
    if (!file.isEmpty()) {

        int index = file.lastIndexOf("/");
        if (index != file.count() - 1)
            file = file + "/";

        m_testBox->clear();
        QDir chosenDir(file);

        QStringList allFiles = chosenDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        for (int i = 0; i < allFiles.count(); i++) {

            if (!findDumlicateFile(m_testBox, allFiles.at(i))
                    && checkIfTestDb(file + allFiles.at(i))) {

                QListWidgetItem *newItem = new QListWidgetItem();

                newItem->setText(file + allFiles.at(i));
                m_testBox->insertItem(m_testBox->count(), newItem);
            }
        }
    } else {
        QMessageBox::warning(0, "Внимание", "Путь к тестовым данным не выбран.");
    }
}

bool TestTabView::findDumlicateFile(QListWidget *itemBox, const QString &fileName)
{
    if (itemBox) {
        for (int i = 0; i < itemBox->count(); i++) {
            if (fileName == itemBox->item(i)->text())
                return true;
        }
    }
    return false;
}

void TestTabView::fillChoiceBox(const QString &folderPath)
{
    QString tmpFolderStr = folderPath;
    if (!tmpFolderStr.isEmpty()) {

        if (tmpFolderStr.at(tmpFolderStr.count() - 1) != '/')
            tmpFolderStr = tmpFolderStr + slash;

        m_testBox->clear();

        QDir entryDir(tmpFolderStr);
        if (entryDir.exists()) {

            QStringList filesList = entryDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

            for (int i = 0; i < filesList.count(); i++) {

                if (!findDumlicateFile(m_testBox, filesList.at(i))
                        && checkIfTestDb(tmpFolderStr + filesList.at(i))) {

                    QListWidgetItem *newItem = new QListWidgetItem();

                    newItem->setText(tmpFolderStr + filesList.at(i));
                    m_testBox->insertItem(m_testBox->count(), newItem);
                }
            }
        }
    }
}

bool TestTabView::checkIfTestDb(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        file.seek(0);

        QByteArray bytes = file.read(16);
        if (QString(bytes.data()).contains("SQLite format")) {

            QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
            dbPtr.setDatabaseName(filename);
            if (!dbPtr.open()) {
                QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
                return false;
            }

            if (dbPtr.tables().contains(QLatin1String("testdata"))
                    && dbPtr.tables().contains(QLatin1String("questionsdata"))) {
                return true;
            }

            dbPtr.close();
        }
    }
    return false;
}
