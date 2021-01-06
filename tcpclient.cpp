#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QTextCharFormat>
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
void TcpClient::setUserName(QString s){
    ui->label->setText(s+",欢迎你");
    userName=s;
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
        QString msg = "3"+userName + ":Leave Chat Room";
//        if((length = tcpsocket->write((char*)msg.toUtf8().data(), msg.length())) != msg.length())
//        {
//            return;
//        }
        tcpsocket->write(msg.toUtf8().data());
        ui->tableWidget->clear();
        //for(int i=0;i<ui->tableWidget->rowCount()+1;i++)
            //ui->tableWidget->removeRow(0);
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
    QString msg= userName + ":Enter Chat Room";

//    if((length = tcpsocket->write((char*)msg.toUtf8().data(), msg.length())) != msg.length())
//    {
//        return;
//    }
    msg = "0"+msg;
    tcpsocket->write(msg.toUtf8().data());

}

//收到消息
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

    QByteArray sign = tcpsocket->read(1);
    qDebug()<<QString(sign);
    if(sign == "0"){
        qDebug()<<"fxxk";
        QString string = tcpsocket->readAll();
        QString msg = string.split("_")[0];
        qDebug()<<string;
        int sz = string.split("_").size();
        ui->textEdit->append(msg.toHtmlEscaped());
        for(int i=0;i<ui->tableWidget->rowCount();i++)
            ui->tableWidget->removeRow(0);
        //ui->tableWidget->clear();
        for(int i=1;i<sz;i++){
            QString name = string.split("_")[i];
            usrEnter(name);
        }
    }

    else if(sign=="3"){
        qDebug()<<"fxxk 2";
        QString string = tcpsocket->readAll();
        QString name = string.split(":")[0];
        qDebug()<<ui->tableWidget->rowCount();
        for(int i=0;i<ui->tableWidget->rowCount();i++){
            qDebug()<<ui->tableWidget->item(i,0)->text();
            if(ui->tableWidget->item(i,0)->text() == name)
            {
                ui->tableWidget->removeRow(i);
                break;
            }
        }
        ui->textEdit->append(string.toHtmlEscaped());
    }
    else{
        QByteArray array = tcpsocket->readAll();
        ui->textEdit->append(QString(array).toHtmlEscaped());
    }
}

void TcpClient::usrEnter(QString usrname)
{
    QTableWidgetItem *usr = new QTableWidgetItem(usrname);
    //QTableWidgetItem *ip = new QTableWidgetItem(ipaddr);
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0,usr);
    //ui->usrTblWidget->setItem(0,1,ip);
}

//发送消息
void TcpClient::on_pushButtonSend_clicked()
{
    if(ui->msgTxtEdit->toHtml() == "")
    {
        return;
    }
    QString msg = "1"+userName + ":" + ui->msgTxtEdit->toPlainText();
   // tcpsocket->write((char*)msg.toUtf8().data(), msg.length());
    tcpsocket->write(msg.toUtf8().data());
    ui->msgTxtEdit->clear();
}

void TcpClient::slotdisconnected()
{
    ui->pushButtonSend->setEnabled(false);
    ui->pushButtonEnter->setText("进入聊天室");
}

//样式设置
void TcpClient::on_fontCbx_currentFontChanged(const QFont &f)
{
    ui->msgTxtEdit->setCurrentFont(f);
    ui->msgTxtEdit->setFocus();
}

void TcpClient::on_sizeCbx_currentIndexChanged(const QString &arg1)
{
    ui->msgTxtEdit->setFontPointSize(arg1.toDouble());
    ui->msgTxtEdit->setFocus();
}

void TcpClient::on_boldTBtn_clicked(bool checked)
{
    if(checked)
        ui->msgTxtEdit->setFontWeight(QFont::Bold);
    else
        ui->msgTxtEdit->setFontWeight(QFont::Normal);
    ui->msgTxtEdit->setFocus();
}

void TcpClient::on_italicTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontItalic(checked);
    ui->msgTxtEdit->setFocus();
}

void TcpClient::on_underlineTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontUnderline(checked);
    ui->msgTxtEdit->setFocus();
}
void TcpClient::on_clearTBtn_clicked()
{
    ui->textEdit->clear();
}
void TcpClient::on_colorTBtn_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid()){
        ui->msgTxtEdit->setTextColor(color);
        ui->msgTxtEdit->setFocus();
    }
}

void TcpClient::curFmtChanged(const QTextCharFormat &fmt)
{
    ui->fontCbx->setCurrentFont(fmt.font());

    if (fmt.fontPointSize() < 8) {
        ui->sizeCbx->setCurrentIndex(4);
    } else {
        ui->sizeCbx->setCurrentIndex(ui->sizeCbx->findText(QString::number(fmt.fontPointSize())));
    }
    ui->boldTBtn->setChecked(fmt.font().bold());
    ui->italicTBtn->setChecked(fmt.font().italic());
    ui->underlineTBtn->setChecked(fmt.font().underline());
    color = fmt.foreground().color();
}
bool TcpClient::saveFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("保存文件"),tr("无法保存文件 %1:\n %2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();

    return true;
}

void TcpClient::on_saveTBtn_clicked()
{
    if (ui->textEdit->document()->isEmpty()) {
        QMessageBox::warning(0, tr("警告"), tr("聊天记录为空，无法保存！"), QMessageBox::Ok);
    } else {
        QString fname = QFileDialog::getSaveFileName(this,tr("保存聊天记录"), tr("聊天记录"), tr("文本(*.txt);;所有文件(*.*)"));
        if(!fname.isEmpty())
            saveFile(fname);
    }
}




void TcpClient::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    qDebug()<<"succ!";
    QString msg = "2"+ userName  + ":" + ui->msgTxtEdit->toPlainText()+"_"+item->text();
   // tcpsocket->write((char*)msg.toUtf8().data(), msg.length());
    tcpsocket->write(msg.toUtf8().data());
    ui->msgTxtEdit->clear();
}
