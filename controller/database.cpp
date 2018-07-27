#include "database.h"
#include "qdebug.h"

Database::Database()
{
}

bool Database::connectDB()
{
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        m_db = QSqlDatabase::database("qt_sql_default_connection");
        return true;
    }
    else{
        // 添加连接
        m_db = QSqlDatabase::addDatabase("QMYSQL" );
        // 连接设置
        m_db.setHostName("localhost");
        m_db.setDatabaseName("pr");
        m_db.setUserName("root");
        m_db.setPassword("357721012ZX");
        // 打开数据库
        if(!m_db.open())
        {
            qDebug()<<"db connection error";
            return false;
        }
        return true;
    }
}

QSqlDatabase Database::getDatabase()
{
    return m_db;
}
