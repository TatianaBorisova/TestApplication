#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "global.h"

#include <QObject>
#include <QTcpSocket>

class QPushButton;

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

public slots:
    void connectToHost(const QString &host, int port);
    void disconnectToHost();
    void sendToServer(const StudentResult &result);

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();

private:
    QTcpSocket* m_pTcpSocket;
    quint16     m_nNextBlockSize;
};

#endif // TCPCLIENT_H
