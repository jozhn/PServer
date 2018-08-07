#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "qss/qss.h"

namespace Ui {
class Login;
}

class Login : public QssDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    QString strMd5(QString str);
    void start();

private slots:
    void on_login_clicked();
    bool writeInit(QString path,QString user_key,QString user_value);
    bool readInit(QString path,QString user_key,QString &user_value);
    bool rememberPass();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
