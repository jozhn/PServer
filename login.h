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

private slots:
    void on_login_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
