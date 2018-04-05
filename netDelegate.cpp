#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "netdelegate.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>

NetDelegate *NetDelegate::m_instance = nullptr;

void NetDelegate::releaseInstance()
{
    if(m_instance != nullptr){
        delete m_instance;
        m_instance = nullptr;
    }
}

bool NetDelegate::connectToServer(const QString &ip, int port, const QString &name)
{
    this->name = name;
    socket->connectToHost(QHostAddress(ip), port);
    if(socket->waitForConnected()){
        sendMyInfo(socket->socketDescriptor(), name);
        return true;
    }else{
        return false;
    }
}

void NetDelegate::sendMyInfo(qintptr socketDescriptor, const QString &name)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream<<(int)0;
    stream<<(int)CONNECTED;
    stream<<socketDescriptor;
    stream<<name.toUtf8();
    stream.device()->seek(0);
    stream<<data.size();
    socket->write(data);
}

bool NetDelegate::sendTextToOne(qintptr someone, const QString &text)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream<<(int)0;
    stream<<(int)TEXT_TO_ONE;
    stream<<someone;
    stream<<text.toUtf8();
    stream.device()->seek(0);
    stream<<data.size();

    socket->write(data);
    return true;
}

bool NetDelegate::sendTextToAll(const QString &text)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream<<(int)0;
    stream<<(int)TEXT_TO_ALL;
    stream<<socket->socketDescriptor();
    stream<<text.toUtf8();
    stream.device()->seek(0);
    stream<<data.size();

    socket->write(data);
    return true;
}

void NetDelegate::readyRead()
{
    int size;           //大小
    int type;           //类型
    qintptr sender;   //接收/发送方
    QByteArray tmpData; //数据包内容

    packetData.append(socket->readAll());
    QDataStream stream(&packetData, QIODevice::ReadWrite);

    while(packetData.size() > sizeof(int)){
        stream>>size;
        if(packetData.size() < size) break;
        stream>>type;
        stream>>sender;

        qDebug()<<"sender: "<<sender<<", type: "<< type;

        switch(type){
        case CONNECTED: //用户信息
        {
            stream>>tmpData;
            name = QString::fromUtf8(tmpData);
            emit onClientConnected(sender, name);
            break;
        }
        case TEXT_TO_ONE:
        {
            stream>>tmpData;
            emit onReceiveText(sender, QString::fromUtf8(tmpData), false);
            break;
        }
        case VOICE_TO_ONE:
        {
            break;
        }
        case TEXT_TO_ALL:
        {
            stream>>tmpData;
            emit onReceiveText(sender, QString::fromUtf8(tmpData), true);
            break;
        }
        case VOICE_TO_ALL:
        {
            break;
        }
        case DISCONNECTED:
        {
            emit onClientDisconnected(sender);
            break;
        }
        }

        //截取未读数据块
        packetData = packetData.right(packetData.size() - size);
    }
}

NetDelegate::NetDelegate(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(disconnected()), SIGNAL(onDisconnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
}

NetDelegate::~NetDelegate()
{
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    socket->disconnectFromHost();
}

NetDelegate *NetDelegate::getInstance(QObject *parent)
{
    if(m_instance == nullptr){
        m_instance = new NetDelegate(parent);
    }
    return m_instance;
}



