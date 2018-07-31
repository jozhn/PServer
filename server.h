﻿#ifndef NFSERVER_H
#define NFSERVER_H

#include <QTcpServer>
#include <QFile>
#include <QTcpSocket>
#include <QHostInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QCryptographicHash>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    void startServer(quint16 port);
	void stopServer();

protected:
	void incomingConnection(int handle);

signals:

public slots:
	void displayError(int);
	void finished();
    //void newConnect();
    //void readMessages();
private:

    //void sendMessages(QString msg);

    //bool verify(QString msg);


};

#endif // NFSERVER_H
