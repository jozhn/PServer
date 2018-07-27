#include "mainwindow.h"
#include "login.h"
#include <QApplication>

#include <QFile>
#include <QTextStream>

void initQss()
{
    QFile file(":/qss/css/qss.css");
    if (!file.open(QIODevice::ReadOnly))
        exit(0);

    QTextStream in(&file);
    QString css = in.readAll();
    qApp->setStyleSheet(css);
    qApp->setFont(QFont("微软雅黑", 9));

    return;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initQss();//step 2

    Login l;
    l.show();

    return a.exec();
}
