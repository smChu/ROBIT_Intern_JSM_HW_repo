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
#include "../include/ros_qt_sub/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace ros_qt_sub
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
    qRegisterMetaType<std::string>("std::string");
    QObject::connect(&qnode, SIGNAL(msgSignal(std::string)), this, SLOT(msgTolabel(std::string)));
  
  }

  MainWindow::~MainWindow()
  {
  }

  void MainWindow::msgTolabel(std::string msg)
  {
    //메시지를 받아서 라벨에 띄우기
    QString l_msg =  QString::fromStdString(msg);
    ui.label->setText(l_msg);
    qnode.isReceived = false;
    
  }
  /*****************************************************************************
  ** Functions
  *****************************************************************************/

}  // namespace s
