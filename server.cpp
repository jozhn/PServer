#include "server.h"
#include "serverthread.h"
#include <QDir>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
}

void Server::startServer(quint16 port)
{
    listen(QHostAddress::LocalHost, port);
    //qDebug() << "Server started on port "<<port;
}

void Server::stopServer()
{
	close();
}

/*在服务器端使用tcpServer的incomingConnection()函数
来获取已经建立的连接的Tcp套接字，
使用它来完成数据的接收和其它操作*/

void Server::incomingConnection(int handle)
{
    serverThread* thread = new serverThread(handle, this);
    connect(thread, SIGNAL(error(int)), this, SLOT(displayError(int)));
    connect(thread, SIGNAL(started()), this, SLOT(started()));
    connect(thread, SIGNAL(finished()), this, SLOT(finished()));
    /*关联finished()和deleteLater()槽函数，发送完成时就会断开连接，
    调用deleteLater()函数保证在关闭连接后删除该套接字。*/
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Server::displayError(int e)
{
    qDebug() << "The clientThread have a error. " << e << ".";
}

void Server::started()
{
    qDebug() << "a clientThread started.";
    emit updateTable();
}

void Server::finished()
{
	qDebug() << "a clientThread finished.";
    emit updateTable();
}
