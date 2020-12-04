#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
private slots:
    void on_pushButtonEnter_clicked();
    void slotconnectedsuccess();//用来处理连接成功的信号
    void slotreceive();//接收服务器传过来的信息
    void on_pushButtonSend_clicked();
    void slotdisconnected();//用来处理离开聊天室的信号

private:
    Ui::TcpClient *ui;    
    bool status;//用来判断是否进入了聊天室
    int port;
    QHostAddress *serverIP;
    QString userName;
    QTcpSocket *tcpsocket;
};
#endif // TCPCLIENT_H
