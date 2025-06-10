#pragma once
#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <QThread>

class UDPClass : public QObject
{
    Q_OBJECT 
public:
    UDPClass(QObject *parent = nullptr);
signals:
    void DataReceived(const QByteArray &data);
public slots:
    bool Bind(const QString& localAddress,quint16 port);
    void ReadPendingDatagrams();
    void UDPSend(const QByteArray &data,const QString &address,const int &port);
private:
    QUdpSocket *m_udpSocket;
    int m_recbufferSize = 1024 * 1024;
};
#endif // UDPSERVER_H
