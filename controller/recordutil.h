#ifndef RECORDUTIL_H
#define RECORDUTIL_H

#include <QSqlQueryModel>
#include <QDateTime>

class RecordUtil
{
public:
    RecordUtil();
    bool submitRecord(QDateTime datetime);
    bool save(int fileId,QString plate_color,QString plate_num,int type,QString location);
};

#endif // RECORDUTIL_H
