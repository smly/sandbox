#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  ui.setupUi(this);
  connect(ui.northButton, SIGNAL(clicked()), this, SLOT(pushNorth()));
  connect(ui.southbutton, SIGNAL(clicked()), this, SLOT(pushSouth()));
  connect(ui.eastButton, SIGNAL(clicked()), this, SLOT(pushEast()));
  connect(ui.westButton, SIGNAL(clicked()), this, SLOT(pushWest()));
  setWindowTitle(tr("control panel"));
  resize(230, 100);
  show();
}

void
MainWindow::setSelect(Select * s)
{
  s_ = s;
}

std::string
MainWindow::getDir()
{
  return s_->getDir();
}

void
MainWindow::pushNorth()
{
  s_->setDir("N");  qApp->quit();
}

void
MainWindow::pushSouth()
{
  s_->setDir("S");  qApp->quit();
}

void
MainWindow::pushEast()
{
  s_->setDir("E");  qApp->quit();
}

void
MainWindow::pushWest()
{
  s_->setDir("W");  qApp->quit();
}
