#include "recordutil.h"
#include <QSqlQuery>
#include <QDebug>

RecordUtil::RecordUtil()
{
}

bool RecordUtil::submitRecord(QDateTime datetime)
{
    QSqlQuery query,update;
    QString sql = QString("select c.pid,r.plate_num,r.type,t.points,t.fine,r.location "
                          "from rec_record r,type t,car c "
                          "where r.flag=0 and r.type=t.id and r.plate_num = c.plate_num");
    query.exec(sql);
    int pid,points,fine;
    QString type,location,plate_num;
    while(query.next()){
        pid = query.value("pid").toInt();
        points = query.value("points").toInt();
        fine = query.value("fine").toInt();
        type = query.value("type").toString();
        location = query.value("location").toString();
        plate_num = query.value("plate_num").toString();
        update.prepare("insert into fine_record (pid,plate_num,type,points,fine,location,datetime) "
                      "values "
                      "(:pid,:plate_num,:type,:points,:fine,:location,:datetime)");
        update.bindValue(":pid",pid);
        update.bindValue(":plate_num",plate_num);
        update.bindValue(":type",type);
        update.bindValue(":points",points);
        update.bindValue(":fine",fine);
        update.bindValue(":location",location);
        update.bindValue(":datetime",datetime);
        if(!update.exec())
            return false;
        update.exec("update rec_record set flag=1 where flag=0");
    }
    return true;

}

bool RecordUtil::save(int fileId, QString plate_color, QString plate_num, int type, QString location)
{
        QSqlQuery query;
        query.prepare("update rec_record set plate_color=:plate_color,plate_num=:plate_num,type=:type,location=:location "
                              "where file_id = :file_id");
        query.bindValue(":plate_color",plate_color);
        query.bindValue(":plate_num",plate_num);
        query.bindValue(":type",type);
        query.bindValue(":location",location);
        query.bindValue(":file_id",fileId);
        bool s = query.exec();
        if(s){
            qDebug()<<"success";
            return true;
        }
        else{
            qDebug()<<"error";
            return false;
        }
}
