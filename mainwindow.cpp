#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller/recogize.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QssMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/qss/app.png"));
    setWindowTitle("Server");

    server = new Server;
    sourcePic = new QImage;
    resultPic = new QImage;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete sourcePic;
    delete resultPic;
}

void MainWindow::on_startService_clicked()
{
    quint16 port = ui->portEdit->text().toUInt();
    server->startServer(port);
    ui->statusEdit->setText("开始监听localhost "+QString::number(port)+"端口");
}

void MainWindow::on_stopService_clicked()
{
    server->stopServer();
    ui->statusEdit->setText("服务端停止");
}

//void MainWindow::on_testButton_clicked()
//{
//    Recogize *r = new Recogize();
//    QString dir = "E://";
//    QString filename = "test.jpg";
//    if(r->rec(dir,filename)){
//        qDebug()<<"ok";
//        ui->plateColorEdit->setText(r->plateColor);
//        ui->plateStrEdit->setText(r->plateNum);
//        sourcePic->load(r->sourcePath);
//        resultPic->load(r->resultPath);
//        int width = ui->sourceLabel->width();int height = ui->sourceLabel->height();
//        ui->sourceLabel->setPixmap(QPixmap::fromImage(*sourcePic).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//        width = ui->resultLabel->width();height = ui->resultLabel->height();
//        ui->resultLabel->setPixmap(QPixmap::fromImage(*resultPic).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//    }
//    else
//        qDebug()<<"error";
//}
