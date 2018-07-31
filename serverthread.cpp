#include "serverthread.h"
#include <QAbstractSocket>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QHostAddress>
#include <QString>
#include <QDir>


serverThread::serverThread(int h, QObject *parent) :
    QThread(parent),socket(parent) /*s(QAbstractSocket::TcpSocket, parent)*//*, file(parent)*/
{
    //设置上传到服务器固定位置  permanent 固定的；永久的
    //创建一个目录的对象
    QDir d;
    //创建一个目录，就是文件夹。其实文件夹就是目录，
    //你在同一个硬盘下复制东西都很快，就是因为硬盘只改变了目录！
    d.mkpath("E:/source");
    //创建以个tmp的文件放到你刚创建的文件夹内。
    p_file = new QFile("E:/source/tmp.txt");
    //打开文件，大家可以试下 没有第4个文件显示不出来
    p_file->open(QFile::ReadOnly);

	blockSize = 0;
	blockNumber  = 0;
    socket.setSocketDescriptor(h);
    connect(&socket, SIGNAL(disconnected()), this, SLOT(on_socket_disconnected()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(on_socket_readyRead()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(displaySocketError(QAbstractSocket::SocketError)));


}

void serverThread::proccessData(QByteArray &array)
{


	QDataStream in(&array, QIODevice::ReadOnly);

    /*如果你希望数据和以前版本的Qt一致，请使用setVersion()。
    如果你希望数据是人们可读的，比如，用于调试，
    你可以用setPrintableData()设置数据流为可打印数据模式。
    然后这个数据写起来慢一些，并且膨胀起来但已经是人们可以读取的格式了。*/

    in.setVersion(QDataStream::Qt_5_3);
    quint16 key;
	QByteArray data;
	in >> key >> data;
	blockNumber ++;
	qDebug() << "BlockNumber " << blockNumber << ", size: " << blockSize+sizeof(qint64) << ".";
	qDebug() << "\tKey: " << key << "\t\tSize: " << data.size() << ".";


    switch(key)
	{
    //0x0001 16进制 无符型整数
	case 0x0001:		// file name
        fileName = "E:/source/"+fileName.fromUtf8(data.data(), data.size());

        //打开文件的方式有2种，一种是在构造函数中指定文件名
        //QFile file（“文件名”）
        //另一种是使用setFileName（）函数设置文件名
       p_file->setFileName(fileName);


        //判断目录里是否存在用exists（） exists存在
        if(p_file->exists(fileName))
		{
			qDebug() << "File was exists. now remove it!";
            //remove()删除文件
            p_file->remove(fileName);
        }
        //打开，以只写的方式
        if(!p_file->open(QIODevice::WriteOnly))
		{
            qDebug() << "Can not open file " << p_file->fileName() << ".";
			throw 2;
		}
        else
        {
            qDebug() << "open file:" << p_file->fileName() << ".";
        }
		break;
	case 0x0002:		// file data
        p_file->write(data.data(), data.size());
        //flush（） 清除缓冲
        p_file->flush();
		break;
	case 0x0003:		// file EOF
		qDebug() << "File transt finished.";
        p_file->close();
        //socket断开连接
        socket.disconnectFromHost();
		//emit finished();
		break;
	default: ;
	}

}

//线程的入口

void serverThread::run()
{
    /*address()和port()返回连接所使用的IP地址和端口。
    peerAddress()和peerPort()函数返回自身所用到的IP地址和端口
    并且peerName()返回自身所用的名称
   （通常是被传送给connectToHost()的名字）。
    socket() 返回这个套接字所用到的QSocketDevice的指针。*/

    qDebug() << "New connect from " << socket.peerAddress().toString() << ":" << socket.peerPort() << ".";
	exec();
}

//socket断开连接

void serverThread::on_socket_disconnected()
{
	qDebug() << "socket disconnected.";
	exit();
//	emit finished();
}

//socket准备好读

void serverThread::on_socket_readyRead()
{
    //Available 可用的  socket传输的可用的字节大于或者等于
    while(socket.bytesAvailable() >= sizeof(quint64))
	{
		if(blockSize == 0)
		{
            if(socket.bytesAvailable() < sizeof(qint64))
				return;
            socket.read((char*)&blockSize, sizeof(qint64));  // read blockSize
		}

        if(socket.bytesAvailable() < blockSize)				// have no enugh（足够） data
			return;
        QByteArray data = socket.read(blockSize);
        proccessData(data);
		blockSize = 0;
	}
}

void serverThread::displaySocketError(QAbstractSocket::SocketError e)
{
    qDebug() << "Error: socket have error " << e << "\n\t" << socket.errorString() << ".";
    qDebug() << "Removed " << p_file->fileName() << ".";
    if(p_file->isOpen())
        p_file->close();
	else
		return;
    if(!p_file->fileName().isEmpty())
        p_file->remove(fileName);
	else
		return;
	emit error(2);
	//exit();
}
