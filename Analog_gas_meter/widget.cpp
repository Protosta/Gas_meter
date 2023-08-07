#include "widget.h"
#include <QGridLayout>
#include <QHBoxLayout>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    te = new QTextEdit;
    re = new QTextEdit;
    le1 = new QLineEdit;
    le2 = new QLineEdit;
    le3 = new QLineEdit;
    tem = new QLineEdit;
    pre = new QLineEdit;
    dat = new QLineEdit;
    pb1 = new QPushButton("发送");
    pb4 = new QPushButton("发送常规数据");
    pb3 = new QPushButton("计量故障");
    pb5 = new QPushButton("欠压关阀故障");
    pb6 = new QPushButton("燃气泄露");
    pb7 = new QPushButton("流量异常");
    t1 = new QLabel("目标IP地址");
    t2 = new QLabel("目标端口号");
    t3 = new QLabel("服务器--->模拟表");
    t4 = new QLabel("模拟表--->服务器");
    t5 = new QLabel("本机IP地址");
    t6 = new QLabel("设置温度(℃)");
    t7 = new QLabel("设置压力(Kpa)");
    t8 = new QLabel("燃气表读数(m³/h)");
    t9 = new QLabel("异常告警");
    opengroup = new QGroupBox("阀门",this);
    op = new QRadioButton("打开", opengroup);
    cl= new QRadioButton("关闭",opengroup);
    op->setChecked(true);
    opengroup->setGeometry(30, 250,200, 100);
    QHBoxLayout *hbox = new QHBoxLayout(opengroup);
    hbox->addWidget(op);
    hbox->addWidget(cl);
    QGridLayout *gbox = new QGridLayout;
    gbox->addWidget(t5,0,0,1,1);
    gbox->addWidget(le1,0,1,1,1);
    gbox->addWidget(t1,1,0,1,1);
    gbox->addWidget(le3,1,1,1,1);
    gbox->addWidget(t2,2,0,1,1);
    gbox->addWidget(le2,2,1,1,1);
    gbox->addWidget(t6,4,0,1,1);
    gbox->addWidget(tem,4,1,1,1);
    gbox->addWidget(t7,5,0,1,1);
    gbox->addWidget(pre,5,1,1,1);
    gbox->addWidget(t8,6,0,1,1);
    gbox->addWidget(dat,6,1,1,1);
    gbox->addLayout(hbox,7,0,1,1);
    gbox->addWidget(pb4,11,0,1,1);
    gbox->addWidget(t9,12,0,1,1);
    gbox->addWidget(pb3,13,0,1,1);
    gbox->addWidget(pb5,13,1,1,1);
    gbox->addWidget(pb6,14,0,1,1);
    gbox->addWidget(pb7,14,1,1,1);
    gbox->addWidget(t3,0,3,1,6);
    gbox->addWidget(re,1,3,6,6);
    gbox->addWidget(t4,7,3,1,6);
    gbox->addWidget(te,8,3,6,6);
    gbox->addWidget(pb1,14,3,1,6);

    setLayout(gbox);
    udpsock = new QUdpSocket;
    udpsock->bind(QHostAddress("10.88.168.93"), 8002); //******本机IP以及端口，配合server使用
    le1->setText("10.88.168.93");
    le3->setText("10.88.168.93");
    le2->setText("8001");
    tem->setText("35");
    pre->setText("1024");
    dat->setText("2000");
    connect(pb1, SIGNAL(clicked(bool)), this, SLOT(senddata()));
    connect(udpsock, SIGNAL(readyRead()), this, SLOT(recvdata()));
    connect(pb3, SIGNAL(clicked(bool)), this, SLOT(send_calwarn()));
    connect(pb4, SIGNAL(clicked(bool)), this, SLOT(sendusualdata()));
    connect(pb5, SIGNAL(clicked(bool)), this, SLOT(send_prewarn()));
    connect(pb6, SIGNAL(clicked(bool)), this, SLOT(send_leakwarn()));
    connect(pb7, SIGNAL(clicked(bool)), this, SLOT(send_curwarn()));
    connect(op, &QRadioButton::toggled, [=](bool isChecked){
           if (isChecked == true)
           {
               sta &= 0b10110111;
               qDebug() << "阀门打开";
           }
           else if (isChecked == false)
           {
               sta |= 0b01001000;
               qDebug() << "阀门关闭";
           }
       });
}

Widget::~Widget()
{

}
