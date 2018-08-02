#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QEvent>
#include "qss/qss.h"
#include "server.h"

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
    void ResizeTableView(QTableView *tableview);
private slots:
    void on_startService_clicked();
    void on_stopService_clicked();
    void on_refreshUnrec_clicked();
    void on_recognizeAll_clicked();
    void on_refreshSuccess_clicked();
    void on_refreshFail_clicked();
    void on_successTableView_clicked(const QModelIndex &index);
    void on_lastRec_clicked();
    void on_nextRec_clicked();

private:
    Ui::MainWindow *ui;
    Server *server;
    FileUtil *fileUtil;
    QSqlQueryModel *unrecModel;
    QSqlQueryModel *successModel;
    QSqlQueryModel *failModel;
    void closeEvent(QCloseEvent *e);
    int row;
};

#endif // MAINWINDOW_H
