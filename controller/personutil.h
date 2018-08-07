#ifndef PERSONUTIL_H
#define PERSONUTIL_H
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "database.h"
class PersonUtil
{
public:
    PersonUtil();

    void setModel();
    QSqlQueryModel *getModel();
    bool searchItem(QString name, QString idcard, QString platenum);
    QSqlQueryModel *model;
    QSqlQuery query;
    bool personFine(int tyid,int typo);
    bool execSql(int tyid, int typo);

private slots:
    void exportExcel(QTableView *tableview);
};

#endif // PERSONUTIL_H
