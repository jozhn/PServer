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
                QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.id,t.type,t.points,p.points,f.fine,f.flag FROM person p,car c,rec_record r,type t,fine_record f "
                                  "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and f.id=p.id "
                                  "and p.name like '%1%2%3' "
                                  "and p.id_card like '%4%5%6' "
                                  "and c.plate_num like '%7%8%9' ").arg("%").arg(name).arg("%").arg("%").arg(idcard).arg("%").arg("%").arg(platenum).arg("%");
                model->setQuery(q);
            }
            else
            {
                QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.id,t.type,t.points,p.points,f.fine,f.flag FROM person p,car c,rec_record r,type t,fine_record f "
                                  "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and f.id=p.id "
                                  "and p.name like '%1%2%3' "
                                  "and p.id_card like '%4%5%6' ").arg("%").arg(name).arg("%").arg("%").arg(idcard).arg("%");
                model->setQuery(q);
            }
        }
        else
        {
            QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.id,t.type,t.points,p.points,f.fine,f.flag FROM person p,car c,rec_record r,type t,fine_record f "
                              "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and f.id=p.id "
                              "and p.name like '%1%2%3' "
                              "and c.plate_num like '%7%8%9' ").arg("%").arg(name).arg("%").arg("%").arg(platenum).arg("%");
            model->setQuery(q);
        }
    }
    else
    {
        QString q=QString("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.id,t.type,t.points,p.points,f.fine,f.flag FROM person p,car c,rec_record r,type t,fine_record f "
                          "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and f.id=p.id "
                          "and p.id_card like '%4%5%6' "
                          "and c.plate_num like '%7%8%9' ").arg("%").arg(idcard).arg("%").arg("%").arg(platenum).arg("%");
        model->setQuery(q);
    }
}

bool PersonUtil::personFine(int tyid,int typo)
{
    qDebug()<<QString::number(tyid)+"id";
    qDebug()<<QString::number(typo)+"po";
    if(NULL!=tyid){
         if(tyid==1||tyid==2)
         {
            execSql(tyid,typo);
          }
         else if(tyid==3||tyid==4||tyid==5)
         {
             execSql(tyid,typo);
         }
         else if(tyid==6||tyid==7)
         {
             execSql(tyid,typo);
         }
         else if(tyid==8)
         {
             execSql(tyid,typo);
         }
         else if(tyid==9)
         {
             execSql(tyid,typo);
         }
         else if(tyid==10)
         {
             execSql(tyid,typo);
         }
         else if(tyid==11)
         {
              execSql(tyid,typo);
         }
     }
    else
    {
        qDebug()<<"违规类型错误";
    }
}

bool PersonUtil::execSql(int tyid,int typo)
{
    QString q=QString("update person p,car c,rec_record r,type t,fine_record f "
                      "set f.flag='已扣',p.points=p.points-'%1' "
                      "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and t.id='%3' ").arg(typo).arg(tyid);
    query.exec(q);
}

void PersonUtil::setModel()
{
    model = new QSqlQueryModel;
//    model->setQuery("SELECT s.id,s.filename,t.type,s.location,s.receive_state FROM send_record s,type t "
//                    "where s.type=t.id and s.rec_state='识别成功' and s.receive_flag=0");
    model->setQuery("SELECT p.id,p.name,p.id_card,c.plate_num,r.location,t.id,t.type,t.points,p.points,f.fine,f.flag FROM person p,car c,rec_record r,type t,fine_record f "
                    "where p.id=c.pid and c.plate_num=r.plate_num and r.type=t.id and f.id=p.id and f.flag='未扣' ");
}

QSqlQueryModel *PersonUtil::getModel()
{
     return model;
}

void PersonUtil::exportExcel(QTableView *tableview)
{
    QString filePath = QFileDialog::getSaveFileName(this, "文件保存", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    "Excel 文件(*.xls *.xlsx)");
    if(!filePath.isEmpty()){
        qDebug()<<"connect successfully";
        QAxObject *excel = new QAxObject(this);
        excel->setControl("Excel.Application");//连接excel控件
        excel->dynamicCall("SetVisible(bool Visible)", false);//不显示窗体
        excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭
        QAxObject *workbooks = excel->querySubObject("WorkBooks");
        workbooks->dynamicCall("Add");
        QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
        QAxObject *worksheets = workbook->querySubObject("Sheets");
        QAxObject *worksheet = worksheets->querySubObject("Item(int)", 1);
        int rowCount = tableview->model()->rowCount();
        int columnCount =  tableview->model()->columnCount();
        QAxObject *cell,*col;
        QString merge_cell=QString("A1:Z%1").arg(rowCount+2); // 设置单元格的属性
        QAxObject *merge_range = worksheet->querySubObject("Range(const QString&)", merge_cell);
        merge_range->setProperty("HorizontalAlignment", -4108); // 水平居中
        merge_range->setProperty("VerticalAlignment", -4108);  // 垂直居中
        merge_range->setProperty("NumberFormatLocal", "@");  // 设置为文本
        //列标题
        for(int i=0;i<columnCount;i++)
        {
            QString columnName;
            columnName.append(QChar(i  + 'A'));
            columnName.append(":");
            columnName.append(QChar(i + 'A'));
            col = worksheet->querySubObject("Columns(const QString&)", columnName);
            cell=worksheet->querySubObject("Cells(int,int)", 1, i+1);
            columnName=tableview->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
            cell->dynamicCall("SetValue(const QString&)", columnName);
        }
        for(int i = 0; i < rowCount; ++i){
            for(int j =0 ; j < columnCount; ++j){
                QModelIndex index = tableview->model()->index(i, j);
                QString strdata=tableview->model()->data(index).toString();
                worksheet->querySubObject("Cells(int,int)", i+2, j+1)->dynamicCall("SetValue(const QString&)", strdata);
            }
        }
        workbook->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(filePath));
        if(excel != NULL){
            excel->dynamicCall("Quit()");
            delete excel;
            excel = NULL;
        }
        QUIWidget::showMessageBoxInfo("导出数据成功");
    }
}
