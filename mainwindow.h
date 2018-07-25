void on_testButton_clicked();
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qss/qss.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QssMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_testButton_clicked();

private:
    Ui::MainWindow *ui;
//    QImage *sourcePic;
//    QImage *resultPic;
};

#endif // MAINWINDOW_H
