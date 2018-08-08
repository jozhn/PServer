#include "login.h"
#include "ui_login.h"
#include "qcryptographichash.h"
#include "controller/database.h"
#include "qsqlquery.h"
#include "mainwindow.h"
#include "qdebug.h"

#include <QSettings>

Login::Login(QWidget *parent) :
    QssDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->usernameEdit->setFocus();
    setWindowIcon(QIcon(":/qss/app.png"));
    setWindowTitle("Server");

      QString username;
       QString password;
       QString rem_state;
       QString auto_state;
       bool is_un = readInit(QString("./user.ini"), "username", username);
       bool is_pass = readInit(QString("./user.ini"), "password", password);
       readInit(QString("./user.ini"), "remember_states", rem_state);
       readInit(QString("./user.ini"), "auto_states", auto_state);
       ui->usernameEdit->setText(username);
       ui->passwordEdit->setText(password);
       ui->checkBox->setChecked(rem_state=="1");
       ui->checkBox_2->setChecked(auto_state=="1");

}

Login::~Login()
{
    delete ui;
}

void Login::on_login_clicked()
{
    if(this->ui->usernameEdit->text().isEmpty() || this->ui->passwordEdit->text().isEmpty()){
        this->ui->warning->setText("请输入账号和密码");
    }
    else{
        QString username = this->ui->usernameEdit->text();//qDebug()<<username;
        QString password = strMd5(this->ui->passwordEdit->text());//qDebug()<<password;
        try{
            if(!connectDB()){
                throw QString("no connection");
                qDebug()<<"db error";
            }
            else{
                QSqlQuery query;
                QString sql = QString("select * from user where username='%1'").arg(username);
                query.exec(sql);
                while(query.next()){
                    if(query.value(2).toString() == password ){
                        this->ui->warning->setText("登陆成功");
                        rememberPass();
                        this->hide();
                        MainWindow *w = new MainWindow();
                        w->show();
                        break;
                    }
                }
                this->ui->warning->setText("请输入正确的账号密码");
            }
        }catch(QString ex){
            qDebug()<<ex;
            this->ui->warning->setText("无网络");
        }
    }
}

bool Login::writeInit(QString path, QString user_key, QString user_value)
{
    if(path.isEmpty() || user_key.isEmpty()){
           return false;
       }
       else{
           //创建文件
           QSettings * config =  new QSettings(path,QSettings::IniFormat);

           //写入
           config->beginGroup("config");
           config->setValue(user_key,user_value);
           config->endGroup();
           return true;
       }
}

bool Login::readInit(QString path, QString user_key, QString &user_value)
{
    user_value = QString("");
        if(path.isEmpty() || user_key.isEmpty()){
            return false;
        }
        else{
            //创建操作对象
            QSettings * config = new QSettings(path,QSettings::IniFormat);

            //读取
            user_value = config->value(QString("config/") + user_key).toString();
            return true;
        }
}

bool Login::rememberPass()
{
    if(ui->checkBox->isChecked()){
           QString username = ui->usernameEdit->text();  //账号
           QString password = ui->passwordEdit->text();  //密码

           bool is_address = writeInit(QString("./user.ini"), "username", username);
           bool is_port = writeInit(QString("./user.ini"), "password", password);

       }
       else{
           bool is_address = writeInit(QString("./user.ini"), "username", "");
           bool is_port = writeInit(QString("./user.ini"), "password", "");

       }

       writeInit(QString("./user.ini"), "remember_states", QString::number(ui->checkBox->isChecked()));
       writeInit(QString("./user.ini"), "auto_states", QString::number(ui->checkBox_2->isChecked()));
}

QString Login::strMd5(QString str)
{
    QByteArray byte = str.toLatin1();
    QByteArray byteMd5 = QCryptographicHash::hash(byte, QCryptographicHash::Md5);
    QString strMd5 = byteMd5.toHex();
    return strMd5;
}

void Login::start()
{
    if(ui->checkBox_2->isChecked()){
      on_login_clicked();
        this->hide();
    }
    else
    {
        this->show();
    }
}
