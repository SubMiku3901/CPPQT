#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  // 初始化服务器
  server = new QTcpServer(this);
  // 初始化数据库
  if (!db.contains("user.db")) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
  }
  // 打开数据库
  if (!db.open()) {
    QMessageBox::critical(this, "错误", db.lastError().text());
    return;
  }
  // 创建数据表
  QString sql =
      "create table if not exists user("
      "id integer primary key autoincrement,"
      "username varchar(255) not null,"
      "password varchar(255) not null)";
  QSqlQuery query;
  // 执行该 sql
  if (!query.exec(sql)) {
    QMessageBox::critical(this, "错误", query.lastError().text());
    return;
  }
}

Widget::~Widget() {
  delete ui;
}

void Widget::on_startBtn_clicked() {
  quint16 port = ui->portEdit->text().toUInt();  // 固定了 8888

  if (server->listen(QHostAddress::Any, port)) {
    ui->msgWidget->addItem("服务器启动成功");
    ui->startBtn->setEnabled(false);
  }
  connect(server, &QTcpServer::newConnection, this, &Widget::on_newConnect);
}

void Widget::on_newConnect() {
  QTcpSocket* s = server->nextPendingConnection();
  socketList.push_back(s);
  connect(s, &QTcpSocket::readyRead, this, &Widget::on_readyRead);
}

void Widget::on_readyRead() {
  // 移除断开连接的客户端
  for (int i = 0; i < socketList.size(); i++) {
    if (socketList[i]->state() == 0) {
      socketList.removeAt(i);
    }
  }

  for (int i = 0; i < socketList.size(); i++) {
    if (socketList[i]->bytesAvailable() != 0) {
      QByteArray msg = socketList[i]->readAll();
      QString str = QString::fromLocal8Bit(msg);
      handleMsg(socketList[i], str);
    }
  }
}

void Widget::handleMsg(QTcpSocket* socket, QString msg) {
  if (msg.startsWith("reg ")) {
    // 注册
    // 在数据表中查找该用户名
    QString username = msg.split(" ")[1];
    QString sql =
        QString("select * from user where username = '%1'").arg(username);
    QSqlQuery query;
    query.exec(sql);
    if (!query.next()) {
      // 该用户名不存在，可以注册
      socket->write("ok");
      QString password = msg.split(" ")[2];
      sql = QString("insert into user(username, password) values('%1', '%2')")
                .arg(username)
                .arg(password);
      query.exec(sql);
      ui->msgWidget->addItem(QString("%1 注册成功").arg(username));
    } else {
      // 该用户名已存在，不可以注册
      socket->write("err 已注册");
      ui->msgWidget->addItem(QString("%1 已经注册").arg(username));
    }
  } else if (msg.startsWith("login ")) {
    // 登录
    QString username = msg.split(" ")[1];
    QString password = msg.split(" ")[2];
    QString sql =
        QString("select * from user where username = '%1' and password = '%2'")
            .arg(username)
            .arg(password);
    QSqlQuery query;
    query.exec(sql);
    if (query.next()) {
      // 登录成功
      socket->write("ok");
      ui->msgWidget->addItem(QString("%1 登录成功").arg(username));
    } else {
      // 登录失败
      socket->write("err 用户名或密码错误");
      ui->msgWidget->addItem(QString("%1 登录失败").arg(username));
    }
  } else if (msg.startsWith("msg ")) {
    QString username = msg.split(" ")[1];
    QString mainMsg = msg.split(" ")[2];
    ui->msgWidget->addItem(QString("%1: %2").arg(username).arg(mainMsg));
    // 转发消息
    for (int j = 0; j < socketList.size(); j++) {
      if (this->socketList[j] != socket)
        this->socketList[j]->write(msg.toLocal8Bit());
    }
  }
}
