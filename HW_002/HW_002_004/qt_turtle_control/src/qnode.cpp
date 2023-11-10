/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include "../include/qt_turtle_control/qnode.hpp"


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace qt_turtle_control
{
  /*****************************************************************************
  ** Implementation
  *****************************************************************************/

  QNode::QNode(int argc, char** argv) : init_argc(argc), init_argv(argv)
  {
  }

  QNode::~QNode()
  {
    if (ros::isStarted())
    {
      ros::shutdown();  // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    wait();
  }

  bool QNode::init()
  {
    ros::init(init_argc, init_argv, "qt_turtle_control");
    if (!ros::master::check())
    {
      return false;
    }
    ros::start();  // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;
    // Add your ros communications here. ok
    //퍼블리셔 선언
    qt_turtle_control = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",50);//토픽명을 같게 해주므로써 통신
    start();
    return true;
  }

  void QNode::run()
  {
    ros::Rate loop_rate(33);
    while (ros::ok())
    {

      geometry_msgs::Twist qt_turtle;//메시지형 변수 선언

      qt_turtle.linear.x= _x;//
      qt_turtle.angular.z= _z;
      qt_turtle_control.publish(qt_turtle);//퍼블리셔 이름.publish(메시지형)
      ros::spinOnce();
      loop_rate.sleep();
    }
    std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown();  // used to signal the gui for a shutdown (useful to roslaunch)
  }

}  // namespace s
