#include <controller/fileutil.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <controller/database.h>

FileUtil::FileUtil()
{
}

void FileUtil::addType(QComboBox *typebox)
{
    QSqlQuery query;
    QString sql = QString("select * from type order by id ASC");
    query.exec(sql);
    int i=0,id;QString type;
    while(query.next()){
        id = query.value("id").toInt();
        type = query.value("type").toString();
        typebox->insertItem(i,type,QString::number(id));
        i++;
    }
}

bool FileUtil::addSuccessRec(int file_id, QString plate_color, QString plate_num, int type, QString location, QDateTime datetime)
{
    QSqlQuery query,update;
    query.prepare("insert into rec_record (file_id,plate_color,plate_num,type,location,datetime) "
                  "values "
                  "(:file_id,:plate_color,:plate_num,:type,:location,:datetime)");
    query.bindValue(":file_id",file_id);
    query.bindValue(":plate_color",plate_color);
    query.bindValue(":plate_num",plate_num);
    query.bindValue(":type",type);
    query.bindValue(":location",location);
    query.bindValue(":datetime",datetime);
    bool success = query.exec();
    if(success){
        update.exec(QString("update send_record set rec_state='识别成功' where id=%1").arg(file_id));
        return true;
    }
    else
        return false;
}

bool FileUtil::addFailRec(int file_id, int type, QString location, QDateTime datetime)
{
    QSqlQuery query,update;
    query.prepare("insert into rec_record (file_id,type,location,datetime) "
                  "values "
                  "(:file_id,:type,:location,:datetime)");
    query.bindValue(":file_id",file_id);
    query.bindValue(":type",type);
    query.bindValue(":location",location);
    query.bindValue(":datetime",datetime);
    bool success = query.exec();
    if(success){
        update.exec(QString("update send_record set rec_state='识别失败' where id=%1").arg(file_id));
        return true;
    }
    else
        return false;
}

PlateRecord FileUtil::getPlateRecord(int fileId)
{
    QSqlQuery query;
    QString sql = QString("select s.filename,r.plate_color,r.plate_num,r.type,t.points,t.fine,r.location "
                          "from rec_record r,type t,send_record s "
                          "where r.file_id=%1 and r.type=t.id and r.file_id=s.id").arg(fileId);
    query.exec(sql);
    PlateRecord pr;
    while(query.next()){
        pr.fileName = query.value("filename").toString();
        pr.plateColor = query.value("plate_color").toString();
        pr.plateNum = query.value("plate_num").toString();
        pr.type = query.value("type").toInt();
        pr.points = query.value("points").toInt();
        pr.fine = query.value("fine").toInt();
        pr.location = query.value("location").toString();
    }
    return pr;
}

bool FileUtil::deleteItem()
{

}

bool FileUtil::updateItem(QString fileName)
{
    QSqlQuery query;
    QString sql = QString("update send_record set receive_state='已接收' where filename='%1'").arg(fileName);
    bool success = query.exec(sql);
    if(success){
        return true;
    }
    else
        return false;
}

bool FileUtil::deleteAll()
{
    QSqlQuery query;
    QString sql = "update send_record set receive_flag=1 where receive_flag=0";
    bool success = query.exec(sql);
    if(success){
        return true;
    }
    else
        return false;
}

void FileUtil::setUnrecModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT s.id,s.filename,t.type,s.location,s.receive_state FROM send_record s,type t "
                    "where s.type=t.id and s.rec_state='待识别' and s.receive_flag=0");
}

void FileUtil::setSuccessModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT s.id,s.filename,t.type,r.location,s.receive_state FROM send_record s,type t,rec_record r "
                    "where r.type=t.id and s.rec_state='识别成功' and s.receive_flag=0 and r.flag=0 and s.id=r.file_id");
}

void FileUtil::setFailModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT s.id,s.filename,t.type,r.location,s.receive_state FROM send_record s,type t,rec_record r "
                    "where r.type=t.id and s.rec_state='识别失败' and s.receive_flag=0 and r.flag=0 and s.id=r.file_id");
}

QSqlQueryModel *FileUtil::getModel()
{
    return model;
}
