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
	explicit ChatClient(QWidget *parent = 0);
	~ChatClient();

private slots:
	void on_listUser_itemDoubleClicked(QListWidgetItem *item);

    void disconnectFromServer();

    void on_btnVoice_pressed();

    void on_btnVoice_released();

    void on_btnText_clicked();

    void on_btnLogin_clicked();

    void receiveText(qintptr descriptor, const QString &text);

private:
    inline QString getTime();

	Ui::ChatClient *ui;
    QHash<QString, QString> onlineUsers; //在线用户表
    NetDelegate *netDelegate;

};

#endif // CHATCLIENT_H
