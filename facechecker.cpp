#include "facechecker.h"

FaceChecker::FaceChecker()
{
    img_show_ = Mat::zeros(128*4, 128*5, CV_8UC1);
}

Mat FaceChecker::ShowSeletion(int x, int y)
{
    line(img_show_, Point(x*128, y*128), Point(x*128+127, y*128+127), Scalar(0,255,0), 4);
    line(img_show_, Point(x*128, y*128+127), Point(x*128+127, y*128), Scalar(0,255,0), 4);
    return img_show_;
}

Mat FaceChecker::ShowFaces(int idx_group, QString src_dir, QVector<std::string> names)
{
    if(names.size() > 20)
        return Mat();
    int r = 0;
    int c = 0;
    Rect rect = Rect(0,0,128,128);
    Mat img_temp;
    for(int i = 0; i < names.size(); i++)
    {
        r = i / 5;
        c = i % 5;
        rect.x = c * 128;
        rect.y = r * 128;
        img_temp = imread(src_dir.toStdString() + '/' + names[i], IMREAD_GRAYSCALE);
        if(img_temp.rows != 128 && img_temp.cols != 128)
        {
            img_temp = Mat::ones(128,128,CV_8UC1);
            img_temp = img_temp * 255;
        }
        putText(img_temp, QString::number(idx_group*20+i+1).toStdString(), Point(50, 110), 0, 0.5, Scalar(0,255,0));
        img_temp.copyTo(img_show_(rect));
    }
    // 剪裁参数
    double dist1 = 42;		// distance between the eyes to the upper boundary
    double dist2 = 36;		// distance between the left eye to the left boundary
    double dist3 = 56;		// distance between two eye centers

    for (int i = 0; i < 5; i++) {
        line(img_show_, Point(int(dist2)+128*i, 0), Point(int(dist2)+128*i, 128*4-1), Scalar(0,255,0), 1);
        line(img_show_, Point(int(dist2+dist3)+128*i, 0), Point(int(dist2+dist3)+128*i, 128*4-1), Scalar(0,255,0), 1);
    }
    for (int i = 0; i < 4; i++) {
        line(img_show_, Point(0, int(dist1)+128*i), Point(5*128-1, int(dist1)+128*i), Scalar(0,255,0), 1);
    }
    return img_show_;
}
