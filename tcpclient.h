#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <iostream>

#include <QWidget>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient;}
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void setUserName(QString s);
private slots:
    void on_pushButtonEnter_clicked();
    void slotconnectedsuccess();//用来处理连接成功的信号
    void slotreceive();//接收服务器传过来的信息
    void on_pushButtonSend_clicked();
    void slotdisconnected();//用来处理离开聊天室的信号

    void on_fontCbx_currentFontChanged(const QFont &f);
    void on_sizeCbx_currentIndexChanged(const QString &arg1);
    void on_boldTBtn_clicked(bool checked);
    void on_italicTBtn_clicked(bool checked);
    void on_underlineTBtn_clicked(bool checked);
    void on_colorTBtn_clicked();
    void curFmtChanged(const QTextCharFormat &fmt);
    void on_saveTBtn_clicked();
    void on_clearTBtn_clicked();
    bool saveFile(const QString& filename);

private:
    Ui::TcpClient *ui;    
    bool status;//用来判断是否进入了聊天室
    int port;
    QHostAddress *serverIP;
    QString userName;
    QTcpSocket *tcpsocket;
    QString name;
    QColor color;

};
#endif // TCPCLIENT_H
