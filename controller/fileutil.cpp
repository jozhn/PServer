#include <controller/fileutil.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <controller/database.h>

FileUtil::FileUtil()
{
}

bool FileUtil::addSuccessRec(int file_id,QString plate_color,QString plate_num,QString type,QString location)
{
    QSqlQuery query,update;
    query.prepare("insert into rec_record (file_id,plate_color,plate_num,type,location) "
                  "values "
                  "(:file_id,:plate_color,:plate_num,:type,:location)");
    query.bindValue(":file_id",file_id);
    query.bindValue(":plate_color",plate_color);
    query.bindValue(":plate_num",plate_num);
    query.bindValue(":type",type);
    query.bindValue(":location",location);
    bool success = query.exec();
    if(success){
        update.exec(QString("update send_record set rec_state='识别成功' where id=%1").arg(file_id));
        return true;
    }
    else
        return false;
}

bool FileUtil::addFailRec(int file_id,QString type,QString location)
{
    QSqlQuery query,update;
    query.prepare("insert into rec_record (file_id,type,location) "
                  "values "
                  "(:file_id,:type,:location)");
    query.bindValue(":file_id",file_id);
    query.bindValue(":type",type);
    query.bindValue(":location",location);
    bool success = query.exec();
    if(success){
        update.exec(QString("update send_record set rec_state='识别失败' where id=%1").arg(file_id));
        return true;
    }
    else
        return false;
}

bool FileUtil::deleteItem()
{

}

bool FileUtil::updateItem()
{

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
    model->setQuery("SELECT id,filename,type,location,receive_state FROM send_record where rec_state='待识别'");
}

void FileUtil::setSuccessModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT id,filename,type,location,receive_state FROM send_record where rec_state='识别成功' and receive_flag=0");
//    model->setQuery("SELECT id,plate_color,plate_num,type,location FROM rec_record where rec_state='识别成功'");
}

void FileUtil::setFailModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT id,filename,type,location,receive_state FROM send_record where rec_state='识别失败'");
//    model->setQuery("SELECT id,plate_color,plate_num,type,location FROM rec_record where rec_state='识别失败'");
}

QSqlQueryModel *FileUtil::getModel()
{
    return model;
}
