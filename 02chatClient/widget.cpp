#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  socket->connectToHost("127.0.0.1", 8888);
  connect(socket, &QTcpSocket::connected, this, &Widget::on_connected);
  connect(socket, &QTcpSocket::readyRead, this, &Widget::on_readyRead);
  connect(socket, &QTcpSocket::disconnected, this, &Widget::on_disconnected);
}

Widget::~Widget() {
  delete ui;
}

void Widget::on_connected() {
  qDebug() << "聊天页面连接服务器成功";
}

void Widget::on_readyRead() {
  QByteArray msg = socket->readAll();
  QString msgStr = QString::fromLocal8Bit(msg);
  QString username = msgStr.section(' ', 1, 1);
  QString data = msgStr.section(' ', 2);
  ui->msgWidget->addItem(username + ": " + data);
}

void Widget::on_sendBtn_clicked() {
  QString ui_data = ui->msgEdit->text();

  QString msg = "msg " + userName + " " + ui_data;

  ui->msgWidget->addItem(userName + ": " + ui_data);
  // 将消息发送给服务器
  socket->write(msg.toLocal8Bit());

  // 清空编辑的框内容
  ui->msgEdit->clear();
}

// 关于disconnected信号对应槽函数的实现
void Widget::on_disconnected() {
  QMessageBox::information(this, "", "断开成功");
}

void Widget::setUserName(QString name) {
  userName = name;
  ui->userNameEdit->setText(name);
  ui->userNameEdit->setEnabled(false);
}
