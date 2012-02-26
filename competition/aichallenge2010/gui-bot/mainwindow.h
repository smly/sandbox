#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "button.h"
#include "select.h"
#include <string>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  void setSelect(Select *sl);
  std::string getDir();

private slots:
  void pushNorth();
  void pushSouth();
  void pushEast();
  void pushWest();

private:
  Ui::MainWindow ui;
  Select * s_;
};

#endif
