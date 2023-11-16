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
#include "../include/image_view_qt/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace image_view_qt
{
  using namespace std;
  using namespace Qt;

  /*****************************************************************************
  ** Implementation [MainWindow]
  *****************************************************************************/

  MainWindow::MainWindow(int argc, char **argv, QWidget *parent) : QMainWindow(parent), qnode(argc, argv)
  {
    ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    my_ip = QHostAddress("192.168.0.70");
    your_ip = QHostAddress("192.168.0.51");

    PORT = 8888;

    socket = new QUdpSocket(this);

    if (socket->bind(my_ip, PORT, QUdpSocket::ShareAddress))
    {
      connect(socket, SIGNAL(readyRead()), this,
              SLOT(sub_udp()));
    }

    setWindowIcon(QIcon(":/images/icon.png"));

    qnode.init();

    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
    QObject::connect(&qnode, SIGNAL(msgSignal()), this, SLOT(toUi()));
    // binary white
    connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_4, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_5, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_6, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    // binary orange
    connect(ui.horizontalSlider_7, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_8, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_9, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_10, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_11, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_12, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    // binary lime
    connect(ui.horizontalSlider_13, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_14, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_15, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_16, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_17, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui.horizontalSlider_18, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
  }

  MainWindow::~MainWindow()
  {
  }

  void MainWindow::toUi()
  {
    // ui로 띄우기
    img = QImage(qnode.cam_img->data, qnode.cam_img->cols, qnode.cam_img->rows, qnode.cam_img->step, QImage::Format_RGB888);
    img = img.scaled(img.width(), img.height());
    ui.label->setPixmap(QPixmap::fromImage(img));
    ui.label->show();
    qnode.isReceived = false;
    qnode.cam_img = nullptr;
  }

  void MainWindow::sub_udp()
  {
    // // HSV 값 동적 조절
    // cv::Scalar lowerBound_w(sliderValue[0], sliderValue[2], sliderValue[4]);
    // cv::Scalar upperBound_w(sliderValue[1] + 179, sliderValue[3] + 255, sliderValue[5] + 255);
    // cv::Scalar lowerBound_o(sliderValue[7], sliderValue[11], sliderValue[9]);
    // cv::Scalar upperBound_o(sliderValue[8] + 179, sliderValue[10] + 255, sliderValue[6] + 255);
    // cv::Scalar lowerBound_l(sliderValue[13], sliderValue[17], sliderValue[15]);
    // cv::Scalar upperBound_l(sliderValue[14] + 179, sliderValue[16] + 255, sliderValue[12] + 255);

    // HSV 값 정적 조절
    cv::Scalar lowerBound_w(0, 0, 203);
    cv::Scalar upperBound_w(179, 35, 255);
    cv::Scalar lowerBound_o(96, 47, 134);
    cv::Scalar upperBound_o(179, 255, 255);
    cv::Scalar lowerBound_l(0, 46, 119);
    cv::Scalar upperBound_l(96, 169, 255);

    // 이미지 수신
    img_img = a.MatImgRcv(img_img, PORT, my_ip, *socket);
    // cv_cam 이미지
    cv::resize(img_img, img_img, Size(271, 171), 0, 0, CV_INTER_LINEAR);

    // 관심영역 좌표
    int width = img_img.cols;
    int height = img_img.rows;
    int trap_height = 5;
    int trap_width = 5;
    cv::Point points[4];
    points[0] = cv::Point(15, 75);
    points[1] = cv::Point(200, 80);
    points[2] = cv::Point(200, 110);
    points[3] = cv::Point(15, 100);

    // 가우시안블러 처리
    GaussianBlur(img_img, img_blur, Size(5, 5), 0); // Size는 커널 크기로 블러 효과의 크기
    img_raw = img_blur.clone();

    // RGB 이미지를 HSV 이미지로 변환
    cv::cvtColor(img_img, img_hsv, cv::COLOR_RGB2HSV);

    // HSV 값 정적 조정~label_3에 띄우기
    cv::Scalar f_lowerBound(0, 0, 0);
    cv::Scalar f_upperBound(179, 255, 255);

    // inrange를 통한 이진화, lowerBound에서 upperBound 사이에 있는 값들은 밝게. 나머진 검게 이진화
    cv::inRange(img_hsv, f_lowerBound, f_upperBound, f_img_roi);
    cv::inRange(img_hsv, lowerBound_w, upperBound_w, img_binary);
    cv::inRange(img_hsv, lowerBound_o, upperBound_o, img_binary2);
    cv::inRange(img_hsv, lowerBound_l, upperBound_l, img_binary3);

    // 침식과 팽창
    erode(img_binary, img_mask, Mat::ones(Size(1, 1), CV_8UC1), Point(-1, -1), 2);
    dilate(img_binary, img_mask, Mat::ones(Size(1, 1), CV_8UC1), Point(-1, -1), 2);
    erode(img_binary2, img_mask2, Mat::ones(Size(1, 1), CV_8UC1), Point(-1, -1), 2);
    dilate(img_binary2, img_mask2, Mat::ones(Size(1, 1), CV_8UC1), Point(-1, -1), 3);
    erode(img_binary3, img_mask3, Mat::ones(Size(1, 1), CV_8UC1), Point(-1, -1), 2);
    dilate(img_binary3, img_mask3, Mat::ones(Size(1, 1), CV_8UC1), Point(-1, -1), 2);

    // 관심영역
    img_Roi = Roi(img_mask, points);
    f_img_roi = Roi(f_img_roi, points);

    // Canny
    f_img_roi = Roi(f_img_roi, points);
    Canny(img_mask, img_edge, 50, 200, 3);

    // houghLineP
    HoughLinesP(img_Roi, white_line, 1, (CV_PI / 180), 50, 20, 80);
    for (size_t i = 0; i < white_line.size(); i++)
    {
      Vec4i l = white_line[i];
      cv::line(img_blur, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 255), 2, 8);
      line_point[0] = l[0];
      line_point[1] = l[1];
      line_point[2] = l[2];
      line_point[3] = l[3];
    }

    // orange labeling
    int numLabel1 = connectedComponentsWithStats(img_mask2, img_labels1, stats1, centroids1, 8, CV_32S);

    for (int j = 1; j < numLabel1; j++)
    {
      area1 = stats1.at<int>(j, CC_STAT_AREA);
      left1 = stats1.at<int>(j, CC_STAT_LEFT);
      top1 = stats1.at<int>(j, CC_STAT_TOP);
      width1 = stats1.at<int>(j, CC_STAT_WIDTH);
      height1 = stats1.at<int>(j, CC_STAT_HEIGHT);

      rectangle(img_blur, cv::Point(left1, top1), Point(left1 + width1, top1 + height1), Scalar(0, 0, 255), 1);
      // 중앙 값: (left + width/2, top + height/2)

      if ((top1 + height1 <= line_point[1] && top1 + height1 <= line_point[3]) && (left1 >= line_point[0] && left1 + width1 <= line_point[2]))
      {
        ui.label_14->setText("above the line");
        // cout << left1 <<"//" <<top1 << "//"<<width1 << "//"<< height1 << endl;
        // cout <<  line_point[0] <<"//" << line_point[1] << "//"<<  line_point[2]<< "//"<< line_point[3] << endl;
      }
      else if ((left1 >= line_point[0] && left1 + width1 <= line_point[2]) && ((line_point[1] >= top1 + height1 / 2 && line_point[1] <= top1 + height1) && (line_point[3] >= top1 + height1 / 2 && line_point[3] <= top1 + height1)))
      {
        ui.label_14->setText("on the line");
      }
      else if ((top1 + height1 / 2 >= line_point[1] && top1 + height1 / 2 >= line_point[3]) && (left1 >= line_point[0] && left1 + width1 <= line_point[2]))
      {
        ui.label_14->setText("below the line");
      }
      else
      {
        ui.label_14->setText("doesn't exist");
      }

      if ((top2 + height2 <= line_point[1] && top1 + height2 <= line_point[3]) && (left2 >= line_point[0] && left2 + width2 <= line_point[2]))
      {
        ui.label_16->setText("above the line");
        // cout << left1 <<"//" <<top1 << "//"<<width1 << "//"<< height1 << endl;
        // cout <<  line_point[0] <<"//" << line_point[1] << "//"<<  line_point[2]<< "//"<< line_point[3] << endl;
      }
      else if ((left2 >= line_point[0] && left2 + width2 <= line_point[2]) && ((line_point[1] >= top2 + height2 / 2 && line_point[1] <= top2 + height2) && (line_point[3] >= top2 + height2 / 2 && line_point[3] <= top2 + height2)))
      {
        ui.label_16->setText("on the line");
      }
      else if ((top2 + height2 / 2 >= line_point[1] && top2 + height2 / 2 >= line_point[3]) && (left2 >= line_point[0] && left2 + width2 <= line_point[2]))
      {
        ui.label_16->setText("below the line");
      }
      else
      {
        ui.label_16->setText("doesn't exist");
      }
    }

    // lime labeling
    int numLabel2 = connectedComponentsWithStats(img_mask3, img_labels2, stats2, centroids2, 8, CV_32S);

    for (int j = 1; j < numLabel2; j++)
    {
      area2 = stats2.at<int>(j, CC_STAT_AREA);
      left2 = stats2.at<int>(j, CC_STAT_LEFT);
      top2 = stats2.at<int>(j, CC_STAT_TOP);
      width2 = stats2.at<int>(j, CC_STAT_WIDTH);
      height2 = stats2.at<int>(j, CC_STAT_HEIGHT);
      rectangle(img_blur, cv::Point(left2, top2), cv::Point(left2 + width2, top2 + height2), Scalar(255, 0, 0), 1);
      // 중앙 값: (left + width/2, top + height/2)
    }

    // QImage
    f_roiImg = QImage(f_img_roi.data, f_img_roi.cols, f_img_roi.rows, f_img_roi.step, QImage::Format_Grayscale8);
    maskImg = QImage(img_mask.data, img_mask.cols, img_mask.rows, img_mask.step, QImage::Format_Grayscale8);
    maskImg2 = QImage(img_mask2.data, img_mask2.cols, img_mask2.rows, img_mask2.step, QImage::Format_Grayscale8);
    maskImg3 = QImage(img_mask3.data, img_mask3.cols, img_mask3.rows, img_mask3.step, QImage::Format_Grayscale8);
    img = QImage(img_blur.data, img_blur.cols, img_blur.rows, img_blur.step, QImage::Format_RGB888);
    img = img.rgbSwapped();
    rawImg = QImage(img_raw.data, img_raw.cols, img_raw.rows, img_raw.step, QImage::Format_RGB888);
    rawImg = rawImg.rgbSwapped();
    roiImg = QImage(img_Roi.data, img_Roi.cols, img_Roi.rows, img_Roi.step, QImage::Format_Grayscale8);
    cannyImg = QImage(img_edge.data, img_edge.cols, img_edge.rows, img_edge.step, QImage::Format_Grayscale8);
    houghImg = QImage(img_hough.data, img_hough.cols, img_hough.rows, img_hough.step, QImage::Format_Grayscale8);

    // UI에 표시
    ui.label->setPixmap(QPixmap::fromImage(rawImg));     // usb cam
    ui.label_2->setPixmap(QPixmap::fromImage(img));      // find object
    ui.label_3->setPixmap(QPixmap::fromImage(f_roiImg)); // roi
    ui.label_4->setPixmap(QPixmap::fromImage(roiImg));   // binary white
    ui.label_5->setPixmap(QPixmap::fromImage(maskImg2)); // binary orange
    ui.label_6->setPixmap(QPixmap::fromImage(maskImg3)); // binary lime
  }

  void MainWindow::on_horizontalSlider_valueChanged(int value)
  {
    // 현재 어떤 트랙바의 값이 변경되었는지를 확인
    // 이를 위해 sender() 함수를 사용
    QSlider *changedSlider = qobject_cast<QSlider *>(sender());
    // binary white
    if (changedSlider == ui.horizontalSlider)
    {
      ui.horizontalSlider->setValue(value);
      sliderValue[0] = ui.horizontalSlider->value();
      ui.label_17->setText(QString::number(sliderValue[0]));
    }
    else if (changedSlider == ui.horizontalSlider_2)
    {
      ui.horizontalSlider_2->setValue(value);
      sliderValue[1] = ui.horizontalSlider_2->value();
      ui.label_18->setText(QString::number(sliderValue[1] + 179));
    }
    else if (changedSlider == ui.horizontalSlider_3)
    {
      ui.horizontalSlider_3->setValue(value);
      sliderValue[2] = ui.horizontalSlider_3->value();
      ui.label_19->setText(QString::number(sliderValue[2]));
    }
    else if (changedSlider == ui.horizontalSlider_4)
    {
      ui.horizontalSlider_4->setValue(value);
      sliderValue[3] = ui.horizontalSlider_4->value();
      ui.label_20->setText(QString::number(sliderValue[3] + 255));
    }
    else if (changedSlider == ui.horizontalSlider_5)
    {
      ui.horizontalSlider_5->setValue(value);
      sliderValue[4] = ui.horizontalSlider_5->value();
      ui.label_21->setText(QString::number(sliderValue[4]));
    }
    else if (changedSlider == ui.horizontalSlider_6)
    {
      ui.horizontalSlider_6->setValue(value);
      sliderValue[5] = ui.horizontalSlider_6->value();
      ui.label_22->setText(QString::number(sliderValue[5] + 255));
    }
    // binary orange
    else if (changedSlider == ui.horizontalSlider_7)
    {
      ui.horizontalSlider_7->setValue(value);
      sliderValue[6] = ui.horizontalSlider_7->value();
      ui.label_39->setText(QString::number(sliderValue[6] + 255));
    }
    else if (changedSlider == ui.horizontalSlider_8)
    {
      ui.horizontalSlider_8->setValue(value);
      sliderValue[7] = ui.horizontalSlider_8->value();
      ui.label_31->setText(QString::number(sliderValue[7]));
    }
    else if (changedSlider == ui.horizontalSlider_9)
    {
      ui.horizontalSlider_9->setValue(value);
      sliderValue[8] = ui.horizontalSlider_9->value();
      ui.label_34->setText(QString::number(sliderValue[8] + 179));
    }
    else if (changedSlider == ui.horizontalSlider_10)
    {
      ui.horizontalSlider_10->setValue(value);
      sliderValue[9] = ui.horizontalSlider_10->value();
      ui.label_37->setText(QString::number(sliderValue[9]));
    }
    else if (changedSlider == ui.horizontalSlider_11)
    {
      ui.horizontalSlider_11->setValue(value);
      sliderValue[10] = ui.horizontalSlider_11->value();
      ui.label_38->setText(QString::number(sliderValue[10] + 255));
    }
    else if (changedSlider == ui.horizontalSlider_12)
    {
      ui.horizontalSlider_12->setValue(value);
      sliderValue[11] = ui.horizontalSlider_12->value();
      ui.label_36->setText(QString::number(sliderValue[11]));
    }
    // binary lime
    else if (changedSlider == ui.horizontalSlider_13)
    {
      ui.horizontalSlider_13->setValue(value);
      sliderValue[12] = ui.horizontalSlider_13->value();
      ui.label_50->setText(QString::number(sliderValue[12] + 255));
    }
    else if (changedSlider == ui.horizontalSlider_14)
    {
      ui.horizontalSlider_14->setValue(value);
      sliderValue[13] = ui.horizontalSlider_14->value();
      ui.label_42->setText(QString::number(sliderValue[13]));
    }
    else if (changedSlider == ui.horizontalSlider_15)
    {
      ui.horizontalSlider_15->setValue(value);
      sliderValue[14] = ui.horizontalSlider_15->value();
      ui.label_45->setText(QString::number(sliderValue[14] + 179));
    }
    else if (changedSlider == ui.horizontalSlider_16)
    {
      ui.horizontalSlider_16->setValue(value);
      sliderValue[15] = ui.horizontalSlider_16->value();
      ui.label_48->setText(QString::number(sliderValue[15]));
    }
    else if (changedSlider == ui.horizontalSlider_17)
    {
      ui.horizontalSlider_17->setValue(value);
      sliderValue[16] = ui.horizontalSlider_17->value();
      ui.label_49->setText(QString::number(sliderValue[16] + 255));
    }
    else if (changedSlider == ui.horizontalSlider_18)
    {
      ui.horizontalSlider_18->setValue(value);
      sliderValue[17] = ui.horizontalSlider_18->value();
      ui.label_47->setText(QString::number(sliderValue[17]));
    }
  }

  cv::Mat MainWindow::Roi(Mat img, Point *points)
  {
    cv::Mat img_roi = Mat::zeros(img.rows, img.cols, CV_8UC1);
    const Point *ppt[1] = {points}; // position of points
    int npt[] = {4};                // the number of points

    cv::fillPoly(img_roi, ppt, npt, 1, Scalar(255, 255, 255), LINE_8); // draw trapeziod

    cv::Mat img_roied;
    bitwise_and(img, img_roi, img_roied); // img && img_roi ==> img_roied
    return img_roied;
  }
  /*****************************************************************************
  ** Functions
  *****************************************************************************/

} // namespace s
