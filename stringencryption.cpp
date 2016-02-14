#include "stringencryption.h"

StringEncryption::StringEncryption(QObject *parent) :
    QObject(parent)
{

}

QString StringEncryption::stringEncrypt(const QString &strInput,
                                        const QString &strPassword)
{
    int i,j;
    QString strOutput="";

    // Перевод строк в битовые массивы с использованием локальных таблиц
    QByteArray baInput    = strInput.toLocal8Bit();
    QByteArray baPassword = strPassword.toLocal8Bit();

    // Кодирование информации
    for (i=0; i < baInput.size(); i++)
    {
        for (j=0; j < baPassword.size(); j++)
        {
            // XOR - кодировка символа
            baInput[i] = baInput[i] ^ (baPassword[j] + (i*j));
        }

        //Преобразование числа в шестнадцатеричную систему
        strOutput += QString("%1").arg((int)((unsigned char)baInput[i]),2,16,QChar('0'));
    }

    //Возврат кодированной строки
    return strOutput;
}


// Декодирование строки
QString StringEncryption::stringDecrypt(const QString &strInput,
                                        const QString &strPassword)
{
    int i,j;

    // Декодировка строки из 16-ричной системы в битовый массив
    QByteArray baInput    = QByteArray::fromHex(strInput.toLocal8Bit());
    // Перевод строки пароля в битовый массив
    QByteArray baPassword = strPassword.toLocal8Bit();

    // Декодирование информации
    for (i=0; i < baInput.size(); i++)
    {
        for (j=0; j < baPassword.size(); j++)
        {
            // XOR - кодировка символа
            baInput[i] = baInput[i] ^ (baPassword[j] + (i*j));
        }
    }

    //Возврат кодированной строки
    return QString::fromLocal8Bit(baInput.data());
}
