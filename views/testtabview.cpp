#include "testtabview.h"
#include "settingsview.h"

#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

namespace {
const QLatin1String slash = QLatin1String("/");
const int btnWidth = 200;
const int minHeight = 20;
}

TestTabView::TestTabView(QWidget *parent) :
    TestBaseView(parent),
    m_box(new QGridLayout(this)),
    m_vbox(new QVBoxLayout(this)),
    m_testBox(new QListWidget(this)),
    m_chooseTestBD(new QPushButton("Открыть БД", this)),
    m_downloadTest(new QPushButton("Загрузить БД с сервера", this)),
    m_chooseFolder(new QPushButton("Загрузить БД из папки", this)),
    m_chooseTest(new QPushButton("Выбрать тест", this)),
    m_back(new QPushButton("Вернуться на главную", this))
{
    connect(m_chooseTestBD, &QPushButton::clicked, this, &TestTabView::chooseTestDb);
    connect(m_chooseFolder, &QPushButton::clicked, this, &TestTabView::chooseFolder);
    connect(m_chooseTest, &QPushButton::clicked, this, &TestTabView::chooseTest);
    connect(m_back, &QPushButton::clicked, this, &TestTabView::back);
    connect(this, &TestTabView::addPath, this, &TestTabView::addToChoiceBox);

    m_downloadTest->setEnabled(false); //TBD make enabled after logic adding
    QFont wdgFont("Times", 11);
    m_testBox->setFont(wdgFont);

    this->setStyleSheet("QPushButton { height: 45px; }");

    m_vbox->addWidget(m_downloadTest);
    m_vbox->addWidget(m_chooseFolder);
    m_vbox->addWidget(m_chooseTestBD);
    m_vbox->addWidget(m_chooseTest);
    m_vbox->addWidget(m_back);

    m_box->setSpacing(minHeight);
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

void TestTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    m_testBox->setFixedSize(width() - btnWidth - 3*minHeight, height()*0.5);

    fillChoiceBox(QDir::currentPath() + "/test/");
}

void TestTabView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

    m_testBox->setFixedSize(width() - btnWidth - 3*minHeight, height()*0.5);

    fillChoiceBox(QDir::currentPath() + "/test/");
}

void TestTabView::chooseTestDb()
{
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item) {
            emit chosenTestDB(item->text());
            m_chooseTest->setEnabled(true);
            m_chooseTestBD->setEnabled(false);
        } else {
            QMessageBox::warning(0, "Error", "Выберите файл базы данных из списка.");
        }
    }
}

void TestTabView::chooseFolder()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"));
    emit addPath(filePath);
    m_chooseTest->setEnabled(false);
    m_chooseTestBD->setEnabled(true);
}

void TestTabView::chooseTest()
{
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item)
            emit chosenTestName(item->text());
        else
            QMessageBox::warning(0, "Error", "Выберите тест из списка.");

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
            if (!findDumlicateFile(m_testBox, allFiles.at(i))) {

                QListWidgetItem *newItem = new QListWidgetItem();

                newItem->setText(file + allFiles.at(i));
                m_testBox->insertItem(m_testBox->count(), newItem);
            }
        }
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

void TestTabView::fillChoiceBox(QString folderPath)
{
    if (!folderPath.isEmpty()) {

        if (folderPath.at(folderPath.count() - 1) != '/')
            folderPath = folderPath + slash;

        m_testBox->clear();

        QDir entryDir(folderPath);
        if (entryDir.exists()) {

            QStringList filesList = entryDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

            for (int i = 0; i < filesList.count(); i++) {

                if (!findDumlicateFile(m_testBox, filesList.at(i))) {

                    QListWidgetItem *newItem = new QListWidgetItem();

                    newItem->setText(folderPath + filesList.at(i));
                    m_testBox->insertItem(m_testBox->count(), newItem);
                }
            }
        }
    }
}
