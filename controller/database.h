#ifndef DATABASE_H
#define DATABASE_H

#include <qsqldatabase.h>

class Database
{
private:
    QSqlDatabase m_db;
public:
    Database();
    // 连接数据库
    bool connectDB();
    QSqlDatabase getDatabase();
};

#endif // DATABASE_H
