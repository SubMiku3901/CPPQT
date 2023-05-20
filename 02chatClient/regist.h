#ifndef REGIST_H
#define REGIST_H

#include <QMessageBox>
#include <QTcpSocket>
#include <QWidget>

namespace Ui {
class Regist;
}

class Regist : public QWidget {
  Q_OBJECT

 public:
  explicit Regist(QWidget* parent = nullptr);
  ~Regist();

 private slots:
  void on_pushButton_2_clicked();
  void on_pushButton_clicked();
  void on_connected();
  void on_readyRead();

 private:
  Ui::Regist* ui;
  QTcpSocket* socket;

 signals:
  void showLogin();
};

#endif  // REGIST_H
