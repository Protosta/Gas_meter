#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>

class udpServer : public QObject
{
    Q_OBJECT
public:
    explicit udpServer(QObject *parent = 0);
    void init()
    {
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(QHostAddress("10.88.168.93"), 8001);   //server的端口设置，关键

        connect(udpSocket, SIGNAL(readyRead()),
                      this, SLOT(readPendingDatagrams()));
        qDebug()<<"init....";
    }
signals:

public slots:
    void readPendingDatagrams()
    {
        while (udpSocket->hasPendingDatagrams()) {
                  QByteArray datagram;
                  datagram.resize(udpSocket->pendingDatagramSize());
                  QHostAddress sender;
                  quint16 senderPort;

                  udpSocket->readDatagram(datagram.data(), datagram.size(),
                                          &sender, &senderPort);

                  //processTheDatagram(datagram);
                  qDebug()<<"recv: "<<datagram;
                  udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                           QHostAddress("10.88.168.93"), 8002);    //发送给client的端口，关键（client的端口查看client的widget.cpp中有配置）

              }
    }

private:
    QUdpSocket *udpSocket;
};

#endif // UDPSERVER_H
