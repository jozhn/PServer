#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QssDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/qss/app.png"));
    setWindowTitle("Server");
}

Login::~Login()
{
    delete ui;
}
