#include "regist.h"
#include "ui_regist.h"

Regist::Regist(QWidget* parent) : QWidget(parent), ui(new Ui::Regist) {
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  // 先直接连接到 127.0.0.1 8888
  socket->connectToHost("127.0.0.1", 8888);
  connect(socket, &QTcpSocket::connected, this, &Regist::on_connected);
  connect(socket, &QTcpSocket::readyRead, this, &Regist::on_readyRead);
}

Regist::~Regist() {
  delete ui;
}

void Regist::on_pushButton_2_clicked() {
  if (ui->lineEdit_2->text() != ui->lineEdit_3->text()) {
    QMessageBox::warning(this, "警告", "两次密码不一致");
    return;
  }
  // 获取用户名和密码
  QString userName = ui->lineEdit->text();
  QString password = ui->lineEdit_2->text();
  // 发送给服务器
  QString msg = "reg " + userName + " " + password;
  socket->write(msg.toLocal8Bit());
}

void Regist::on_pushButton_clicked() {  // 返回按键，忘记重命名了
  emit showLogin();
}

void Regist::on_connected() {
  qDebug() << "注册页面连接服务器成功";
}

void Regist::on_readyRead() {
  // 读取服务器发来的数据
  QByteArray data = socket->readAll();
  QString msg = QString::fromLocal8Bit(data);
  if (msg == "ok") {
    QMessageBox::information(this, "注册成功", "注册成功");
    emit showLogin();
  } else if (msg.startsWith("msg ")) {
    // 由于这个 socket 没有关闭，会一直接收到服务器发来的消息，所以这里直接返回
    // 最好还是找个办法在登录后关闭 socket 连接
    return;
  } else {
    QMessageBox::warning(this, "注册失败", "注册失败");
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
  }
}
