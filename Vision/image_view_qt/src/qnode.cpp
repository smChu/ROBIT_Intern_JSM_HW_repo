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
#include <sstream>
#include "../include/image_view_qt/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace image_view_qt
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
  ros::init(init_argc, init_argv, "image_view_qt");
  if (!ros::master::check())
  {
    return false;
  }
  ros::start();  // explicitly needed since our nodehandle is going out of scope.
  ros::NodeHandle n;
  sub = n.subscribe("/usb_cam/image_raw", 10, &QNode::myCallBack, this);
  // Add your ros communications here.
  start();
  return true;
}

void QNode::run()
{
  ros::Rate loop_rate(33);
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
  Q_EMIT rosShutdown();  // used to signal the gui for a shutdown (useful to roslaunch)
}

void QNode::myCallBack(const sensor_msgs::Image::ConstPtr& imageMsg)
{
  if (!isReceived && cam_img == NULL)
  {
    cam_img = new cv::Mat(cv_bridge::toCvCopy(imageMsg, sensor_msgs::image_encodings::RGB8)->image);
    if (cam_img != NULL)
    {
      resize_img = cam_img->clone();
      cv::resize(resize_img, resize_img, Size(resize_img.cols, resize_img.rows), 0, 0, CV_INTER_LINEAR);

      isReceived = true;
      Q_EMIT msgSignal();
    }
  }
  else
    return;
}

}  // namespace image_view_qt
