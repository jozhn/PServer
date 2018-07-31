#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QString>
#include <QDir>




class serverThread : public QThread
{
    Q_OBJECT
public:
    explicit serverThread(int h, QObject *parent = 0);
	void proccessData(QByteArray& array);
   // void setPath();

protected:
	void run();

signals:
	void error(int);

public slots:
	void on_socket_readyRead();
	void on_socket_disconnected();
	void displaySocketError(QAbstractSocket::SocketError e);

private:
	QFile file;
    QFile *p_file;
	QString fileName;
	//QAbstractSocket s;
    QTcpSocket socket;
	qint64 blockSize;
	qint64 blockNumber;

    QDir my_dir;


};

#endif // CLIENTTHREAD_H
