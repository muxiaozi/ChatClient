#include "chatclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatClient chatWindow;
    chatWindow.show();
    return a.exec();
}
