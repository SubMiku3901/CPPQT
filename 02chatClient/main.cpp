#include <QApplication>
#include <QTcpServer>
#include "login.h"
#include "regist.h"
#include "widget.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Login l;
  Regist r;
  Widget w;

  QObject::connect(&l, &Login::showRegist, [&]() {
    r.show();
    l.hide();
  });

  QObject::connect(&r, &Regist::showLogin, [&]() {
    l.show();
    r.hide();
  });

  QObject::connect(&l, &Login::showWidget, [&](QString userName) {
    w.setUserName(userName);
    w.show();
    l.hide();
  });

  l.show();
  return a.exec();
}
