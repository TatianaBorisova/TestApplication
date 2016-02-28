#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <QMap>

class QRadioButton;
class QHBoxLayout;
class QLabel;

class CustomButton : public QWidget
{
    Q_OBJECT
public:
    explicit CustomButton(QWidget *parent = 0);

    QString text() const;
    void setText(const QString &text);
    QRadioButton *radioBtn();
    void setButtonId(int id);
    int id() { return m_id; }

    static void addNewBtn(int id, CustomButton *btn);
    static void deleteAllBtn();
    static QMap<int, CustomButton *> customBtns() { return m_radioBtns; }

signals:
    void selectId(int id);

private slots:
    void selectedBtn();
    static void unsetNotSelected(int id);

private:
    QRadioButton *m_btn;
    QHBoxLayout  *m_box;
    QLabel       *m_text;
    int           m_id;

    static QMap<int, CustomButton *> m_radioBtns;
};

#endif // CUSTOMBUTTON_H
