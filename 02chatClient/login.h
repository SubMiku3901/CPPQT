#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
  class Login;
}

class Login : public QWidget
{
  Q_OBJECT

public:
  explicit Login(QWidget *parent = nullptr);
  ~Login();

private slots:
  void on_loginBtn_clicked();
  void on_regBtn_clicked();
  void on_connected();
  void on_readyRead();

private:
  Ui::Login *ui;
  QTcpSocket* socket;

signals:
  void showRegist();
  void showWidget(QString userName);

};

#endif // LOGIN_H
