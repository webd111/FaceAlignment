#ifndef FACECHECKER_H
#define FACECHECKER_H

#include <QVector>

#include <my_opencv.h>

using namespace cv;

class FaceChecker
{
    Mat img_show_;
public:
    FaceChecker();

    Mat ShowSeletion(int x, int y);
    Mat ShowFaces(int idx_group, QString src_dir, QVector<std::string> names);
};

#endif // FACECHECKER_H
