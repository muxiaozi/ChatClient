#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>
#include <QHash>

namespace Ui {
	class ChatClient;
}

class QListWidgetItem;
class NetDelegate;

class ChatClient : public QMainWindow
{
	Q_OBJECT
public:
    struct ClientEntity{
        QString name;
        QStringList textHistory;
    };

public:
	explicit ChatClient(QWidget *parent = 0);
	~ChatClient();

private slots:
	void on_listUser_itemDoubleClicked(QListWidgetItem *item);

    void disconnectFromServer();

    void onClientConnected(qintptr user, const QString &name);
    void onClientDisconnected(qintptr user);
    void onReceiveText(qintptr user, const QString &text, bool global);

    void on_btnVoice_pressed();

    void on_btnVoice_released();

    void on_btnText_clicked();

    void on_btnLogin_clicked();

    void receiveText(qintptr descriptor, const QString &text);

    void on_listUser_clicked(const QModelIndex &index);

private:
    inline QString getTime();

	Ui::ChatClient *ui;
    QHash<qintptr, ClientEntity> clientMap; //用户消息
    QStringList globalMessage; //全局消息
    NetDelegate *netDelegate;

};

#endif // CHATCLIENT_H
