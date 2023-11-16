/**
 * @file /include/image_view_qt/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef image_view_qt_QNODE_HPP_
#define image_view_qt_QNODE_HPP_

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
#include <sensor_msgs/Image.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>



/*****************************************************************************
** Namespaces
*****************************************************************************/
using namespace cv;
namespace image_view_qt
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
    cv::Mat* cam_img = nullptr;
    cv::Mat resize_img;

  Q_SIGNALS:
    void rosShutdown();
    void msgSignal();

  private:
    int init_argc;
    char** init_argv;
    ros::Subscriber sub;
    void myCallBack(const sensor_msgs::Image::ConstPtr& imageMsg);
  };

}  // namespace s

#endif /* image_view_qt_QNODE_HPP_ */
