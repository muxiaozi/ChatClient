#include "login.h"
#include "ui_logindialog.h"
#include "netdelegate.h"
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>

Login::Login(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
}

Login::~Login()
{
	delete ui;
}

void Login::on_btnLogin_clicked()
{
    if(ui->txtIP->text().isEmpty()){
        QMessageBox::information(this, "提示", "IP不能为空！");
        return;
    }

    if(ui->txtPort->text().isEmpty()){
        QMessageBox::information(this, "提示", "端口不能为空！");
        return;
    }

    //连接到服务器
    QString ip = ui->txtIP->text();
    int port = ui->txtPort->text().toInt();

    NetDelegate *delegate = NetDelegate::getInstance();
    if(delegate->connectToServer(ip, port, ui->txtName->text())){
        accept();
    }else{
        QMessageBox::information(this, "连接服务器失败", "请检查IP和端口是否正确，以及网络是否连通！");
    }
}
