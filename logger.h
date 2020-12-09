#ifndef LOGGER_H
#define LOGGER_H
#include "tcpclient.h"
#include <QWidget>
#include <QToolBox>
#include <QToolButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
namespace Ui {
class Logger;
}

class Logger : public QWidget
{
    Q_OBJECT

public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger();
    TcpClient w;
private slots:

    void showclient();

    void on_logupbutton_clicked();

private:
    Ui::Logger *ui;
    QToolButton *logbutton;
};

#endif // LOGGER_H
