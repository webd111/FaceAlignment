#include "faceprocessor.h"

FaceProcessor::FaceProcessor()
{

}


void FaceProcessor::set_img_ori(Mat img)
{
    if(img.channels())
        cvtColor(img, img_ori_, COLOR_BGR2GRAY);
    else {
        img.copyTo(img_ori_);
    }
    return;
}

Mat FaceProcessor::get_aligned_img()
{
    return img_aligned_;
}

void FaceProcessor::AlignFaceWithEyeCood(QVector<cv::Point> eyes)
{
    Point eye_left = eyes[0];
    Point eye_right = eyes[1];
    AlignFace(img_ori_, img_aligned_, eye_left, eye_right, Rect(Point(0, 0), Point(img_ori_.cols - 1, img_ori_.rows - 1)));
    return;
}

void FaceProcessor::AlignFace(Mat &img, Mat &faceAligned, Point left, Point right, Rect roi)
{
    //int offsetx = roi.x;
    //int offsety = roi.y;

    // 剪裁参数
    double dist1 = 42;		// distance between the eyes to the upper boundary
    double dist2 = 36;		// distance between the left eye to the left boundary
    double dist3 = 56;		// distance between two eye centers

    // 计算中心位置
    int cx = roi.width / 2 + roi.x;
    int cy = roi.height / 2 + roi.y;

    // 计算角度
    int dx = right.x - left.x;
    int dy = right.y - left.y;
    double degree = 180 * ((atan2(dy, dx)) / CV_PI);
    double scale = dist3 / (right.x - left.x);

    //int left_x = cvRound((left.x - cx) * cos(-degree * CV_PI / 180.0) - (left.y - cy) * sin(-degree * CV_PI / 180.0) + cx);
    //int left_y = cvRound((left.x - cx) * sin(-degree * CV_PI / 180.0) + (left.y - cy) * cos(-degree * CV_PI / 180.0) + cy);

    // 旋转矩阵计算
    Mat M = getRotationMatrix2D(Point2f(cx, cy), degree, scale);
    //Point2f center(cx, cy);
    //Rect bbox = RotatedRect(center, img.size(), degree).boundingRect();
    //M.at<double>(0, 2) += (bbox.width / 2.0 - center.x);
    //M.at<double>(1, 2) += (bbox.height / 2.0 - center.y);
    Point2f left_mapped = Point2f(0, 0);
    left_mapped.x = M.ptr<double>(0)[0] * left.x + M.ptr<double>(0)[1] * left.y + M.ptr<double>(0)[2];
    left_mapped.y = M.ptr<double>(1)[0] * left.x + M.ptr<double>(1)[1] * left.y + M.ptr<double>(1)[2];

    // 对齐
    Mat result;
    //warpAffine(img, result, M, bbox.size());
    warpAffine(img, result, M, Size(left_mapped.x + 128, left_mapped.y + 128), INTER_LINEAR, BORDER_CONSTANT, 0);
//    imshow("1", result);
//    waitKey(1);
//    warpAffine(img, result, M, Size(left_mapped.x + 150, left_mapped.y + 150), INTER_LINEAR, BORDER_CONSTANT, 0);
//    imshow("2", result);
//    waitKey(1);

    // 裁剪
    if(left_mapped.x - dist2 >= 0 &&
           left_mapped.y - dist1 >= 0 &&
            left_mapped.x - dist2 + 128 < result.cols &&
            left_mapped.y - dist1 + 128 < result.rows)
        result(Rect(int(left_mapped.x - dist2), int(left_mapped.y) - dist1, 128, 128)).copyTo(faceAligned);
    else {
        qDebug() << "rect error." ;
    }
}
