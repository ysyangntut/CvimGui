#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

#include <opencv2/opencv.hpp>

#include "IntrinsicCalibrator.h"

cv::TermCriteria tc_30_eps = cv::TermCriteria(
            cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, DBL_EPSILON);

double calibrateCameraSingleImage(
        const cv::Mat & objPoints,
        const cv::Mat & imgPoints,
        const cv::Mat & colinearPoints,
        cv::Size        imgSize,
              cv::Mat & cmat,
              cv::Mat & dvec,
              cv::Mat & rvec,
              cv::Mat & tvec,
              cv::Mat & stdDeviationsIntrinsics,
              cv::Mat & stdDeviationsExtrinsics,
              cv::Mat & perViewErrors,
                  int   flags,
       cv::TermCriteria criteria)
{
    double rms = 0.0;
    vector<vector<cv::Point3f>> objPointsVec(1);
    vector<vector<cv::Point2f>> imgPointsVec(1);

    // conventional opencv calibration
    objPointsVec[0] = vector<cv::Point3f>(objPoints.cols * objPoints.rows);
    for (int i = 0; i < objPoints.cols * objPoints.rows; i++) {
        objPointsVec[0][i] = objPoints.at<cv::Point3f>(i);
    }
    imgPointsVec[0] = vector<cv::Point2f>(imgPoints.cols * imgPoints.rows);
    for (int i = 0; i < imgPoints.cols * imgPoints.rows; i++) {
        imgPointsVec[0][i] = imgPoints.at<cv::Point2f>(i);
    }

    // set initial guess of cmat
    if ((flags & cv::CALIB_USE_INTRINSIC_GUESS) == 0)
    {
        rvec = cv::Mat::zeros(3, 1, CV_64F);
        tvec = cv::Mat::zeros(3, 1, CV_64F);
        cmat = cv::Mat::eye(3, 3, CV_64F);
        cmat.at<double>(0, 0) = imgSize.width;
        cmat.at<double>(1, 1) = imgSize.width;
        cmat.at<double>(0, 2) = (imgSize.width - 1) * .5f;
        cmat.at<double>(1, 2) = (imgSize.height - 1) * .5f;
        cmat.at<double>(2, 2) = 1;
        dvec = cv::Mat::zeros(1, 12, CV_32F);
        flags = flags | cv::CALIB_USE_INTRINSIC_GUESS;
        cout << "Gave a cmat initial guess for camera calibration: " << cmat << "\n";
    }

    // if flag
    if (flags == 0) {
        cout << "Warning: You gave calibration flags which are all zeros.\n";
        cout << "         Likely you could get divergence from camera calibration.\n";
        cout.flush();
//        return 0.0;
    }

//    dvec = dvec.t();
    try {
        rms = cv::calibrateCamera(objPointsVec,
                                  imgPointsVec,
                                  imgSize,
                                  cmat,
                                  dvec,
                                  rvec,
                                  tvec ,
                                  stdDeviationsIntrinsics,
                                  stdDeviationsExtrinsics,
                                  perViewErrors,
                                  flags,
                                  criteria
                                  );

        cout << "Calibrationf flags is " << flags << "\n";
        cout << "Cmat: (type: " << cmat.type() << ")\n" << cmat << "\n";
        cout << "Dvec: (type: " << cmat.type() << ")\n" << dvec << "\n";
        cout << "Rvec: (type: " << cmat.type() << ")\n" << rvec << "\n";
        cout << "Tvec: (type: " << cmat.type() << ")\n" << tvec << "\n";
        cout << "stdDeviationsIntrinsics: (type: " << cmat.type() << ")\n" << stdDeviationsIntrinsics << "\n";
        cout << "stdDeviationsExtrinsics: (type: " << cmat.type() << ")\n" << stdDeviationsExtrinsics << "\n";
        cout << "perViewErrors: (type: " << cmat.type() << ")\n" << perViewErrors << "\n";
        cout.flush();
    } catch ( ... ) {
        cout << "Error: Failed to do calibration.\n";
        cout << "  Calibrationf flags is " << flags << "\n";
        cout << "  Change the flags and try again.\n";
        cout.flush();
    }

    // start doing minimization.



    return rms;
}
