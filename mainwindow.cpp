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

//    sourcePic = new QImage;
//    resultPic = new QImage;

}

MainWindow::~MainWindow()
{
    delete ui;
//    delete sourcePic;
//    delete resultPic;
}

void MainWindow::on_testButton_clicked()
{
    Recogize *r = new Recogize();
    QString dir = "E://";
    QString filename = "test.jpg";
    if(r->rec(dir,filename)){
        qDebug()<<"ok";
        ui->plateColorEdit->setText(r->plateColor);
        ui->plateStrEdit->setText(r->plateNum);

        QImage *sourcePic = new QImage;
        QImage *resultPic = new QImage;
        sourcePic->load(r->sourcePath);
        resultPic->load(r->resultPath);
        int width = ui->sourceLabel->width();int height = ui->sourceLabel->height();
        ui->sourceLabel->setPixmap(QPixmap::fromImage(*sourcePic).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        width = ui->resultLabel->width();height = ui->resultLabel->height();
        ui->resultLabel->setPixmap(QPixmap::fromImage(*resultPic).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//        sourcePic->load(r->sourcePath);
//        QGraphicsScene *scene = new QGraphicsScene;
//        scene->addPixmap(QPixmap::fromImage(*sourcePic));
//        ui->capturePic->setScene(scene);
//        ui->capturePic->show();
//        ui->textEdit->append(r->resultPath);
    }
}
