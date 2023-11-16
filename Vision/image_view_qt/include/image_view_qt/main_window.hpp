/**
 * @file /include/image_view_qt/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date November 2010
 **/
#ifndef image_view_qt_MAIN_WINDOW_H
#define image_view_qt_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include <QHostAddress>
#include <QUdpSocket>
#include "udp.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <vector>

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace image_view_qt
{
  /*****************************************************************************
  ** Interface [MainWindow]
  *****************************************************************************/
  /**
   * @brief Qt central, all operations relating to the view part here.
   */
  class MainWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    MainWindow(int argc, char** argv, QWidget* parent = 0);
    ~MainWindow();

  public Q_SLOTS:
    void toUi();
    void sub_udp();
    void on_horizontalSlider_valueChanged(int value);



  private:
    Ui::MainWindowDesign ui;
    QHostAddress my_ip;
    QHostAddress your_ip;
    uint16_t PORT;
    QUdpSocket* socket;
    udp::UDP a;

    QImage rawImg;
    QImage img;
    QImage cannyImg;
    QImage maskImg;
    QImage maskImg2;
    QImage maskImg3;
    QImage roiImg;
    QImage f_roiImg;
    QImage houghImg;

    cv::Mat img_img;
    cv::Mat img_raw;
    cv::Mat img_blur;
    cv::Mat img_hsv;
    cv::Mat img_binary;
    cv::Mat img_binary2;
    cv::Mat img_binary3;
    cv::Mat img_mask;
    cv::Mat img_mask2;
    cv::Mat img_mask3;
    cv::Mat img_edge;
    cv::Mat img_Roi;
    cv::Mat f_img_roi;
    cv::Mat img_hough;
    cv::Mat img_labels1, stats1, centroids1;
    cv::Mat img_labels2, stats2, centroids2;

    std::vector<Vec4i> white_line;
    

    cv::Mat Roi(Mat img, Point * points);

    Vec4i l;
    int sliderValue[19] = {0, };
    int area1, left1, top1, width1, height1, area2, left2, top2, width2, height2; 
    int line_point[4];
    

    QNode qnode;
  };

}  // namespace s

#endif  // image_view_qt_MAIN_WINDOW_H
