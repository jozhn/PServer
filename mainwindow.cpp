#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller/recogize.h"
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QssMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/qss/app.png"));
    setWindowTitle("Server");
    row = 0;
    unrecModel = new QSqlQueryModel(this);
    successModel = new QSqlQueryModel(this);
    failModel = new QSqlQueryModel(this);
    fileUtil = new FileUtil();
    server = new Server;
    sourcePic = new QImage;
    resultPic = new QImage;
    initUnrecTable();
    initSuccessTable();
    initFailTable();
    fileUtil->addType(ui->typeBox);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sourcePic;
    delete resultPic;
}

void MainWindow::ResizeTableView(QTableView *tableview)
{
    tableview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应列宽
    tableview->resizeRowsToContents();//最小行高
}

void MainWindow::initUnrecTable()
{
    fileUtil->setUnrecModel();
    unrecModel = fileUtil->getModel();
    unrecModel->setHeaderData(1, Qt::Horizontal, tr("文件名"));
    unrecModel->setHeaderData(2, Qt::Horizontal, tr("类型"));
    unrecModel->setHeaderData(3, Qt::Horizontal, tr("地点"));
    unrecModel->setHeaderData(4, Qt::Horizontal, tr("接收状态"));
    ResizeTableView(ui->unrecTableView);
    ui->unrecTableView->setModel(unrecModel);
    ui->unrecTableView->hideColumn(0);
}

void MainWindow::initSuccessTable()
{
    fileUtil->setSuccessModel();
    successModel = fileUtil->getModel();
    successModel->setHeaderData(1, Qt::Horizontal, tr("文件名"));
    successModel->setHeaderData(2, Qt::Horizontal, tr("类型"));
    successModel->setHeaderData(3, Qt::Horizontal, tr("地点"));
    successModel->setHeaderData(4, Qt::Horizontal, tr("接收状态"));
    ResizeTableView(ui->successTableView);
    ui->successTableView->setModel(successModel);
    ui->successTableView->hideColumn(0);
}

void MainWindow::initFailTable()
{
    fileUtil->setFailModel();
    failModel = fileUtil->getModel();
    failModel->setHeaderData(1, Qt::Horizontal, tr("文件名"));
    failModel->setHeaderData(2, Qt::Horizontal, tr("类型"));
    failModel->setHeaderData(3, Qt::Horizontal, tr("地点"));
    failModel->setHeaderData(4, Qt::Horizontal, tr("接收状态"));
    ResizeTableView(ui->failTableView);
    ui->failTableView->setModel(failModel);
    ui->failTableView->hideColumn(0);
}

void MainWindow::on_startService_clicked()
{
    quint16 port = ui->portEdit->text().toUInt();
    server->startServer(port);
    ui->statusEdit->setText("开始监听localhost "+QString::number(port)+"端口");
    connect(server, SIGNAL(updateTable()), this, SLOT(on_refreshUnrec_clicked()));
}

void MainWindow::on_stopService_clicked()
{
    server->stopServer();
    ui->statusEdit->setText("服务端停止");
}

void MainWindow::on_refreshUnrec_clicked()
{
    initUnrecTable();
}

void MainWindow::on_recognizeAll_clicked()
{
    QSqlQuery query;
    Recogize *r = new Recogize();
    QString sql = "select * from send_record where receive_state='已接收' and rec_state='待识别'";
    query.exec(sql);
    int fileId,type;QString fileName,location;
    while(query.next()){
        fileId = query.value("id").toInt();//qDebug()<<fileId;
        fileName = query.value("filename").toString();//qDebug()<<fileName;
        type = query.value("type").toInt();//qDebug()<<type;
        location = query.value("location").toString();//qDebug()<<location;
        if(r->rec("E:/source/",fileName)){
            fileUtil->addSuccessRec(fileId,r->plateColor,r->plateNum,type,location,QDateTime::currentDateTime());
        }else{
            fileUtil->addFailRec(fileId,type,location,QDateTime::currentDateTime());
        }
    }
    initUnrecTable();
    initSuccessTable();
    initFailTable();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    fileUtil->deleteAll();
}

void MainWindow::on_refreshSuccess_clicked()
{
    initSuccessTable();
}

void MainWindow::on_refreshFail_clicked()
{
    initFailTable();
}

void MainWindow::on_successTableView_clicked(const QModelIndex &index)
{
    row = index.row();
    if(row!=-1){
        int fileId = ui->successTableView->model()->data( ui->successTableView->model()->index(row,0) ).toInt();
        PlateRecord pr = fileUtil->getPlateRecord(fileId);
        sourcePic->load("E:/source/"+pr.fileName);
        resultPic->load("E:/result/"+pr.fileName);
        int width = ui->sourceLabel->width();int height = ui->sourceLabel->height();
        ui->sourceLabel->setPixmap(QPixmap::fromImage(*sourcePic).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        width = ui->resultLabel->width();height = ui->resultLabel->height();
        ui->resultLabel->setPixmap(QPixmap::fromImage(*resultPic).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->plateColorEdit->setText(pr.plateColor);
        ui->plateStrEdit->setText(pr.plateNum);
        ui->typeBox->setCurrentIndex(pr.type);
        ui->pointsEdit->setText(QString::number(pr.points));
        ui->fineEdit->setText(QString::number(pr.fine));
        ui->locationEdit->setText(pr.location);
    }
}

void MainWindow::on_lastRec_clicked()
{
    if(row!=-1 && (row-1)>=0){
        row--;
        QModelIndex index = ui->successTableView->model()->index(row,1);
        ui->successTableView->setCurrentIndex(index);
        on_successTableView_clicked(index);
    }
}

void MainWindow::on_nextRec_clicked()
{
    if(row!=-1 && (row+1)<ui->successTableView->model()->rowCount()){
        row++;
        QModelIndex index = ui->successTableView->model()->index(row,1);
        ui->successTableView->setCurrentIndex(index);
        on_successTableView_clicked(index);
    }
}
