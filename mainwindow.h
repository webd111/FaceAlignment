#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPixmap>
#include <QMainWindow>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QFile>

#include <my_opencv.h>

#include <faceprocessor.h>
#include <facechecker.h>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Mat img_ori_;
    QImage qimg_ori_;
    Mat img_show_;
    QImage qimg_show_;
    Mat img_aligned_;
    int num_pts_drawn_ = 0;
    QVector<cv::Point> pts_draw_;
    QVector<cv::Point> pts_align_;

    QString dir_src_;
    QString dir_save_;
    int idx_current_file_ = 0;
    QFileInfoList list_files_;

    QImage qimg_show_check_;

    QString dir_src_group_;
    QString dir_ori_group_;
    QString dir_dst_group_;
    int idx_current_group_ = 0;
    QFileInfoList list_files_group_;

    FaceProcessor face_processor_;
    FaceChecker face_checker_;

    void DrawPoint2Image();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent* ev);
    void mousePressEvent(QMouseEvent * ev);
    void mouseMoveEvent(QMouseEvent * ev);
    void wheelEvent(QWheelEvent * ev);

private slots:
    void on_action_finish_one_image_triggered();
    void on_action_next_image_triggered();
    void on_pushButton_dir_src_clicked();
    void on_pushButton_dir_save_clicked();
    void on_action_previous_image_triggered();
    void on_action_start_labeling_triggered();
    void on_action_clear_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_dir_src_group_clicked();
    void on_pushButton_dir_ori_group_clicked();
    void on_pushButton_dir_dst_group_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
