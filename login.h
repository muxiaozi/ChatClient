﻿#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}
class NetWork;

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGIN_H
