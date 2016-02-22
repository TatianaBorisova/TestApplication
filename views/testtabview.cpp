#include "testtabview.h"
#include "settingsview.h"

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QDir>

#include <QDebug>

namespace {
const QLatin1String slash = QLatin1String("/");
const int btnWidth = 200;
const int margin = 20;
}

TestTabView::TestTabView(QWidget *parent) :
    TestBaseView(parent),
    m_box(new QGridLayout(this)),
    m_testBox(new QListWidget(this)),
    m_chooseTestFile(new QPushButton("Выбрать тест", this)),
    m_downloadTest(new QPushButton("Загрузить с сервера", this)),
    m_chooseFolder(new QPushButton("Загрузить из папки", this))
{
    connect(m_chooseTestFile, &QPushButton::clicked, this, &TestTabView::chooseTest);
    connect(m_chooseFolder, &QPushButton::clicked, this, &TestTabView::chooseFolder);
    connect(this, &TestTabView::addPath, this, &TestTabView::AddToChoiceBox);

    QFont wdgFont("Times", 11);
    m_testBox->setFont(wdgFont);

    QFont font("Times", 15);
    m_chooseTestFile->setFont(font);
    m_downloadTest->setFont(font);
    m_chooseFolder->setFont(font);

    m_chooseTestFile->setFixedSize(btnWidth, 50);
    m_downloadTest->setFixedSize(btnWidth, 50);
    m_chooseFolder->setFixedSize(btnWidth, 50);

    m_box->setSpacing(margin);
    m_box->addWidget(m_testBox, 0, 0);
    m_box->addWidget(m_chooseTestFile, 0, 1);
    m_box->addWidget(m_chooseFolder, 1, 1);
    m_box->addWidget(m_downloadTest, 2, 1);

    m_box->setAlignment(m_downloadTest, Qt::AlignBottom);
    m_box->setAlignment(m_chooseTestFile, Qt::AlignBottom);
    m_box->setAlignment(m_chooseFolder, Qt::AlignBottom);

    m_box->setMargin(margin);

    setLayout(m_box);
}

void TestTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    m_testBox->setFixedSize(width() - btnWidth - 3*margin, height()*0.5);

    fillChoiceBox(QDir::currentPath() + "/test/");
}

void TestTabView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

    m_testBox->setFixedSize(width() - btnWidth - 3*margin, height()*0.5);

    fillChoiceBox(QDir::currentPath() + "/test/");
}

void TestTabView::chooseTest()
{
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item)
            emit chosenTestName(item->text());
    }
    emit chosenTestName("");
}

void TestTabView::chooseFolder()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose Test Document"), "");
    emit addPath(filePath);
}

void TestTabView::fillChoiceBox(QString folderPath)
{
    if (!folderPath.isEmpty()) {

        if (folderPath.at(folderPath.count() - 1) != '/')
            folderPath = folderPath + slash;

        m_testBox->clear();

        QDir entryDir(folderPath);
        if (entryDir.exists()) {

            QStringList fileFilter;
            QStringList filesList = entryDir.entryList(fileFilter);

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

void TestTabView::AddToChoiceBox(const QString &filepath)
{
    if (!filepath.isEmpty()) {
        if (!findDumlicateFile(m_testBox, filepath)) {
            QListWidgetItem *newItem = new QListWidgetItem();

            newItem->setText(filepath);
            m_testBox->insertItem(m_testBox->count(), newItem);
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
