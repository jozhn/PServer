#include <controller/fileutil.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <controller/database.h>

FileUtil::FileUtil()
{
}

bool FileUtil::deleteItem()
{

}

void FileUtil::setModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM send_record where flag=0");
}

QSqlQueryModel *FileUtil::getModel()
{
    return model;
}
