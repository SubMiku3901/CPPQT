#include "login.h"
#include "ui_login.h"

Login::Login(QWidget* parent) : QWidget(parent), ui(new Ui::Login) {
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  // 先直接连接到 127.0.0.1 8888
  socket->connectToHost("127.0.0.1", 8888);
  connect(socket, &QTcpSocket::connected, this, &Login::on_connected);
  connect(socket, &QTcpSocket::readyRead, this, &Login::on_readyRead);
}

Login::~Login() {
  delete ui;
}

void Login::on_loginBtn_clicked() {
  QString username = ui->userEdit->text();
  QString password = ui->pwdEdit->text();
  if (username == "" || password == "") {
    QMessageBox::warning(this, "警告", "用户名或密码不能为空！");
    return;
  }
  QString msg = "login " + username + " " + password;
  socket->write(msg.toLocal8Bit());
}

void Login::on_regBtn_clicked() {
  emit showRegist();
}

void Login::on_connected() {
  qDebug() << "登录页面连接服务器成功";
}

void Login::on_readyRead() {
  // 读取服务器发来的数据
  QByteArray data = socket->readAll();
  QString msg = QString::fromLocal8Bit(data);
  if (msg == "ok") {
    QMessageBox::information(this, "登录成功", "登录成功");
    socket->disconnect();
    emit showWidget(ui->userEdit->text());
  } else {
    QMessageBox::warning(this, "登录失败", "登录失败");
    ui->pwdEdit->clear();
  }
}
