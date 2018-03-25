#include "netdelegate.h"

#include <QTcpSocket>
#include <QJsonObject>
#include <QStringList>
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
		socket->write(name.toUtf8());
		return true;
	}else{
		return false;
	}
}

bool NetDelegate::sendText(const QString &text)
{
	if(socket->isWritable()){
		socket->write(text.toUtf8());
	}
}

void NetDelegate::readyRead()
{
	while(socket->bytesAvailable() > 0){
		QByteArray data = socket->readLine();
		emit receiveText(socket->socketDescriptor(), QString::fromUtf8(data));
	}
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



