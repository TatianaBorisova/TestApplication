#include "tcpclient.h"

#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QNetworkInterface>

namespace {
const QString hostName = QString("localhost");
const int portNumber = 33333;
const QString divSymbol = ";";
const QString zeroIp = "0.0.0.0";
const QString notFoundIp = "127.0.0.1";
}

TcpClient::TcpClient(QObject *parent) :
    QObject(parent),
    m_pTcpSocket(new QTcpSocket(this)),
    m_nNextBlockSize(0),
    m_port(portNumber),
    m_connectionState(-1)
{
    if(!findLocalIpv4InterfaceData())
        setServerIp(notFoundIp);
}

bool TcpClient::findLocalIpv4InterfaceData()
{
    //Firstly get IP and try to find server on local host
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (!address.isNull() && address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost)) {

            m_localHost = address.toString();

            if (connectToHost(address.toString(), m_port)) {
                setServerIp(address.toString());
                return true;

            } else if (pingServerInNetwork()) {
                return true;
            }
        }
    }
    return false;
}

bool TcpClient::pingServerInNetwork()
{
    //Then find mask/broadcast
    QList<QNetworkInterface> interface = QNetworkInterface::allInterfaces();
    for (int i = 0; i < interface.size(); i++)
    {
        QNetworkInterface item = interface.at(i);
        QList<QNetworkAddressEntry> entryList = item.addressEntries();

        for (int j = 0; j < entryList.size(); j++)
        {
            if (m_localHost == entryList.at(j).ip().toString()) {
                m_mask = entryList.at(j).netmask().toString();

                if (m_connectionState != 0) {

                    QList<int> ipList = getNumbersFromIp(m_localHost);
                    QList<int> maskList = getNumbersFromIp(m_mask);
                    QList<int> networkAddress;


                    int local = -1;

                    if (ipList.count() == maskList.count()) {

                        for (int j = 0; j < ipList.count(); j++) {
                            networkAddress.append((ipList.at(j) & maskList.at(j)));
                        }

                        for (int j = 0; j < ipList.count(); j++) {
                            if (ipList.at(j) != (ipList.at(j) & maskList.at(j))) {
                                local = j;
                                break;
                            }
                        }

                        if (local != -1) {
                            switch (local) {
                            case 1:
                            {
                                for (int l = networkAddress.at(1); l <= 255; l++) {
                                    for (int k = 0; k <= 255; k++) {
                                        for (int n = 0; n <= 255; n++) {
                                            QString tmpIP = QString::number(networkAddress.at(0)) + "." + QString::number(l) + "." + QString::number(k) + "." + QString::number(n);
                                            if (connectToHost(tmpIP, m_port)) {
                                                setServerIp(tmpIP);
                                                return true;
                                            }
                                        }
                                    }
                                }
                            } break;
                            case 2:
                            {
                                for (int k = networkAddress.at(2); k <= 255; k++) {
                                    for (int n = 0; n <= 255; n++) {
                                        QString tmpIP = QString::number(networkAddress.at(0)) + "." + QString::number(networkAddress.at(1)) + "." + QString::number(k) + "." + QString::number(n);
                                        if (connectToHost(tmpIP, m_port)) {
                                            setServerIp(tmpIP);
                                            return true;
                                        }
                                    }
                                }
                            } break;
                            case 3:
                            {
                                for (int n = networkAddress.at(3); n <= 255; n++) {
                                    QString tmpIP = QString::number(networkAddress.at(0)) + "." + QString::number(networkAddress.at(1)) + "." + QString::number(networkAddress.at(2)) + "." + QString::number(n);
                                    if (connectToHost(tmpIP, m_port)) {
                                        setServerIp(tmpIP);
                                        return true;
                                    }
                                }
                            } break;
                            case 0:
                            default:
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

QList<int> TcpClient::getNumbersFromIp(const QString &value)
{
    QList<int> list;
    QStringList strings = value.split(".");

    for(int i = 0; i < strings.count(); i++) {
        int networkValue = strings.at(i).toInt();
        list.append(networkValue);
    }

    return list;
}

void TcpClient::userTryConnectToHost(const QString &host, int port)
{
    setServerPort(port);
    if (host == zeroIp) {
        if (!findLocalIpv4InterfaceData()) {
            return;
        }
    } else {
        if (connectToHost(host, port)) {
            setServerIp(host);
        }
    }
}

bool TcpClient::connectToHost(const QString &host, int port)
{
    m_pTcpSocket->connectToHost(host, port);

    if (!m_pTcpSocket->waitForConnected(10)) {
        emit connected(m_connectionState = -1);
        return false;
    } else {
        emit connected(m_connectionState = 0);
    }

    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError)));

    return true;
}

void TcpClient::disconnectHost()
{
    m_pTcpSocket->disconnectFromHost();
    disconnect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    disconnect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    disconnect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
               this,         SLOT(slotError(QAbstractSocket::SocketError)));
    emit connected(m_connectionState = -1);
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

    emit connected(m_connectionState = -1);

    QMessageBox::warning(0, "Error", strError);
}

void TcpClient::sendToServer(const StudentResult &result)
{
    //create msg string
    QString studentResult = result.testName
            + divSymbol
            + result.firstName
            + divSymbol
            + result.secondName
            + divSymbol
            + result.surname
            + divSymbol
            + result.group
            + divSymbol
            + QString::number(result.score)
            + divSymbol
            + QString::number(result.maxPosibleScore);

    //non latin symbols have more size then latin,
    //so string length != real symbols array size
    QByteArray bytes = studentResult.toUtf8();
    //calculate msg sum
    int msgSize = headerMsgSize + bytes.length();

    //put data to bytearray
    QByteArray  arrBlock;
    arrBlock.fill(0, msgSize);
    arrBlock.insert(0, QString::number(msgSize));
    arrBlock.insert(headerMsgSize, studentResult);
    arrBlock.resize(msgSize);

    //send data to server
    qDebug() << m_pTcpSocket->write(arrBlock);
    m_pTcpSocket->waitForBytesWritten(3000);
}

void TcpClient::slotConnected()
{
    qDebug() << ("Received the connected() signal");
}

QString TcpClient::getServerIp() const
{
    return m_host;
}

int TcpClient::getServerPort() const
{
    return m_port;
}

int TcpClient::getErrorState() const
{
    return m_connectionState;
}

void TcpClient::setServerIp(const QString &ip)
{
    m_host = ip;
    emit serverIpChanged(m_host);
}

void TcpClient::setServerPort(int port)
{
    m_port = port;
    emit serverPortChanged(m_port);
}
