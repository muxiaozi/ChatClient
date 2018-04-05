#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

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
    connect(netDelegate, SIGNAL(onDisconnected()), SLOT(onDisconnected()));
    connect(netDelegate, SIGNAL(onClientConnected(qintptr,QString)), this, SLOT(onClientConnected(qintptr,QString)));
    connect(netDelegate, SIGNAL(onClientDisconnected(qintptr)), this, SLOT(onClientDisconnected(qintptr)));
    connect(netDelegate, SIGNAL(onReceiveText(qintptr,QString,bool)), this, SLOT(onReceiveText(qintptr,QString,bool)));
}

ChatClient::~ChatClient()
{
    delete ui;
    NetDelegate::releaseInstance();
}

void ChatClient::onDisconnected()
{
    ui->btnLogin->setVisible(true);
    ui->btnVoice->setVisible(false);
    ui->btnText->setVisible(false);
    ui->txtReceive->append("您与服务器断开连接！");
}

void ChatClient::on_listUser_itemDoubleClicked(QListWidgetItem *item)
{
    QString info = item->text();

}

void ChatClient::onClientConnected(qintptr user, const QString &name)
{
    QListWidgetItem *item = new QListWidgetItem(name);
    item->setData(Qt::UserRole, user);
    ui->listUser->addItem(item);
    ClientEntity entity;
    entity.name = name;
    entity.textHistory = QStringList("您可以与" + name + "开始聊天");
    clientMap.insert(user, entity);
    qDebug()<<user<<","<<name<<" Connected!";
}

void ChatClient::onClientDisconnected(qintptr user)
{
    clientMap.remove(user);
    for(int i = 0; i < ui->listUser->count(); ++i){
        if(ui->listUser->item(i)->data(Qt::UserRole) == user){
            ui->listUser->removeItemWidget(ui->listUser->item(i));
            break;
        }
    }
    qDebug()<<user<<" Disconnected!";
}

void ChatClient::onReceiveText(qintptr user, const QString &text, bool global)
{
    Q_UNUSED(global);

    qDebug()<<user<<": "<<text;
    //列表中没有此用户
    if(clientMap.find(user) == clientMap.end()){
        return;
    }
    ClientEntity &client = clientMap[user];
    client.textHistory.append(client.name + ": " + text);
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
        netDelegate->sendTextToAll(ui->txtSend->toPlainText());
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

void ChatClient::on_listUser_clicked(const QModelIndex &index)
{
    qDebug()<<index.row();
}
