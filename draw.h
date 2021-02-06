#ifndef DRAWPROJECTION_H
#define DRAWPROJECTION_H

#include <vector>
#include <opencv2/opencv.hpp>


void drawGridPoints(cv::Mat cmat, cv::Mat dvec, cv::Mat rvec, cv::Mat tvec,
                    std::vector<float> gx,
                    std::vector<float> gy,
                    std::vector<float> gz,
                    cv::Mat & img);

void drawGridPoints(cv::Mat cmat, cv::Mat dvec, cv::Mat rvec, cv::Mat tvec,
                    float gx0, float gx1, int ngx,
                    float gy0, float gy1, int ngy,
                    float gz0, float gz1, int ngz);

/*!
\param img the image to be drawn
\param points points positions in format of Mat(N,2,CV_32F)
\param symbol symbol of markers, can be "+", "x", "o", "square"
\param size the size of the markers (in pixels)
\param color cv::Scalar(blue,green,red)
\param alpha alpha of markers (0:invisible, 1:opaque)
*/
int drawImgPoints(cv::Mat & img, cv::Mat _points, std::string symbol,
    int size, int thickness,
    cv::Scalar color, float alpha, int putText, int shift);

#endif // DRAWPROJECTION_H
