#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>
#include <QList>  //链表容器
#include <QMessageBox>
#include <QMessageBox>   //消息对话框
#include <QSqlDatabase>  //数据库头文件
#include <QSqlError>     //数据库错误头文件
#include <QSqlQuery>     //数据库查询头文件
#include <QSqlRecord>    //数据库记录的头文件
#include <QTcpServer>    //服务器头文件
#include <QTcpSocket>    //客户端头文件
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

 public:
  Widget(QWidget* parent = nullptr);
  ~Widget();

 private slots:
  void on_startBtn_clicked();
  void on_newConnect();  // 自定义的处理newConnection信号的槽函数
  void on_readyRead();   // 自定义处理readyRead信号对应的槽函数

 private:
  Ui::Widget* ui;
  // 定义服务器指针
  QTcpServer* server;
  // 定义客户端容器
  QList<QTcpSocket*> socketList;
  // 保存用户数据的数据库
  QSqlDatabase db;

  void handleMsg(QTcpSocket* socket, QString msg);
};
#endif  // WIDGET_H
