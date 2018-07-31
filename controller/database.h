#ifndef DATABASE_H
#define DATABASE_H

#include <qsqldatabase.h>
#include <QDebug>

static bool connectDB(){
    // 添加连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL" );
    // 连接设置
    db.setHostName("localhost");
    db.setDatabaseName("pr");
    db.setUserName("root");
    db.setPassword("357721012ZX");
    // 打开数据库
    if(!db.open())
    {
        qDebug()<<"db connection error";
        return false;
    }
    return true;
}

#endif // DATABASE_H
