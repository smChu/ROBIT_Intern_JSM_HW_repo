/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/qt_turtle_control/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace qt_turtle_control
{
  using namespace Qt;
  /*****************************************************************************
  ** Implementation [MainWindow]
  *****************************************************************************/

  MainWindow::MainWindow(int argc, char** argv, QWidget* parent) : QMainWindow(parent), qnode(argc, argv)
  {
    ui.setupUi(this);  // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

    setWindowIcon(QIcon(":/images/icon.png"));

    qnode.init();

    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));

    QObject::connect(ui.stop, SIGNAL(clicked()), this, SLOT(stop()));
    QObject::connect(ui.go, SIGNAL(clicked()), this, SLOT(go()));
    QObject::connect(ui.right, SIGNAL(clicked()), this, SLOT(right()));
    QObject::connect(ui.left, SIGNAL(clicked()), this, SLOT(left()));
    QObject::connect(ui.back, SIGNAL(clicked()), this, SLOT(back()));


  }

  MainWindow::~MainWindow()
  {
    
  }

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  switch(event->key())
  {
  case Qt::Key_W: go();  break;
  case Qt::Key_A: left(); break;
  case Qt::Key_D: right(); break;
  case Qt::Key_X: back(); break;
  case Qt::Key_S: stop(); break;
  }
}
  void MainWindow::stop()
  {
      qnode._x=0;
      qnode._z=0;
  }
  void MainWindow::go()
  {
      qnode._x=2;
      qnode._z=0;
  }
  void MainWindow::left()
  {
      qnode._x=0;
      qnode._z=2;
  }
  void MainWindow::right()
  {
      qnode._x=0;
      qnode._z=-2;
  }
  void MainWindow::back()
  {
      qnode._x=-2;
      qnode._z=0;
  }

  /*****************************************************************************
  ** Functions
  *****************************************************************************/

}  // namespace s
