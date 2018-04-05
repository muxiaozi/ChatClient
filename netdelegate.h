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
        CONNECTED = 1,
        DISCONNECTED = 2,
        TEXT_TO_ONE = 3,
        TEXT_TO_ALL = 4,
        VOICE_TO_ONE = 5,
        VOICE_TO_ALL = 6,
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
    void onDisconnected();
    //用户连接
    void onClientConnected(qintptr user, const QString &name);
    //用户断开
    void onClientDisconnected(qintptr user);
    //收到某用户消息
    void onReceiveText(qintptr user, const QString &text, bool global);
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
