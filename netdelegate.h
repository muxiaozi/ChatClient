#ifndef NETDELEGATE_H
#define NETDELEGATE_H

#include <QObject>

class QTcpSocket;

class NetDelegate : public QObject
{
    Q_OBJECT
    enum DataType{
        CLIENT_CONNECTED = 0,       // id, name     [quint64, utf8]
        CLIENT_DISCONNECTED = 1,    // id           [quint64]
        RECEIVE_TEXT_SINGAL = 2,    // id, text     [quint64, utf8]
        RECEIVE_TEXT_ALL = 3,       // id, text     [quint64, utf8]
        RECEIVE_VOICE_SINGAL = 4,   // id, voice    [quint64, quint32, blob]
        RECEIVE_VOICE_ALL = 5,      // id, voice    [quint64, quint32, blob]
    };

public:
    static NetDelegate *getInstance(QObject *parent = 0);
    static void releaseInstance();

    bool connectToServer(const QString &ip, int port, const QString &name);

    bool sendText(const QString &text);

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

    static NetDelegate *m_instance;

    QTcpSocket *socket;
    QString name;

};

#endif // NETDELEGATE_H
