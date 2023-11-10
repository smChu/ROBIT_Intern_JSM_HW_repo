/**
 * @file /include/ros_qt_sub/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef ros_qt_sub_QNODE_HPP_
#define ros_qt_sub_QNODE_HPP_

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
#include <std_msgs/String.h>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace ros_qt_sub
{
  /*****************************************************************************
  ** Class
  *****************************************************************************/

  class QNode : public QThread
  {
    Q_OBJECT
  public:
    QNode(int argc, char** argv);
    virtual ~QNode();
    bool init();
    void run();

    bool isReceived = false;


  Q_SIGNALS:
    void rosShutdown();
    void msgSignal(std::string msg);

  private:
    int init_argc;
    char** init_argv;
    ros::Subscriber sub;
    void myCallBack(const std_msgs::String::ConstPtr& msg);
  };

}  // namespace s

#endif /* ros_qt_sub_QNODE_HPP_ */
