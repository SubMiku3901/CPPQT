#ifndef WIDGET_H
#define WIDGET_H

#include <QMessageBox>
#include <QTcpSocket>  //客户端套接字类
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
  void setUserName(QString name);

 private slots:
  void on_connected();  // 处理connected信号的槽函数
  void on_readyRead();  // 处理readyRead信号的槽函数

  void on_sendBtn_clicked();
  void on_disconnected();  // 处理disconnected信号的槽函数

 private:
  Ui::Widget* ui;

  // 定义客户端指针
  QTcpSocket* socket;

  QString userName;
};
#endif  // WIDGET_H
