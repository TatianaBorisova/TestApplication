#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "global.h"

#include <QObject>
#include <QTcpSocket>

class DownloadManager;

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

signals:
    void connected(int error = 0);
    void serverIpChanged(const QString &ip);
    void serverPortChanged(int port);
    void error(QAbstractSocket::SocketError err, const QString &errorStr);
    void fileLoadingError();

public slots:
    bool connectToHost(const QString &host, int port);
    void userTryConnectToHost(const QString &host, int port);
    void disconnectHost();
    void sendToServer(const StudentResult &result);
    void sendRequestToServer();
    QString getServerIp() const;
    int getServerPort() const;
    int getErrorState() const;

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    bool findLocalIpv4InterfaceData();
    QList<int> getNumbersFromIp(const QString &value);
    void setServerIp(const QString &ip);
    void setServerPort(int port);
    bool pingServerInNetwork();
    QString processUrl(const QString &url);

private:
    QTcpSocket      *m_pTcpSocket;
    quint16          m_nNextBlockSize;
    QString          m_host;
    QString          m_localHost;
    int              m_port;
    QString          m_mask;
    int              m_connectionState;
    DownloadManager *m_fileManager;
};

#endif // TCPCLIENT_H
