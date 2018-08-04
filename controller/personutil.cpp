#include "personutil.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
PersonUtil::PersonUtil()
{
}
bool PersonUtil::searchItem(QString name, QString platenum, QString idcard)
{
//    if(NULL!=name){

//              if(idcard!=NULL){
//                  model->setQuery(QString("SELECT * FROM person "
//                                          "where name like '%1%2%3' "
//                                          "AND id_card like '%7%8%9' ").arg("%").arg(name).arg("%").arg("%").arg(idcard).arg("%"));
//              }
//              else{
//                  model->setQuery(QString("SELECT * FROM provider where name like '%").append(companyName).append("%' "));
//              }

//      }
//      else{
//          if(principalName!=NULL)
//              model->setQuery(QString("SELECT * FROM provider where principal_name like '%").append(principalName).append("%' "));
//      }
    qDebug()<<name+"1";
    qDebug()<<idcard+"2";
    qDebug()<<platenum+"3";
    if(NULL!=name)
    {
        if(NULL!=idcard)
        {
            if(NULL!=platenum)
            {
                QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.type,p.balance FROM person p,car c,rec_record r,type t "
                                  "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id "
                                  "and p.name like '%1%2%3' "
                                  "and p.id_card like '%4%5%6' "
                                  "and c.plate_num like '%7%8%9' ").arg("%").arg(name).arg("%").arg("%").arg(idcard).arg("%").arg("%").arg(platenum).arg("%");
                model->setQuery(q);
            }
            else
            {
                QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.type,p.balance FROM person p,car c,rec_record r,type t "
                                  "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id "
                                  "and p.name like '%1%2%3' "
                                  "and p.id_card like '%4%5%6' ").arg("%").arg(name).arg("%").arg("%").arg(idcard).arg("%");
                model->setQuery(q);
            }
        }
        else
        {
            QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.type,p.balance FROM person p,car c,rec_record r,type t "
                              "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id "
                              "and p.name like '%1%2%3' "
                              "and c.plate_num like '%7%8%9' ").arg("%").arg(name).arg("%").arg("%").arg(platenum).arg("%");
            model->setQuery(q);
        }
    }
    else
    {
        QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.type,p.balance FROM person p,car c,rec_record r,type t "
                          "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id "
                          "and p.id_card like '%4%5%6' "
                          "and c.plate_num like '%7%8%9' ").arg("%").arg(idcard).arg("%").arg("%").arg(platenum).arg("%");
        model->setQuery(q);
    }
}

bool PersonUtil::personFine(QString fineType)
{
    qDebug()<<fineType;
    if(NULL!=fineType){
         if(fineType=="未贴年检"||fineType=="副驾不系安全带")
         {
            execSql(50,fineType);
          }
         else if(fineType=="超载小于20%"||fineType=="行驶中拨打手机"||fineType=="不系安全带")
         {
             execSql(100,fineType);
         }
         else if(fineType=="超速小于50%"||fineType=="堵车时占用应急车道")
         {
             execSql(200,fineType);
         }
         else if(fineType=="副驾不满14周岁")
         {
             execSql(300,fineType);
         }
         else if(fineType=="倒车、逆行")
         {
             execSql(500,fineType);
         }
         else if(fineType=="超速高于50%")
         {
             execSql(1500,fineType);
         }
         else if(fineType=="正常通行")
         {
              execSql(0,fineType);
         }
     }
    else
    {
        qDebug()<<"违规类型错误";
    }
}

bool PersonUtil::execSql(int number, QString fineType)
{
    qDebug()<<QString::number(number);
    QString q=QString("update person p,car c,rec_record r,type t "
                      "set p.balance=p.balance-'%1' "
                      "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and t.type='%2' ").arg(number).arg(fineType);
    query.exec(q);
}

void PersonUtil::setModel()
{
    model = new QSqlQueryModel;
//    model->setQuery("SELECT s.id,s.filename,t.type,s.location,s.receive_state FROM send_record s,type t "
//                    "where s.type=t.id and s.rec_state='识别成功' and s.receive_flag=0");
    model->setQuery("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.type,p.balance FROM person p,car c,rec_record r,type t "
                    "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id");
}

QSqlQueryModel *PersonUtil::getModel()
{
     return model;
}
