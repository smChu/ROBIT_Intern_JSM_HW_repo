/**
 * @file /include/qt_turtle_control/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef qt_turtle_control_QNODE_HPP_
#define qt_turtle_control_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace qt_turtle_control
{
  /*****************************************************************************
  ** Class
  *****************************************************************************/

  class QNode : public QThread
  {
    Q_OBJECT
  public:
    QNode(int argc, char** argv);
    int _x = 0;
    int _z = 0;
    virtual ~QNode();
    bool init();
    void run();

  Q_SIGNALS:
    void rosShutdown();

  private:
    int init_argc;
    char** init_argv;
    ros::Publisher qt_turtle_control;
  };

}  // namespace s

#endif /* qt_turtle_control_QNODE_HPP_ */
