#include "stringencryption.h"

StringEncryption::StringEncryption(QObject *parent) :
    QObject(parent)
{

}

QString StringEncryption::stringEncrypt(const QString &strInput)
{
    if (strInput.isEmpty())
        return "";

    QString array;
    // Кодирование информации
    for (int i = 0; i < strInput.length(); i++)
        array.append(QChar(strInput.at(i).unicode() + 1));

    //Возврат кодированной строки
    return array;
}

// Декодирование строки
QString StringEncryption::stringDecrypt(const QString &strInput)
{
    if (strInput.isEmpty())
        return "";

    QString array;
    // Кодирование информации
    for (int i = 0; i < strInput.length(); i++)
        array.append(QChar(strInput.at(i).unicode() - 1));

    //Возврат кодированной строки
    return array;
}
