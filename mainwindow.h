#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QEvent>
#include "qss/qss.h"
#include "server.h"
#include "controller/fileutil.h"
#include "controller/personutil.h"
#include "login.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QssMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage *sourcePic;
    QImage *resultPic;
    void initUnrecTable();
    void initSuccessTable();
    void initFailTable();
    void exportExcel(QTableView *tableview);
    void ResizeTableView(QTableView *tableview);
private slots:
    void on_startService_clicked();
    void on_stopService_clicked();
    void on_refreshUnrec_clicked();
    void on_recognizeAll_clicked();
    void on_refreshSuccess_clicked();
    void on_refreshFail_clicked();
    void on_submitSuccess_clicked();
    void on_deleteFail_clicked();
    void on_successTableView_clicked(const QModelIndex &index);
    void on_failTableView_clicked(const QModelIndex &index);
    void on_lastRec_clicked();
    void on_nextRec_clicked();
    void on_searchRecord_clicked();
    void on_initRecordTable_clicked();
    void on_doDeduction_clicked();
    void on_saveResult_clicked();
    void on_typeBox_currentIndexChanged(int index);
    void on_logout_clicked();
    void on_buttonExport_clicked();
private:
    Ui::MainWindow *ui;
    Server *server;
    FileUtil *fileUtil;
	PersonUtil *personUtil;
    QSqlQueryModel *unrecModel;
    QSqlQueryModel *successModel;
    QSqlQueryModel *failModel;
	QSqlQueryModel *personModel;
    void closeEvent(QCloseEvent *e);
    int row,recordRow;
};

#endif // MAINWINDOW_H
