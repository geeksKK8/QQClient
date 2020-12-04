#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QHostAddress>
#include <QMessageBox>
TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    status = false;

    serverIP = new QHostAddress();

    //未进入聊天室内不能发送信息
    ui->pushButtonSend->setEnabled(false);
}

TcpClient::~TcpClient()
{
    delete ui;
}


//进入聊天室
void TcpClient::on_pushButtonEnter_clicked()
{
    //首先判断这个用户是不是在聊天室中
    if(status == false)
    {
        //不在聊天室中就和服务器进行连接
        QString ip = ui->lineEditServerIP->text();//从界面获取ip地址
        port = ui->lineEdit_4->text().toInt();
        if(!serverIP->setAddress(ip))//用这个函数判断IP地址是否可以被正确解析
        {
            //不能被正确解析就弹出一个警告窗口
            QMessageBox::warning(this, "错误", "IP地址不正确");
            return;
        }
        if(ui->lineEditUserName->text() == "")
        {
            //用户名不能为空
            QMessageBox::warning(this, "错误", "用户名不能为空");
            return;
        }

        //从界面获取用户名
        userName = ui->lineEditUserName->text();
        //创建一个通信套接字，用来和服务器进行通信
        tcpsocket = new QTcpSocket(this);
        //和服务器连接成功能会触发connected信号
        connect(tcpsocket, &QTcpSocket::connected, this, &TcpClient::slotconnectedsuccess);
        //接收到服务器的信息就会触发readyRead信号
        connect(tcpsocket, &QTcpSocket::readyRead, this, &TcpClient::slotreceive);

        //和服务器进行连接
        qDebug()<<*serverIP;
        qDebug()<<port;
        tcpsocket->connectToHost(*serverIP, port);
        if(!tcpsocket->waitForConnected(30000))
        {
            QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
            return;
        }



        //将进入聊天室的标志位置为true；
        status = true;
    }
    else//已经进入了聊天室
    {
        //int length = 0;
        QString msg = userName + ":Leave Chat Room";
//        if((length = tcpsocket->write((char*)msg.toUtf8().data(), msg.length())) != msg.length())
//        {
//            return;
//        }
        tcpsocket->write(msg.toUtf8().data());
        tcpsocket->disconnectFromHost();
        status = false;
        //离开聊天室就会触发disconnected信号
        connect(tcpsocket, &QTcpSocket::disconnected, this, &TcpClient::slotdisconnected);
    }
}

//用来处理连接成功的信号
void TcpClient::slotconnectedsuccess()
{
    //进入聊天室可以发送信息了
    ui->pushButtonSend->setEnabled(true);
    //将进入聊天的按钮改为离开聊天室
    ui->pushButtonEnter->setText("离开聊天室");

    //int length = 0;
    //将用户名发送给服务器
    QString msg= userName + " :Enter Chat Room";

//    if((length = tcpsocket->write((char*)msg.toUtf8().data(), msg.length())) != msg.length())
//    {
//        return;
//    }
    tcpsocket->write(msg.toUtf8().data());
}


void TcpClient::slotreceive()
{
//    while(tcpsocket->bytesAvailable() > 0 )
//    {
//        QByteArray datagram;
//        datagram.resize(tcpsocket->bytesAvailable());
//        tcpsocket->read(datagram.data(), datagram.size());
//        QString msg = datagram.data();
//        ui->textEdit->append(msg.left(datagram.size()));
//    }
    QByteArray array = tcpsocket->readAll();
    ui->textEdit->append(array);
}

void TcpClient::on_pushButtonSend_clicked()
{
    if(ui->lineEditSend->text() == "")
    {
        return;
    }
    QString msg = userName + ":" + ui->lineEditSend->text();
   // tcpsocket->write((char*)msg.toUtf8().data(), msg.length());
    tcpsocket->write(msg.toUtf8().data());
    ui->lineEditSend->clear();
}

void TcpClient::slotdisconnected()
{
    ui->pushButtonSend->setEnabled(false);
    ui->pushButtonEnter->setText("进入聊天室");
}
