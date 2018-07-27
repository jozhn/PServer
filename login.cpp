#include "login.h"
#include "ui_login.h"
#include "qcryptographichash.h"
#include "controller/database.h"
#include "qsqlquery.h"
#include "mainwindow.h"
#include "qdebug.h"

Login::Login(QWidget *parent) :
    QssDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->usernameEdit->setFocus();
    setWindowIcon(QIcon(":/qss/app.png"));
    setWindowTitle("Server");
}

Login::~Login()
{
    delete ui;
}

void Login::on_login_clicked()
{
    QString username = this->ui->usernameEdit->text();qDebug()<<username;
    QString password = strMd5(this->ui->passwordEdit->text());qDebug()<<password;
    try{
        Database *d = new Database;
        if(!d->connectDB()){
            throw QString("no connection");
            qDebug()<<"db error";
        }
        else{
            QSqlQuery query(d->getDatabase());

            QString sql = QString("select * from user where username='%1'").arg(username);
            query.exec(sql);
            while(query.next()){
                if(query.value(2).toString() == password ){
                    this->ui->warning->setText("登陆成功");
                    this->hide();
                    MainWindow *w = new MainWindow();
                    w->show();
                    break;
                }else
                    this->ui->warning->setText("密码错误");
            }
        }
    }catch(QString ex){
        qDebug()<<ex;
        this->ui->warning->setText("无网络");
    }
}

QString Login::strMd5(QString str)
{
    QByteArray byte = str.toLatin1();
    QByteArray byteMd5 = QCryptographicHash::hash(byte, QCryptographicHash::Md5);
    QString strMd5 = byteMd5.toHex();
    return strMd5;
}
