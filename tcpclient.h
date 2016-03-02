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

signals:
    void connected(int error = 0);

public slots:
    void connectToHost(const QString &host, int port);
    void disconnectToHost();
    void sendToServer(const StudentResult &result);
    QString getServerIp() const;
    int getServerPort() const;

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();

private:
    QTcpSocket* m_pTcpSocket;
    quint16     m_nNextBlockSize;
    QString     m_host;
    int         m_port;
};

#endif // TCPCLIENT_H
