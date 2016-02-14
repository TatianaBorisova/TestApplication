#include "choosetestview.h"
#include "global.h"

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QDir>

const QLatin1String slash = QLatin1String("/");

ChooseTestView::ChooseTestView(QWidget *parent) :
    QWidget(parent),
    m_hbox(new QHBoxLayout(this)),
    m_vbox(new QVBoxLayout(this)),
    m_testBox(new QListWidget(this)),
    m_chooseFolder(new QPushButton("...", this)),
    m_startTesting(new QPushButton("Выбрать", this))
{
    connect(m_startTesting, &QPushButton::clicked, this, &ChooseTestView::chooseTest);
    connect(m_chooseFolder, &QPushButton::clicked, this, &ChooseTestView::chooseFolder);
    connect(this, &ChooseTestView::addPath, this, &ChooseTestView::fillChoiceBox);

    fillChoiceBox(QDir::currentPath() + "/test/");

    setFixedSize(middleWidth, middleHeight);

    m_hbox->setGeometry(QRect(0, 0, width(), height()));
    m_vbox->setGeometry(QRect(m_testBox->width() - 2*m_hbox->margin(),
                              0,
                              m_testBox->width(),
                              m_testBox->height()));

    QFont font("Times", 15);

    m_testBox->setFont(font);
    m_chooseFolder->setFont(font);
    m_startTesting->setFont(font);

    m_testBox->setFixedSize(500, 500);

    m_chooseFolder->setFixedSize(50, 30);
    m_startTesting->setFixedSize(150, 50);

    m_vbox->setMargin(5);
    m_vbox->addWidget(m_chooseFolder);
    m_vbox->addSpacing(m_testBox->height() - m_chooseFolder->height() - m_startTesting->height() - 50);
    m_vbox->addWidget(m_startTesting);

    m_hbox->setContentsMargins(30, 5, 95, 5);
    m_hbox->addWidget(m_testBox);
    m_hbox->addSpacing(70);
    m_hbox->addLayout(m_vbox);

    this->setLayout(m_hbox);
}

void ChooseTestView::chooseTest()
{
    if (m_testBox->count() > 0) {
        QListWidgetItem *item = m_testBox->currentItem();
        if (item)
            emit chosenTestName(m_absolutePath + item->text());
    }
    emit chosenTestName("");
}

void ChooseTestView::chooseFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Test Folder"));
    emit addPath(folderPath);
}

void ChooseTestView::fillChoiceBox(const QString &folderPath)
{
    if (!folderPath.isEmpty()) {

        m_absolutePath.clear();
        m_testBox->clear();

        QDir entryDir(folderPath);
        if (entryDir.exists()) {

            QStringList fileFilter;
            fileFilter << "*.json";

            QStringList filesList = entryDir.entryList(fileFilter);

            for (int i = 0; i < filesList.count(); i++) {

                if (!findDumlicateFile(m_testBox, filesList.at(i))) {

                    QListWidgetItem *newItem = new QListWidgetItem();
                    if (folderPath.at(folderPath.count() - 1) != '/') {
                        m_absolutePath = folderPath + slash;
                    } else {
                        m_absolutePath = folderPath;
                    }

                    newItem->setText(filesList.at(i));
                    m_testBox->insertItem(m_testBox->count(), newItem);
                }
            }
        }
    }
}


bool ChooseTestView::findDumlicateFile(QListWidget *itemBox, const QString &fileName)
{
    if (itemBox) {
        for (int i = 0; i < itemBox->count(); i++) {
            if (fileName == itemBox->item(i)->text())
                return true;
        }
    }
    return false;
}
