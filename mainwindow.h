#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private slots:
    void on_startService_clicked();
    void on_stopService_clicked();
//    void on_testButton_clicked();

private:
    Ui::MainWindow *ui;
    Server *server;
};

#endif // MAINWINDOW_H
