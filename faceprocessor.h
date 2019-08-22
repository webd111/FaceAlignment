#ifndef FACEPROCESSOR_H
#define FACEPROCESSOR_H

#include <my_opencv.h>

using namespace cv;

class FaceProcessor
{
    Mat img_ori_;
    Mat img_aligned_;
    void AlignFace(Mat &img, Mat &faceAligned, Point left, Point right, Rect roi);
public:
    FaceProcessor();

    void set_img_ori(Mat img);
    Mat get_aligned_img();
    void AlignFaceWithEyeCood(QVector<cv::Point> eyes);
};

#endif // FACEPROCESSOR_H
