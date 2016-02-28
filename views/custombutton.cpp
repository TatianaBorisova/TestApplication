#include "custombutton.h"

#include <QRadioButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

QMap<int, CustomButton *> CustomButton::m_radioBtns;

CustomButton::CustomButton(QWidget *parent)
    : QWidget(parent),
      m_btn(new QRadioButton(this)),
      m_box(new QHBoxLayout(this)),
      m_text(new QLabel(this)),
      m_id(-1)
{
    connect(m_btn, &QRadioButton::clicked, this, &CustomButton::selectedBtn);
    connect(this, &CustomButton::selectId, this, &CustomButton::unsetNotSelected);

    m_text->setWordWrap(true);

    m_box->addWidget(m_btn);
    m_box->addWidget(m_text);

    m_btn->setFixedSize(20, 20);
    m_box->setMargin(5);
    m_box->setSpacing(0);

    setLayout(m_box);
}

void CustomButton::setButtonId(int id)
{
    m_id = id;
}

void CustomButton::selectedBtn()
{
    emit selectId(m_id);
}

void CustomButton::unsetNotSelected(int id)
{
    foreach(CustomButton *button, m_radioBtns) {
        if (button && button->radioBtn()) {
            if(button->id() == id) {
                button->radioBtn()->setChecked(true);
            } else {
                button->radioBtn()->setChecked(false);
            }
        }
    }
}

void CustomButton::deleteAllBtn()
{
    qDeleteAll(m_radioBtns);
    m_radioBtns.clear();
}

void CustomButton::addNewBtn(int id, CustomButton *btn)
{
    m_radioBtns.insert(id, btn);
}

void CustomButton::setText(const QString &text)
{
    m_text->setText(text);
}

QRadioButton *CustomButton::radioBtn()
{
    return m_btn;
}

QString CustomButton::text() const
{
    return m_text->text();
}
