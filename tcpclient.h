#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "global.h"

#include <QObject>
#include <QTcpSocket>

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
    void startFileDownloading();
    void sendDownloadedFilePath(const QString &path);

public slots:
    bool connectToHost(const QString &host, int port);
    void tryConnectToHost(const QString &host, int port);
    void disconnectHost();

    void sendTestResultToServer(const StudentResult &result);
    void sendFilesGettingRequest();
    void sendDownLoadFileRequest(const QString &filename);

    QString getServerIp() const;
    int getServerPort() const;
    int getErrorState() const;

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    bool findLocalIpv4InterfaceData();
    QList<int> convertIpToInt(const QString &value);
    void setServerIp(const QString &ip);
    void setServerPort(int port);
    bool serachServerInNetwork();
    void getDataFileByFile();
    void processFileList(const QString &fullMsg);
    void processFileSaving(const QByteArray &fullMsg);

private:
    //TCP client and connection
    QTcpSocket      *m_pTcpSocket;
    quint16          m_nNextBlockSize;
    QString          m_host;
    QString          m_localHost;
    int              m_port;
    QString          m_mask;
    int              m_connectionState;
    QStringList      m_filelist;
};

#endif // TCPCLIENT_H
