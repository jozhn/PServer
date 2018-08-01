#include <QSqlQueryModel>

#ifndef FILEUTIL_H
#define FILEUTIL_H

class FileUtil
{
public:
    FileUtil();
    bool addSuccessRec(int file_id, QString plate_color, QString plate_num, QString type, QString location);
    bool addFailRec(int file_id, QString type, QString location);
    bool deleteItem();
    bool updateItem();
    bool deleteAll();
    void setUnrecModel();
    void setSuccessModel();
    void setFailModel();
    QSqlQueryModel *getModel();
    QSqlQueryModel *model;
};

#endif // FILEUTIL_H
