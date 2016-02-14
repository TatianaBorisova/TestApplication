#include "tcpclient.h"

#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QTime>

namespace {
const QString hostName = QString("localhost");
const int portNumber = 33333;
const QString divSymbol = ";";
}

TcpClient::TcpClient(QObject *parent) :
    QObject(parent),
    m_pTcpSocket(new QTcpSocket(this)),
    m_nNextBlockSize(0)
{
    connectToHost(hostName, portNumber);
}

void TcpClient::connectToHost(const QString &host, int port)
{
    m_pTcpSocket->connectToHost(host, port);

    if (!m_pTcpSocket->waitForConnected(3000)) {
        qDebug() << "Client is unable to connect to Server ";
    }

    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError)));
}

void TcpClient::disconnectToHost()
{
    m_pTcpSocket->disconnectFromHost();
    disconnect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    disconnect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    disconnect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
               this,         SLOT(slotError(QAbstractSocket::SocketError)));
}

void TcpClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);

    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        qDebug() << time.toString() + " " + str;
        m_nNextBlockSize = 0;
    }
}

void TcpClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError
                                    ? "The host was not found."
                                    : err == QAbstractSocket::RemoteHostClosedError
                                      ? "The remote host is closed."
                                      : err == QAbstractSocket::ConnectionRefusedError
                                        ? "The connection was refused."
                                        : QString(m_pTcpSocket->errorString()));

    QMessageBox::warning(0, "Error", strError);
}

void TcpClient::sendToServer(const StudentResult &result)
{
    //create msg string
    QString studentResult =
            result.firstName
            + divSymbol
            + result.secondName
            + divSymbol
            + result.surname
            + divSymbol
            + result.group
            + divSymbol
            + QString::number(result.score);

    //calculate msg summ
    int msgSize = headerMsgSize + studentResult.length();

    //put data to bytearray
    QByteArray  arrBlock;
    arrBlock.fill(0, msgSize);
    arrBlock.insert(0, QString::number(msgSize));
    arrBlock.insert(headerMsgSize, studentResult);
    arrBlock.resize(msgSize);

    //send data to server
    m_pTcpSocket->write(arrBlock);
}

void TcpClient::slotConnected()
{
    qDebug() << ("Received the connected() signal");
}
