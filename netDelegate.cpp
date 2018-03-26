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
    stream<<(int)CLIENT_CONNECTED;
    stream<<socketDescriptor;
    stream<<name.toUtf8();
    stream.device()->seek(0);
    stream<<data.size();
    socket->write(data);
}

bool NetDelegate::sendText(const QString &text)
{
	if(socket->isWritable()){
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream<<(int)0;
        stream<<(int)RECEIVE_TEXT_SINGAL;
        stream<<(qintptr)socket->socketDescriptor();
        stream<<(qintptr)200;
        stream<<text.toUtf8();
        stream.device()->seek(0);
        stream<<data.size();
        socket->write(data);
	}
    return true;
}

void NetDelegate::readyRead()
{
    int size;
    int type;
    qintptr sender;

    packetData.append(socket->readAll());
    QDataStream stream(&packetData, QIODevice::ReadOnly);

    stream>>size;
    if(packetData.size() < size)
        return;
    stream>>type;
    stream>>sender;

    switch(type){
        case CLIENT_CONNECTED: //用户信息
        {
            QByteArray byteName;
            stream>>byteName;
            //TODO ...
            clientDescriptor = sender;
            name = QString::fromUtf8(byteName);
            emit onClientConnected(sender, name);
            break;
        }
        case RECEIVE_TEXT_SINGAL:
        case RECEIVE_VOICE_SINGAL:
        {
            qintptr receiver;
            stream>>receiver;
            QByteArray byteText;
            stream>>byteText;
            qDebug()<<QString::fromUtf8(byteText);
            emit sendToOne(receiver, packetData.left(size));
            break;
        }
        case CLIENT_DISCONNECTED:
        case RECEIVE_TEXT_ALL:
        case RECEIVE_VOICE_ALL:
        {
            emit sendExceptOne(sender, packetData.left(size));
            break;
        }
    }

    packetData = packetData.right(packetData.size() - size);
}

NetDelegate::NetDelegate(QObject *parent) :
	QObject(parent)
{
	socket = new QTcpSocket();
	connect(socket, SIGNAL(disconnected()), SIGNAL(disconnectFromServer()));
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



