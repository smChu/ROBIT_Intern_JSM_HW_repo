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
#include "../include/ros_qt_pub/main_window.hpp"
#include <QTextEdit>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace ros_qt_pub
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
    //QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushButton()));
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
  }

  MainWindow::~MainWindow()
  {
  }
  void MainWindow::on_pushButton_clicked()
  {
    
    qnode.msgData = ui.textEdit->toPlainText().toStdString();
    qnode.publishMsg();
    ui.textEdit->clear();

   
  }

  /*****************************************************************************
  ** Functions
  *****************************************************************************/

}  // namespace s
