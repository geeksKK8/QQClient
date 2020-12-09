#include "logger.h"
#include "ui_logger.h"
#include "tcpclient.h"
Logger::Logger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logger)
{
    ui->setupUi(this);

    connect(ui->logbutton,SIGNAL(clicked()),this,SLOT(showclient()));
}

Logger::~Logger()
{
    delete ui;
}

void Logger::showclient()
{
    QSqlQuery sql_query;
    int flag = 0;
    QString select_sql = "select name, password from user";
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString name = sql_query.value(0).toString();
            QString password = sql_query.value(1).toString();
            if(ui->username->text()== name &&ui->password->text()== password)
            {

                w.setUserName(ui->username->text());
                flag = 1;
                //QMessageBox::information(NULL,"提示","登录成功");
                //ui->label_3->setText("登录成功");
                w.show();
                this->hide();
            }
        }
        if(flag == 0)
            QMessageBox::information(NULL,"提示","登录失败");
    }

    /*
    if(ui->username->text()=="admin"&&ui->password->text()=="admin")
    {

        w.setUserName(ui->username->text());
        ui->label_3->setText("登录成功");
        w.show();
        this->hide();
    }
    else{
        ui->label_3->setText("登录失败");
    }
    */
}

void Logger::on_logupbutton_clicked()
{
    QSqlQuery sql_query;
    QString insert_sql = "insert into user values (NULL, ?, ?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(ui->username->text());
    sql_query.addBindValue(ui->password->text());

    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted success!";
    }


}
