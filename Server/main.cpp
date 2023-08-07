#include <QCoreApplication>
#include <udpserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    udpServer server;
    server.init();

    return a.exec();
}
