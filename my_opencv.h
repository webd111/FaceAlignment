#ifndef MY_OPENCV_H
#define MY_OPENCV_H

#include <QImage>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

extern QImage Mat2QImage(const cv::Mat& mat);
extern cv::Mat QImage2Mat(QImage image);

#endif // MY_OPENCV_H
