#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QUdpSocket>
#include <QHostAddress>
#include <QLabel>
#include <string.h>
#include <ctime>
#include <QButtonGroup>
#include <QGroupBox>
#include <QRadioButton>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QByteArray>
class Widget : public QWidget
{
    Q_OBJECT
public slots:
    void senddata()
    {
        udpsock->writeDatagram(te->toPlainText().toStdString().c_str(), QHostAddress(le1->text()), le2->text().toInt());
    }

    void recvdata()
    {
        QByteArray datagram;
        datagram.resize(udpsock->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpsock->readDatagram(datagram.data(), datagram.size(),
                                          &sender, &senderPort);
        db.setDatabaseName("gas_meter.db");
        QString s=db.connectionName();
        db.open();
        query=new QSqlQuery();
        query->exec("create table user1(u,p)");
        QString str = QString(datagram);
        char * da = str.toLocal8Bit().data();
    //    query->exec("create table user2(u,p)");
        query->exec("insert into user1(u,p) values(str)") ;
        re->append(datagram);
        if(datagram=="01")
        {
            int temp = sta;
            temp &= 0b01000000;
            if(temp == 0b01000000)
            {udpsock->writeDatagram("44", QHostAddress(le1->text()), le2->text().toInt());}
            else
            {udpsock->writeDatagram("88", QHostAddress(le1->text()), le2->text().toInt());}
        }
        if(datagram=="44")
        {
            sta &= 0b10110111;
            int temp = sta;
            temp &= 0b01000000;
            if(temp == 0b01000000)
            {udpsock->writeDatagram("44", QHostAddress(le1->text()), le2->text().toInt());}
            else
            {udpsock->writeDatagram("88", QHostAddress(le1->text()), le2->text().toInt());}
        }
    }
    void send_calwarn()//计量故障
    {
        sta |= 0b00000010;
        udpsock->writeDatagram("7", QHostAddress(le1->text()), le2->text().toInt());db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("gas_meter.db");
        QString s=db.connectionName();
        db.open();
        query=new QSqlQuery();
        query->exec("create table user1(u,p)");
     //   query->exec("create table user2(u,p)");
        query->exec("insert into user1(u,p) values('计量故障','7')") ;
    }
    void send_prewarn()//欠压关阀
    {
        sta |= 0b00000100;
        udpsock->writeDatagram("30", QHostAddress(le1->text()), le2->text().toInt());
        db.setDatabaseName("gas_meter.db");
        QString s=db.connectionName();
        db.open();
        query=new QSqlQuery();
        query->exec("create table user1(u,p)");
     //   query->exec("create table user2(u,p)");
        query->exec("insert into user1(u,p) values('欠压关阀','30')") ;
    }
    void send_leakwarn()//燃气泄漏
    {
        sta |= 0b00100000;
        udpsock->writeDatagram("4", QHostAddress(le1->text()), le2->text().toInt());
        db.setDatabaseName("gas_meter.db");
        QString s=db.connectionName();
        db.open();
        query=new QSqlQuery();
        query->exec("create table user1(u,p)");
     //   query->exec("create table user2(u,p)");
        query->exec("insert into user1(u,p) values('燃气泄漏','4')") ;
    }
    void send_curwarn()//流量异常
    {
        udpsock->writeDatagram("6", QHostAddress(le1->text()), le2->text().toInt());
        db.setDatabaseName("gas_meter.db");
        QString s=db.connectionName();
        db.open();
        query=new QSqlQuery();
        query->exec("create table user1(u,p)");
     //   query->exec("create table user2(u,p)");
        query->exec("insert into user1(u,p) values('流量异常','6')") ;
    }
    void sendusualdata()
    {

        time_t now = time(0);
        tm *ltm = localtime(&now);
        QByteArray usualdata;
        //version
        usualdata.append((char)0x00);
        usualdata.append(0x01);

        //data_num
        usualdata.append((char)data_num);
        data_num++;

        //Cot_fst_t
        usualdata.append(ltm->tm_year>>8);
        usualdata.append(ltm->tm_year);
        usualdata.append(ltm->tm_mon>>8);
        usualdata.append(ltm->tm_mon);

        //Read_ival
        usualdata.append((char)0x00);
        usualdata.append(0x03); //采集周期3分钟

        //Rand_num
        int num,num1,num2,num3;
        num=rand();
        num1=rand();
        num2=rand();
        num3=rand();
        usualdata.append(num);
        usualdata.append(num1);
        usualdata.append(num2);
        usualdata.append(num3);


        //Reserve1
        usualdata.append((char)0xFF);
        usualdata.append((char)0xFF);
        usualdata.append((char)0xFF);
        usualdata.append((char)0xFF);

        //sig_value
        usualdata.append((char)0x00);
        usualdata.append(0x05);

        //DATA_COT_T
        usualdata.append(ltm->tm_mday>>8);
        usualdata.append(ltm->tm_mday);
        usualdata.append(ltm->tm_hour>>8);
        usualdata.append(ltm->tm_hour);

        //Meter_1
        usualdata.append(0x03);
        usualdata.append(0x03);

        //Meter_2
        usualdata.append(0x05);
        usualdata.append((char)0x00);

        //status_w
        usualdata.append(sta);

        //Total_Standard_flow
        usualdata.append((char)0xFF);
        usualdata.append((char)0xFF);
        usualdata.append((char)0x00);
        usualdata.append(0xFF);

        //Total_Work_flow
        usualdata.append((char)0xEE);
        usualdata.append((char)0xEE);
        usualdata.append((char)0x00);
        usualdata.append(0xEE);

        //Standard_flow
        usualdata.append(dat->text());

        //Work_flow
        usualdata.append(dat->text());

        //tem
        usualdata.append(tem->text());

        //pre
        usualdata.append(pre->text());

        udpsock->writeDatagram(usualdata.toHex(), QHostAddress(le1->text()),le2->text().toInt());
        db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("gas_meter.db");
        QString s=db.connectionName();
        db.open();
        query=new QSqlQuery();
        query->exec("create table user1(u,p)");
     //   query->exec("create table user2(u,p)");
        query->exec("insert into user1(u,p) values('常规数据','0001030079000a0003ba58e18affffffff0005000100000303050000ffff00ffeeee00ee3230303032303030333531303234')") ;
    }
public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    char sta = 0b00000000;
    int data_num=1;
    QTextEdit *te;
    QTextEdit *re;
    QPushButton *pb1,*pb3,*pb4,*pb5,*pb6,*pb7;
    QLabel *t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8,*t9,*t10;
    QLineEdit *le1,*le2,*le3,*tem,*pre,*dat;
    QRadioButton *op,*cl;
    QGroupBox *opengroup;
    QUdpSocket *udpsock;
    QSqlDatabase db;
    QSqlQuery *query;

};

#endif // WIDGET_H
