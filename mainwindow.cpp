#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller/recogize.h"
#include "controller/recordutil.h"
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QSettings>
#include <QAxObject>
#include <QStandardPaths>
#include <QFileDialog>

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
	personModel = new QSqlQueryModel(this);
    fileUtil = new FileUtil();
	personUtil=new PersonUtil();
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
    tableview->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应列宽
    tableview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableview->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //tableview->resizeRowsToContents();//最小行高
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

void MainWindow::on_submitSuccess_clicked()
{
    RecordUtil * util = new RecordUtil();
    if(util->submitRecord(QDateTime::currentDateTime())){
        initSuccessTable();
        QssMessageBox::tips("提交成功",this,tr("提示"));
    }
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


void MainWindow::on_failTableView_clicked(const QModelIndex &index)
{
    row = index.row();
    if(row!=-1){
        int fileId = ui->failTableView->model()->data( ui->failTableView->model()->index(row,0) ).toInt();
        PlateRecord pr = fileUtil->getPlateRecord(fileId);
        sourcePic->load("E:/source/"+pr.fileName);
        int width = ui->sourceLabel->width();int height = ui->sourceLabel->height();
        ui->sourceLabel->setPixmap(QPixmap::fromImage(*sourcePic).scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->resultLabel->clear();
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

void MainWindow::on_searchRecord_clicked()
{
    on_initRecordTable_clicked();
    personUtil->searchItem(ui->carownerEdit->text(),ui->platenumEdit->text(),ui->idcardEdit->text());
}

void MainWindow::on_initRecordTable_clicked()
{
    personUtil->setModel();
    personModel = personUtil->getModel();
    personModel->setHeaderData(1, Qt::Horizontal, tr("名字"));
    personModel->setHeaderData(2, Qt::Horizontal, tr("驾照号"));
    personModel->setHeaderData(3, Qt::Horizontal, tr("车牌号"));
    personModel->setHeaderData(4, Qt::Horizontal, tr("地点"));
    personModel->setHeaderData(6, Qt::Horizontal, tr("类型"));
    personModel->setHeaderData(7, Qt::Horizontal, tr("扣分"));
    personModel->setHeaderData(8, Qt::Horizontal, tr("罚款"));
    personModel->setHeaderData(9, Qt::Horizontal, tr("是否扣分"));
    ResizeTableView(ui->recordTableView);
    ui->recordTableView->setModel(personModel);
    ui->recordTableView->hideColumn(0);
    ui->recordTableView->hideColumn(5);
}

void MainWindow::on_doDeduction_clicked()
{
    on_initRecordTable_clicked();
    recordRow=0;
    while(recordRow!=-1 && (recordRow+1)<=ui->recordTableView->model()->rowCount()){
        QModelIndex index1=ui->recordTableView->model()->index(recordRow,5);
        QModelIndex index2=ui->recordTableView->model()->index(recordRow,7);
        int tyid=index1.data().toInt();
        int typo=index2.data().toInt();
        qDebug()<<tyid;
        qDebug()<<typo;
        personUtil->personFine(tyid,typo);
        recordRow++;
    }
    QssMessageBox::tips("扣分完成",this,tr("提示"));
}


void MainWindow::on_saveResult_clicked()
{
    QTableView *tableview;
    if(ui->tabWidget->currentIndex()==1)
        tableview=ui->successTableView;
    else if(ui->tabWidget->currentIndex()==2)
        tableview=ui->failTableView;

    int currentrow = tableview->currentIndex().row();
    if(currentrow==-1)
        return;
    int fileId = tableview->model()->data( tableview->model()->index(currentrow,0) ).toInt();
    qDebug()<<fileId;
    QString plate_color = ui->plateColorEdit->text();
    QString plate_num = ui->plateStrEdit->text();
    qDebug()<<plate_num;
    int type = ui->typeBox->currentIndex();
    QString location = ui->locationEdit->text();
    RecordUtil * util = new RecordUtil();
    if(util->save(fileId,plate_color,plate_num,type,location)){
        QssMessageBox::tips("保存成功",this,tr("提示"));
        initSuccessTable();
        initFailTable();
        tableview->setCurrentIndex( tableview->model()->index(currentrow,0) );
    }
}

void MainWindow::on_typeBox_currentIndexChanged(int index)
{
    QSqlQuery query;
    query.exec(QString("select points,fine from type where id=%1").arg(index));
    while(query.next()){
        ui->pointsEdit->setText(query.value("points").toString());
        ui->fineEdit->setText(query.value("fine").toString());
    }
}

void MainWindow::on_logout_clicked()
{
    QString path = QString("./user.ini");

    //创建文件
    QSettings * config =  new QSettings(path,QSettings::IniFormat);

    //写入
    config->beginGroup("config");
    config->setValue("auto_states",QString::number(0));
    config->endGroup();

    server->stopServer();
    Login *l=new Login();
    l->show();
    this->closeWindow();
}

void MainWindow::on_buttonExport_clicked()
{
    exportExcel(ui->recordTableView);
}


void MainWindow::exportExcel(QTableView *tableview)
{
    QString filePath = QFileDialog::getSaveFileName(this, "文件保存", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    "Excel 文件(*.xls *.xlsx)");
    if(!filePath.isEmpty()){
        qDebug()<<"connect successfully";
        QAxObject *excel = new QAxObject(this);
        excel->setControl("Excel.Application");//连接excel控件
        excel->dynamicCall("SetVisible(bool Visible)", false);//不显示窗体
        excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭
        QAxObject *workbooks = excel->querySubObject("WorkBooks");
        workbooks->dynamicCall("Add");
        QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
        QAxObject *worksheets = workbook->querySubObject("Sheets");
        QAxObject *worksheet = worksheets->querySubObject("Item(int)", 1);
        int rowCount = tableview->model()->rowCount();
        int columnCount =  tableview->model()->columnCount();
        QAxObject *cell,*col;
        QString merge_cell=QString("A1:Z%1").arg(rowCount+2); // 设置单元格的属性
        QAxObject *merge_range = worksheet->querySubObject("Range(const QString&)", merge_cell);
        merge_range->setProperty("HorizontalAlignment", -4108); // 水平居中
        merge_range->setProperty("VerticalAlignment", -4108);  // 垂直居中
        merge_range->setProperty("NumberFormatLocal", "@");  // 设置为文本
        //列标题
        for(int i=0;i<columnCount;i++)
        {
            QString columnName;
            columnName.append(QChar(i  + 'A'));
            columnName.append(":");
            columnName.append(QChar(i + 'A'));
            col = worksheet->querySubObject("Columns(const QString&)", columnName);
            cell=worksheet->querySubObject("Cells(int,int)", 1, i+1);
            columnName=tableview->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
            cell->dynamicCall("SetValue(const QString&)", columnName);
        }
        for(int i = 0; i < rowCount; ++i){
            for(int j =0 ; j < columnCount; ++j){
                QModelIndex index = tableview->model()->index(i, j);
                QString strdata=tableview->model()->data(index).toString();
                worksheet->querySubObject("Cells(int,int)", i+2, j+1)->dynamicCall("SetValue(const QString&)", strdata);
            }
        }
        workbook->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(filePath));
        if(excel != NULL){
            excel->dynamicCall("Quit()");
            delete excel;
            excel = NULL;
        }
        QssMessageBox::tips("导出数据成功",this,tr("提示"));
    }
}

void MainWindow::on_deleteFail_clicked()
{
    if(fileUtil->deleteFail())
        QssMessageBox::tips("清除成功",this,tr("提示"));
}
