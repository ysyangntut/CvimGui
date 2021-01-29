#include "UserPointCalibrationDialog.h"
#include "ui_UserPointCalibrationDialog.h"

#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include <sstream>

#include <opencv2/opencv.hpp>

#include "pickAPoint.h"
#include "impro_util.h"


UserPointCalibrationDialog::UserPointCalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserPointCalibrationDialog)
{
    ui->setupUi(this);
    this->on_edImgSize_textChanged();
    this->on_edGlobalPoints_textChanged();
    this->on_edImgPoints_textChanged();
}

UserPointCalibrationDialog::~UserPointCalibrationDialog()
{
    delete ui;
}

QImage UserPointCalibrationDialog::opencvMatToQImage(const cv::Mat &img, std::string filename)
{
    QImage qimg;
    if (img.channels() == 3)
        qimg = QImage(this->img.data, this->img.cols, this->img.rows,
                      this->img.step, QImage::Format_BGR888);
    else if (img.channels() == 1)
        qimg = QImage(this->img.data, this->img.cols, this->img.rows,
                      this->img.step, QImage::Format_Grayscale8);
    else if (img.channels() == 4)  {
        QMessageBox msgBox;
        msgBox.setText("Warning: 4-channel image. Assuming ARGB32: " + QString::fromStdString(filename));
        msgBox.exec();
        qimg = QImage(this->img.data, this->img.cols, this->img.rows,
                      this->img.step, QImage::Format_ARGB32);
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Warning: Unknown format: " + QString::fromStdString(filename));
        msgBox.exec();
    }
    return qimg;
}

void UserPointCalibrationDialog::on_pbSelectImg_clicked()
{
    // Declare variables
    QString imgQFilename;
    int wLabel = this->ui->lbImg->width();
    int hLabel = this->ui->lbImg->height();


    // Get file
    imgQFilename = QFileDialog::getOpenFileName();
    if (imgQFilename.length() > 1)
        this->imgFilename = imgQFilename.toStdString();
    else
        return;

    // Read image
    this->img = cv::imread(this->imgFilename);
    if (this->img.cols <= 0 || this->img.rows <= 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Error: Cannot read image from file " + imgQFilename);
        msgBox.exec();
        return;
    }

    // Show image
    QImage qimg;
    qimg = opencvMatToQImage(this->img, this->imgFilename);

    QPixmap qpixmap = QPixmap::fromImage(qimg).scaled(wLabel, hLabel,
                      Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->ui->lbImg->setPixmap(qpixmap);
}

void UserPointCalibrationDialog::on_pbImgGv_clicked()
{
    // Show image in full resolution with QGraphicsView
    if (this->img.cols > 0 && this->img.rows > 0)
    {
        QGraphicsScene* scene = new QGraphicsScene();
        QGraphicsView* view = new QGraphicsView(scene);
        QImage qimg;
        qimg = opencvMatToQImage(this->img, this->imgFilename);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(qimg));
        scene->addItem(item);
        view->show();
    }
}

void UserPointCalibrationDialog::on_pbImgInteractive_clicked()
{
    cv::Point2f dummyPoint;
    pickAPoint("Interctive view", this->img, dummyPoint);
}

void UserPointCalibrationDialog::on_pbImshow1600_clicked()
{
    imshow_resize("Image Show 1600", this->img, cv::Size(1600, 900));
}

void UserPointCalibrationDialog::on_edGlobalPoints_textChanged()
{
    std::stringstream ss;
    std::vector<float> floatsInText;
    // get text (entire content to a single string)
    ss << this->ui->edGlobalPoints->toPlainText().toStdString();
    // read floats from text
    while (true) {
        float tmp;
        std::string stmp;
        // check EOF of the stream
        if (ss.eof() == true) break;
        // read a string
        ss >> stmp;
        // convert to a float
        try {
            tmp = std::stof(stmp);
        }  catch (...) {
            tmp = std::nanf("");
        }
        // append to vector
        if (std::isnan(tmp) == false)
            floatsInText.push_back(tmp);
    }
    // check if number of floats are multiple of three
    this->ui->lbNumGlobalPoints->setStyleSheet("color: black;");
    if (floatsInText.size() % 3 == 0) {
        // if multiple of three, display number of points, and copy to this->uGlobalPoints
        int npoint = (int) (floatsInText.size() / 3);
        char buf[1000];
        snprintf(buf, 1000, "%d points", npoint);
        this->ui->lbNumGlobalPoints->setText(buf);
        this->ui->lbNumGlobalPoints->setStyleSheet("color: black;");
        this->uGlobalPoints = cv::Mat(1, npoint, CV_32FC3);
        for (int i = 0; i < npoint; i++) {
            this->uGlobalPoints.at<cv::Point3f>(0, i).x = floatsInText[i * 3 + 0];
            this->uGlobalPoints.at<cv::Point3f>(0, i).y = floatsInText[i * 3 + 1];
            this->uGlobalPoints.at<cv::Point3f>(0, i).z = floatsInText[i * 3 + 2];
        }
    } else {
        // if not multiple of three, display number of floats by red fonts
        char buf[1000];
        snprintf(buf, 1000, "%d floats", (int) floatsInText.size());
        this->ui->lbNumGlobalPoints->setText(buf);
        this->ui->lbNumGlobalPoints->setStyleSheet("color: red;");
    }
}

void UserPointCalibrationDialog::on_edImgPoints_textChanged()
{
    std::stringstream ss;
    std::vector<float> floatsInText;
    // get text (entire content to a single string)
    ss << this->ui->edImgPoints->toPlainText().toStdString();
    // read floats from text
    while (true) {
        float tmp;
        std::string stmp;
        // check EOF of the stream
        if (ss.eof() == true) break;
        // read a string
        ss >> stmp;
        // convert to a float
        try {
            tmp = std::stof(stmp);
        }  catch (...) {
            tmp = std::nanf("");
        }
        // append to vector
        if (std::isnan(tmp) == false)
            floatsInText.push_back(tmp);
    }
    // check if number of floats are multiple of two
    this->ui->lbNumImgPoints->setStyleSheet("color: black;");
    if (floatsInText.size() % 2 == 0) {
        // if multiple of two, display number of points, and copy to this->uGlobalPoints
        int npoint = (int) (floatsInText.size() / 2);
        char buf[1000];
        snprintf(buf, 1000, "%d points", npoint);
        this->ui->lbNumImgPoints->setText(buf);
        this->ui->lbNumImgPoints->setStyleSheet("color: black;");
        this->uImgPoints = cv::Mat(1, npoint, CV_32FC2);
        for (int i = 0; i < npoint; i++) {
            this->uImgPoints.at<cv::Point2f>(0, i).x = floatsInText[i * 2 + 0];
            this->uImgPoints.at<cv::Point2f>(0, i).y = floatsInText[i * 2 + 1];
        }
    } else {
        // if not multiple of two, display number of floats by red fonts
        char buf[1000];
        snprintf(buf, 1000, "%d floats", (int) floatsInText.size());
        this->ui->lbNumImgPoints->setText(buf);
        this->ui->lbNumImgPoints->setStyleSheet("color: red;");
    }

    int a = cv::CALIB_USE_INTRINSIC_GUESS;
    return;
}

double calibrateCameraSingleImage(
        const cv::Mat & objPoints,
        const cv::Mat & imgPoints,
        const cv::Mat & colinearPoints,
        cv::Size        imgSize,
              cv::Mat & cmat,
              cv::Mat & dvec,
              cv::Mat & rvec,
              cv::Mat & tvec,
                  int   flags = 0,
       cv::TermCriteria criteria = cv::TermCriteria(
            cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, DBL_EPSILON));

void UserPointCalibrationDialog::on_pbCalibrate_clicked()
{
    // declarations
    cv::Mat objPoints, imgPoints;
    cv::Mat colinearPoints;
    cv::Mat cmat, dvec, rvec, tvec;
    cv::Size imgSize;
    int flags = 0;

    // data check
    if (this->uGlobalPoints.cols * this->uGlobalPoints.rows <= 0 ||
        this->uImgPoints.cols * this->uImgPoints.rows <= 0 ||
        this->uGlobalPoints.cols * this->uGlobalPoints.rows !=
        this->uImgPoints.cols * this->uImgPoints.rows)
    {
        QMessageBox mbox;
        mbox.setText("Error: Invalid # of 3D points and 2D points.");
        mbox.exec();
        return;
    }

    // get object points
    objPoints = this->uGlobalPoints;

    // get image points
    imgPoints = this->uImgPoints;

    // get image size
    imgSize = this->imgSize;
    if (imgSize.area() <= 0) {
        QMessageBox mbox;
        mbox.setText("Error: Invalid image size.");
        mbox.exec();
        return;
    }

    // get flags
    flags = this->ui->cb01->isChecked() * cv::CALIB_USE_INTRINSIC_GUESS +
            this->ui->cb02->isChecked() * cv::CALIB_FIX_ASPECT_RATIO    +
            this->ui->cb03->isChecked() * cv::CALIB_FIX_PRINCIPAL_POINT +
            this->ui->cb04->isChecked() * cv::CALIB_ZERO_TANGENT_DIST   +
            this->ui->cb05->isChecked() * cv::CALIB_FIX_FOCAL_LENGTH    +
            this->ui->cb06->isChecked() * cv::CALIB_FIX_K1              +
            this->ui->cb07->isChecked() * cv::CALIB_FIX_K2              +
            this->ui->cb08->isChecked() * cv::CALIB_FIX_K3              +
            this->ui->cb09->isChecked() * cv::CALIB_FIX_K4              +
            this->ui->cb10->isChecked() * cv::CALIB_FIX_K5              +
            this->ui->cb11->isChecked() * cv::CALIB_FIX_K6              +
            this->ui->cb12->isChecked() * cv::CALIB_RATIONAL_MODEL      +
            this->ui->cb13->isChecked() * cv::CALIB_THIN_PRISM_MODEL    +
            this->ui->cb14->isChecked() * cv::CALIB_FIX_S1_S2_S3_S4     +
            this->ui->cb15->isChecked() * cv::CALIB_TILTED_MODEL        +
            this->ui->cb16->isChecked() * cv::CALIB_FIX_TAUX_TAUY       +
            this->ui->cb17->isChecked() * cv::CALIB_USE_QR              +
            this->ui->cb18->isChecked() * cv::CALIB_FIX_TANGENT_DIST    +
            this->ui->cb19->isChecked() * cv::CALIB_FIX_INTRINSIC       +
            this->ui->cb20->isChecked() * cv::CALIB_SAME_FOCAL_LENGTH   +
            this->ui->cb21->isChecked() * cv::CALIB_ZERO_DISPARITY      +
            this->ui->cb22->isChecked() * cv::CALIB_USE_LU              +
            this->ui->cb23->isChecked() * cv::CALIB_USE_EXTRINSIC_GUESS;

    // calibraion
    double rms =
    calibrateCameraSingleImage(objPoints,
                               imgPoints,
                               colinearPoints,
                               imgSize,
                               cmat, dvec, rvec, tvec, flags);

    return;
}

void UserPointCalibrationDialog::on_edImgSize_textChanged()
{
    std::stringstream ss;
    std::vector<int> intsInText;

    // init
    this->imgSize = cv::Size(0, 0);
    // get text (entire content to a single string)
    ss << this->ui->edImgSize->toPlainText().toStdString();
    // get data
    for (int i = 0; i < 2; i++)
    {
        int tmp;
        std::string stmp;
        // check EOF of the stream
        if (ss.eof() == true) break;
        // read a string
        ss >> stmp;
        // convert to a float
        try {
            tmp = std::stoi(stmp);
        }  catch (...) {
            tmp = 0;
        }
        // append to vector
        if (tmp >= 1)
            intsInText.push_back(tmp);
    }
    if (intsInText.size() >= 2) {
        this->imgSize.width = intsInText[0];
        this->imgSize.height = intsInText[1];
        this->ui->edImgSize->setStyleSheet("color: black;");
    } else {
        this->ui->edImgSize->setStyleSheet("color: red;");
    }
    return ;
}
