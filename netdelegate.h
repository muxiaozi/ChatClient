#ifndef NETDELEGATE_H
#define NETDELEGATE_H

#include <QObject>
#include <QHash>

class QTcpSocket;

class NetDelegate : public QObject
{
    Q_OBJECT
public:
    enum DataType{
        CLIENT_CONNECTED = 1,
        CLIENT_DISCONNECTED = 2,
        RECEIVE_TEXT_SINGAL = 3,
        RECEIVE_TEXT_ALL = 4,
        RECEIVE_VOICE_SINGAL = 5,
        RECEIVE_VOICE_ALL = 6,
    };

public:
    static NetDelegate *getInstance(QObject *parent = 0);
    static void releaseInstance();

    bool connectToServer(const QString &ip, int port, const QString &name);
    void sendMyInfo(qintptr socketDescriptor, const QString &name);
    bool sendTextToOne(qintptr someone, const QString &text);
    bool sendTextToAll(const QString &text);

signals:
    //与服务器断开连接
    void disconnectFromServer();
    //用户连接
    void clientConnected(qintptr descriptor, const QString &name);
    //用户断开
    void clientDisconnected(qintptr descriptor);
    //收到某用户消息
    void receiveText(qintptr descriptor, const QString &text);
    //收到某用户语音信息
    void receiveVoice(qintptr descriptor, const char *data);

private slots:
    void readyRead();

private:
    explicit NetDelegate(QObject *parent = 0);
    ~NetDelegate();

private:
    static NetDelegate *m_instance;
    QTcpSocket *socket;
    QByteArray packetData;
    QString name;
    QHash<qintptr, QString> clientMap;

};

#endif // NETDELEGATE_H
