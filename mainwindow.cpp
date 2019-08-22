#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    img_ori_ = imread("D:/LFWp/original/pairs1/00001.jpg");
    //    //    cvtColor(img_ori_, img_ori_, COLOR_BGR2RGB);

    //    qimg_ori_ = Mat2QImage(img_ori_);
    //    //    qimg_ori_ = QImage((const unsigned char*)(img_ori_.data),
    //    //                   img_ori_.cols, img_ori_.rows, img_ori_.step, QImage::Format_RGB888);
    //    img_ori_.copyTo(img_show_);
    //    qimg_show_ = Mat2QImage(img_show_);
    //    face_processor_.set_img_ori(img_ori_);
    //    ui->label_ori_img->setScaledContents(true);
    //    ui->label_ori_img->resize(ui->label_ori_img->width(), ui->label_ori_img->height());
    //    ui->label_ori_img->setPixmap(QPixmap::fromImage(qimg_ori_));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent * ev)
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        int x = ev->pos().x() - ui->centralWidget->x() - ui->tabWidget->x() - ui->tab_face_alignment->x() - ui->label_ori_img->x();
        int y = ev->pos().y() - ui->centralWidget->y() - ui->tabWidget->y() - ui->tab_face_alignment->y() - ui->label_ori_img->y();
        x = x * img_ori_.cols / ui->label_ori_img->width()/* - 1*/;
        y = y * img_ori_.rows / ui->label_ori_img->height()/* - 1*/;
        if( x > 0 && y > 0 &&
                x < img_ori_.cols && y < img_ori_.rows)
        {
            cv::Point pt = cv::Point(x,y);
            pts_draw_.push_back(pt);
            pts_align_.push_back(pt);
            update();
        }
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        int x = ev->pos().x() - ui->centralWidget->x() - ui->tabWidget->x() - ui->tab_face_check->x() - ui->label_img_group->x();
        int y = ev->pos().y() - ui->centralWidget->y() - ui->tabWidget->y() - ui->tab_face_check->y() - ui->label_img_group->y();
        x = x * 5 / ui->label_img_group->width()/* - 1*/;
        y = y * 4 / ui->label_img_group->height()/* - 1*/;
        qDebug() << "x,y" << x << y;
        if( x >= 0 && y >= 0 &&
                x < 5 && y < 4)
        {
            QString name = list_files_group_[idx_current_group_*20+y*5+x].fileName();
            qDebug() << "Copying name " << name;
            QFile::copy(QString(dir_ori_group_+"/"+name), QString(dir_dst_group_+"/"+name));
            qimg_show_check_ = Mat2QImage(face_checker_.ShowSeletion(x,y));
            ui->label_img_group->setScaledContents(true);
            ui->label_img_group->resize(ui->label_img_group->width(), ui->label_img_group->height());
            ui->label_img_group->setPixmap(QPixmap::fromImage(qimg_show_check_));
            ui->label_img_group->show();
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        int x = ev->pos().x() - ui->centralWidget->x() - ui->tabWidget->x() - ui->tab_face_alignment->x() - ui->label_ori_img->x();
        int y = ev->pos().y() - ui->centralWidget->y() - ui->tabWidget->y() - ui->tab_face_alignment->y() - ui->label_ori_img->y();
        x = x * img_ori_.cols / ui->label_ori_img->width()/* - 1*/;
        y = y * img_ori_.rows / ui->label_ori_img->height()/* - 1*/;
        if( x > 0 && y > 0 &&
                x < img_ori_.cols && y < img_ori_.rows)
        {
            ui->statusBar->showMessage("coodinates: " + QString::number(x) + "," + QString::number(y));
            repaint();
        }
    }
    return;
}

void MainWindow::wheelEvent(QWheelEvent* ev)
{
    if(ev->delta() > 0)
    {
        on_action_previous_image_triggered();
    }
    else
    {
        on_action_next_image_triggered();
    }
}

void MainWindow::paintEvent(QPaintEvent* ev)
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        Q_UNUSED(ev)
        DrawPoint2Image();
    }
}

void MainWindow::DrawPoint2Image()
{
    if(img_ori_.rows != 0)
    {
        while(!pts_draw_.isEmpty())
        {
            num_pts_drawn_ ++;
            circle(img_show_, pts_draw_.last(), 1, cv::Scalar(0, 255, 0), 1);
            putText(img_show_, QString::number(num_pts_drawn_).toStdString(), cv::Point(pts_draw_.last().x + 10, pts_draw_.last().y - 10),
                    0, 0.5, cv::Scalar(0, 255, 0));
            pts_draw_.pop_back();
        }
        qimg_show_ = Mat2QImage(img_show_);
        ui->label_ori_img->setPixmap(QPixmap::fromImage(qimg_show_));
        ui->label_ori_img->setScaledContents(true);
        ui->label_ori_img->resize(ui->label_ori_img->width(), ui->label_ori_img->height());
        ui->label_ori_img->show();
    }
}

void MainWindow::on_action_finish_one_image_triggered()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        if(pts_align_.size() < 2)
        {
            ui->statusBar->showMessage("No enough points");
            return;
        }
        else {
            face_processor_.AlignFaceWithEyeCood(pts_align_.mid(0,2));
            img_aligned_ = face_processor_.get_aligned_img();
            pts_align_.clear();
            ui->label_ali_img->setScaledContents(true);
            ui->label_ali_img->resize(ui->label_ali_img->width(), ui->label_ali_img->height());
            ui->label_ali_img->setPixmap(QPixmap::fromImage(Mat2QImage(img_aligned_)));
            if(!dir_save_.isEmpty())
                imwrite(QString(dir_save_ + "/" + list_files_.at(idx_current_file_).fileName()).toStdString(), img_aligned_);
            else {
                QMessageBox::information(nullptr, "Warning", "Please select the saving path first!");
            }
        }
    }
}

void MainWindow::on_action_next_image_triggered()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        on_action_finish_one_image_triggered();
        if(idx_current_file_+1 < list_files_.size())
        {
            idx_current_file_++;
            img_ori_ = imread(QString(dir_src_ + QString("/") + list_files_.at(idx_current_file_).fileName()).toStdString(), IMREAD_COLOR);
            qimg_ori_ = Mat2QImage(img_ori_);
            img_ori_.copyTo(img_show_);
            qimg_show_ = Mat2QImage(img_show_);
            //        img_aligned_;
            num_pts_drawn_ = 0;
            pts_draw_.clear();
            pts_align_.clear();
            face_processor_.set_img_ori(img_ori_);
            ui->label_ori_img->setScaledContents(true);
            ui->label_ori_img->resize(ui->label_ori_img->width(), ui->label_ori_img->height());
            ui->label_ori_img->setPixmap(QPixmap::fromImage(qimg_ori_));
        }
        else {
            QMessageBox::information(nullptr, "Information", "This is the last image!");
        }
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        if((idx_current_group_+1) * 20 + 20 > list_files_group_.size())
        {
            if((idx_current_group_+1) * 20 < list_files_group_.size())
                idx_current_group_++;
            QVector<string> names;
            for (int i = 0; i+idx_current_group_*20 < list_files_group_.size(); i++) {
                names.push_back(list_files_group_[i+idx_current_group_*20].fileName().toStdString());
            }
            qimg_show_check_ = Mat2QImage(face_checker_.ShowFaces(idx_current_group_, dir_src_group_, names));
            QMessageBox::information(nullptr, "Information", "This is the last group!");
        }
        else {
            idx_current_group_++;
            QVector<string> names;
            for (int i = 0; i < 20; i++) {
                names.push_back(list_files_group_[i+idx_current_group_*20].fileName().toStdString());
            }
            qimg_show_check_ = Mat2QImage(face_checker_.ShowFaces(idx_current_group_, dir_src_group_, names));
        }
        ui->label_img_group->setScaledContents(true);
        ui->label_img_group->resize(ui->label_img_group->width(), ui->label_img_group->height());
        ui->label_img_group->setPixmap(QPixmap::fromImage(qimg_show_check_));
        ui->label_img_group->show();
        update();
        ui->label_current_group->setText(QString::number(idx_current_group_*20));
    }
}

void MainWindow::on_action_previous_image_triggered()
{    if(ui->tabWidget->currentIndex() == 0)
    {
        on_action_finish_one_image_triggered();
        if(idx_current_file_ > 0)
        {
            idx_current_file_--;
            img_ori_ = imread(QString(dir_src_ + QString("/") + list_files_.at(idx_current_file_).fileName()).toStdString(), IMREAD_COLOR);
            qimg_ori_ = Mat2QImage(img_ori_);
            img_ori_.copyTo(img_show_);
            qimg_show_ = Mat2QImage(img_show_);
            //        img_aligned_;
            num_pts_drawn_ = 0;
            pts_draw_.clear();
            pts_align_.clear();
            face_processor_.set_img_ori(img_ori_);
        }
        else {
            QMessageBox::information(nullptr, "Information", "This is the first image!");
        }
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        if(idx_current_group_ - 1 < 0)
        {
            QMessageBox::information(nullptr, "Information", "This is the first group!");
        }
        else {
            idx_current_group_--;
            QVector<string> names;
            for (int i = 0; i < 20; i++) {
                names.push_back(list_files_group_[i+idx_current_group_*20].fileName().toStdString());
            }
            qimg_show_check_ = Mat2QImage(face_checker_.ShowFaces(idx_current_group_, dir_src_group_, names));
        }
        ui->label_img_group->setScaledContents(true);
        ui->label_img_group->resize(ui->label_img_group->width(), ui->label_img_group->height());
        ui->label_img_group->setPixmap(QPixmap::fromImage(qimg_show_check_));
        ui->label_img_group->show();
        update();
        ui->label_current_group->setText(QString::number(idx_current_group_*20));
    }
}


void MainWindow::on_pushButton_dir_src_clicked()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        idx_current_file_ = 0;
        dir_src_ = QFileDialog::getExistingDirectory(this,
                                                     tr("Open Directory"),
                                                     "D:/LFWp/temp/outlier/original",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        QDir dir;
        dir.setPath(dir_src_);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Name);
        list_files_ = dir.entryInfoList();
//    for (int i = 0; i < list.size(); ++i) {
//        QFileInfo fileInfo = list.at(i);
//        qDebug() << qPrintable(QString("%1").arg(fileInfo.fileName()));
//    }
    }
}

void MainWindow::on_pushButton_dir_save_clicked()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        dir_save_ = QFileDialog::getExistingDirectory(this,
                                                      tr("Open Directory"),
                                                      "D:/LFWp/temp/outlier/aligned",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    }
}


void MainWindow::on_action_start_labeling_triggered()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        if(idx_current_file_ >= 0 &&
                idx_current_file_ < list_files_.size())
        {
            img_ori_ = imread(QString(dir_src_ + QString("/") + list_files_.at(idx_current_file_).fileName()).toStdString(), IMREAD_COLOR);
            qimg_ori_ = Mat2QImage(img_ori_);
            img_ori_.copyTo(img_show_);
            qimg_show_ = Mat2QImage(img_show_);
            //        img_aligned_;
            num_pts_drawn_ = 0;
            pts_draw_.clear();
            pts_align_.clear();
            face_processor_.set_img_ori(img_ori_);
            ui->label_ori_img->setScaledContents(true);
            ui->label_ori_img->resize(ui->label_ori_img->width(), ui->label_ori_img->height());
            ui->label_ori_img->setPixmap(QPixmap::fromImage(qimg_ori_));
        }
        else {
            QMessageBox::information(nullptr, "Error", "Invalid Index!");
        }
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        if(list_files_group_.size() < 20)
        {
            QVector<string> names;
            for (int i = 0; i < list_files_group_.size(); i++) {
                names.push_back(list_files_group_[i+idx_current_group_*20].fileName().toStdString());
            }
            qimg_show_check_ = Mat2QImage(face_checker_.ShowFaces(idx_current_group_, dir_src_group_, names));
        }
        else {
            QVector<string> names;
            for (int i = 0; i < 20; i++) {
                names.push_back(list_files_group_[i+idx_current_group_*20].fileName().toStdString());
            }
            qimg_show_check_ = Mat2QImage(face_checker_.ShowFaces(idx_current_group_, dir_src_group_, names));
        }
        ui->label_img_group->setScaledContents(true);
        ui->label_img_group->resize(ui->label_img_group->width(), ui->label_img_group->height());
        ui->label_img_group->setPixmap(QPixmap::fromImage(qimg_show_check_));
        ui->label_img_group->show();
        update();
        ui->label_current_group->setText(QString::number(idx_current_group_*20));
    }
}

void MainWindow::on_action_clear_triggered()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        img_ori_.copyTo(img_show_);
        qimg_show_ = Mat2QImage(img_show_);
        num_pts_drawn_ = 0;
        pts_draw_.clear();
        pts_align_.clear();
        ui->label_ori_img->setScaledContents(true);
        ui->label_ori_img->resize(ui->label_ori_img->width(), ui->label_ori_img->height());
        ui->label_ori_img->setPixmap(QPixmap::fromImage(qimg_ori_));
    }
}

void MainWindow::on_pushButton_dir_src_group_clicked()
{
    if(ui->tabWidget->currentIndex() == 1)
    {
        idx_current_group_ = 0;
        dir_src_group_ = QFileDialog::getExistingDirectory(this,
                                                     tr("Open Source Directory"),
                                                     "D:/LFWp/temp",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        QDir dir;
        dir.setPath(dir_src_group_);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Name);
        list_files_group_ = dir.entryInfoList();
    }
}

void MainWindow::on_pushButton_dir_ori_group_clicked()
{
    if(ui->tabWidget->currentIndex() == 1)
    {
        idx_current_group_ = 0;
        dir_ori_group_ = QFileDialog::getExistingDirectory(this,
                                                     tr("Open Original Directory"),
                                                     "D:/LFWp/original",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    }
}

void MainWindow::on_pushButton_dir_dst_group_clicked()
{
    if(ui->tabWidget->currentIndex() == 1)
    {
        idx_current_group_ = 0;
        dir_dst_group_ = QFileDialog::getExistingDirectory(this,
                                                     tr("Open Destination Directory"),
                                                     "D:/LFWp/temp/outlier/original",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    }
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        ui->action_start_labeling->setText("start labeling");
        ui->action_next_image->setText("next image");
        ui->action_previous_image->setText("previous image");
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {
        ui->action_start_labeling->setText("start checking");
        ui->action_next_image->setText("next group");
        ui->action_previous_image->setText("previous group");
    }
}
