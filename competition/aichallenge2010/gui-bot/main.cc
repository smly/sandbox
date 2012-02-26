#include <iostream>
#include "Map.h"
#include "mainwindow.h"
#include "select.h"

int main (int argc, char *argv[])
{
  char name[] = "dev";
  argv[0] = name;
  QApplication app(argc, argv);
  std::string dir;
  Select * s = new Select();

  while (true) {
    Map map;
    MainWindow win;
    win.setSelect(s);
    win.show();
    app.exec();
    Map::MakeMove(s->getDir());
  }
}
