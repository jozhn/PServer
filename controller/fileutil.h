#include <QSqlQueryModel>
#include <QDateTime>
#include <QComboBox>
#include "entity/platerecord.h"

#ifndef FILEUTIL_H
#define FILEUTIL_H

class FileUtil
{
public:
    FileUtil();
    void addType(QComboBox *typebox);
    bool addSuccessRec(int file_id, QString plate_color, QString plate_num, int type, QString location,QDateTime datetime);
    bool addFailRec(int file_id, int type, QString location, QDateTime datetime);
    PlateRecord getPlateRecord(int fileId);
    bool deleteItem();
    bool updateItem(QString fileName);
    bool deleteAll();
    void setUnrecModel();
    void setSuccessModel();
    void setFailModel();
    QSqlQueryModel *getModel();
    QSqlQueryModel *model;
};

#endif // FILEUTIL_H
