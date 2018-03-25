#include "chatclient.h"
#include "ui_chatclient.h"
#include "netdelegate.h"
#include "login.h"

#include <QListWidgetItem>
#include <QDebug>
#include <QDateTime>
#include <QSound>

ChatClient::ChatClient(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ChatClient)
{
	ui->setupUi(this);
    ui->btnVoice->setVisible(false);
    ui->btnText->setVisible(false);

    netDelegate = NetDelegate::getInstance();
    connect(netDelegate, SIGNAL(disconnectFromServer()), SLOT(disconnectFromServer()));
    connect(netDelegate, SIGNAL(receiveText(qintptr,QString)), SLOT(receiveText(qintptr,QString)));
}

ChatClient::~ChatClient()
{
	delete ui;
    NetDelegate::releaseInstance();
}

void ChatClient::on_listUser_itemDoubleClicked(QListWidgetItem *item)
{
	QString info = item->text();

}

void ChatClient::disconnectFromServer()
{
    ui->btnLogin->setVisible(true);
    ui->btnVoice->setVisible(false);
    ui->btnText->setVisible(false);
    ui->txtReceive->append("您与服务器断开连接！");
}

QString ChatClient::getTime()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

void ChatClient::on_btnVoice_pressed()
{
    qDebug()<<"pressed";
}

void ChatClient::on_btnVoice_released()
{
    qDebug()<<"released";
}

void ChatClient::on_btnText_clicked()
{
    if(!ui->txtSend->toPlainText().isEmpty()){
        netDelegate->sendText(ui->txtSend->toPlainText());
        ui->txtSend->clear();
    }
}

void ChatClient::on_btnLogin_clicked()
{
    Login loginDialog(this);
    if(loginDialog.exec() == QDialog::Accepted){
        ui->btnLogin->setVisible(false);
        ui->btnText->setVisible(true);
        ui->btnVoice->setVisible(true);
        ui->txtReceive->append("您已连接到服务器！");
    }
}

void ChatClient::receiveText(qintptr descriptor, const QString &text)
{
    ui->txtReceive->append(text);
}
