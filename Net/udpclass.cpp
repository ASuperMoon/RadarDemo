#include "udpclass.h"

UDPClass::UDPClass(QObject *parent): QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);
    QObject::connect(m_udpSocket, &QUdpSocket::readyRead, this, &UDPClass::ReadPendingDatagrams);
}

bool UDPClass::Bind(const QString& localAddress,quint16 port)
{
    bool ret = false;

    m_udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, m_recbufferSize);
    ret = m_udpSocket->bind(QHostAddress(localAddress), port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(!ret)  qWarning()<<m_udpSocket->errorString();
    return ret;
}

void UDPClass::ReadPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress senderAddress;
        quint16 senderPort;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(),&senderAddress,&senderPort);
        emit DataReceived(datagram);
    }
}

void UDPClass::UDPSend(const QByteArray &data, const QString &address, const int &port)
{
    m_udpSocket->writeDatagram( data,QHostAddress(address),port);
}
