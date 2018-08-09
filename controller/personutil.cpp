#include "personutil.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QDebug>
#include <QAxObject>
#include <QStandardPaths>

PersonUtil::PersonUtil()
{
}
bool PersonUtil::searchItem(QString name, QString platenum, QString idcard)
{
    if(NULL!=name)
    {
        if(NULL!=idcard)
        {
            if(NULL!=platenum)
            {
                QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,f.location,t.id,t.type,f.points,f.fine,f.flag "
                                  "FROM person p,car c,type t,fine_record f "
                                  "where p.id=c.pid and c.plate_num=f.plate_num and f.type=t.id and f.pid=p.id "
                                  "and p.name like '%1%2%3' "
                                  "and p.id_card like '%4%5%6' "
                                  "and c.plate_num like '%7%8%9' ").arg("%").arg(name).arg("%").arg("%").arg(idcard).arg("%").arg("%").arg(platenum).arg("%");
                model->setQuery(q);
            }
            else
            {
                QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,f.location,t.id,t.type,f.points,f.fine,f.flag "
                                  "FROM person p,car c,type t,fine_record f "
                                  "where p.id=c.pid and c.plate_num=f.plate_num and f.type=t.id and f.pid=p.id "
                                  "and p.name like '%1%2%3' "
                                  "and p.id_card like '%4%5%6' ").arg("%").arg(name).arg("%").arg("%").arg(idcard).arg("%");
                model->setQuery(q);
            }
        }
        else
        {
            QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,f.location,t.id,t.type,f.points,f.fine,f.flag "
                              "FROM person p,car c,type t,fine_record f "
                              "where p.id=c.pid and c.plate_num=f.plate_num and f.type=t.id and f.pid=p.id "
                              "and p.name like '%1%2%3' "
                              "and c.plate_num like '%7%8%9' ").arg("%").arg(name).arg("%").arg("%").arg(platenum).arg("%");
            model->setQuery(q);
        }
    }
    else
    {
        QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,f.location,t.id,t.type,f.points,f.fine,f.flag "
                          "FROM person p,car c,type t,fine_record f "
                          "where p.id=c.pid and c.plate_num=f.plate_num and f.type=t.id and f.pid=p.id "
                          "and p.id_card like '%4%5%6' "
                          "and c.plate_num like '%7%8%9' ").arg("%").arg(idcard).arg("%").arg("%").arg(platenum).arg("%");
        model->setQuery(q);
    }
}

bool PersonUtil::personFine(int tyid,int typo)
{
    if(NULL!=tyid){
        return execSql(tyid,typo);
     }
    else
    {
        qDebug()<<"违规类型错误";
        return false;
    }
}

bool PersonUtil::execSql(int tyid,int typo)
{
    QString q=QString("update person p,car c,rec_record r,type t,fine_record f "
                      "set f.flag='已扣',p.points=p.points-'%1' "
                      "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and t.id='%3' ").arg(typo).arg(tyid);
    if(query.exec(q))
        return true;
}

void PersonUtil::setModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT p.id,p.name,p.id_card,c.plate_num,f.location,t.id,t.type,f.points,f.fine,f.flag "
                    "FROM person p,car c,type t,fine_record f "
                    "where p.id=c.pid and c.plate_num=f.plate_num and f.type=t.id and f.pid=p.id and f.flag='未扣' ");
}

QSqlQueryModel *PersonUtil::getModel()
{
     return model;
}
