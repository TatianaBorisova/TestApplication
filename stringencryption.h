#ifndef STRINGENCRYPTION_H
#define STRINGENCRYPTION_H

#include <QObject>

class StringEncryption : public QObject
{
    Q_OBJECT
public:
    explicit StringEncryption(QObject *parent = 0);

    static QString stringDecrypt(const QString &strInput, const QString &strPassword);
    static QString stringEncrypt(const QString &strInput, const QString &strPassword);
};

#endif // STRINGENCRYPTION_H
