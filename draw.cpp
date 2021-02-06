#include "draw.h"

void drawGridPoints(cv::Mat cmat, cv::Mat dvec, cv::Mat rvec, cv::Mat tvec,
                    std::vector<float> gx,
                    std::vector<float> gy,
                    std::vector<float> gz,
                    cv::Mat & img)
{

}

void drawGridPoints(cv::Mat cmat, cv::Mat dvec, cv::Mat rvec, cv::Mat tvec,
                    float gx0, float gx1, int ngx,
                    float gy0, float gy1, int ngy,
                    float gz0, float gz1, int ngz)
{

}


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
    cv::Scalar color, float alpha, int putText, int shift)
{
    int n = _points.rows * _points.cols * _points.channels() / 2;
    cv::Mat points(n, 2, CV_32F);
    if (_points.type() == CV_32F)
        for (int i = 0; i < n; i++)
        {
            points.at<float>(i, 0) = _points.at<float>(i, 0);
            points.at<float>(i, 1) = _points.at<float>(i, 1);
        }
    if (_points.type() == CV_32FC2)
        for (int i = 0; i < n; i++)
        {
            points.at<float>(i, 0) = _points.at<cv::Point2f>(i, 0).x;
            points.at<float>(i, 1) = _points.at<cv::Point2f>(i, 0).y;
        }

    cv::Mat imgCpy; img.copyTo(imgCpy);
    int i = 0;
    for (i = 0; i < points.rows; i++) {
        // circle
        if (symbol.compare("o") == 0) {
            int radius = size / 2 * (1 << shift);
            cv::Point pi(
                (int)(points.at<float>(i, 0) * (1 << shift) + 0.5f),
                (int)(points.at<float>(i, 1) * (1 << shift) + 0.5f));
            cv::circle(img, pi, radius, color, thickness, cv::LINE_8, shift);
        }
        // '+'
        else if (symbol.compare("+") == 0) {
            int d = size / 2;
            cv::Point pi, pj;
            pi = cv::Point(
                (int)((points.at<float>(i, 0) - d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) - 0) * (1 << shift) + 0.5f));
            pj = cv::Point(
                (int)((points.at<float>(i, 0) + d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) - 0) * (1 << shift) + 0.5f));
            cv::line(img, pi, pj, color, thickness, cv::LINE_8, shift);
            pi = cv::Point(
                (int)((points.at<float>(i, 0) - 0) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) - d) * (1 << shift) + 0.5f));
            pj = cv::Point(
                (int)((points.at<float>(i, 0) + 0) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) + d) * (1 << shift) + 0.5f));
            cv::line(img, pi, pj, color, thickness, cv::LINE_8, shift);
        }
        // 'x'
        else if (symbol.compare("x") == 0 || symbol.compare("X") == 0) {
            int d = size / 2;
            cv::Point pi, pj;
            pi = cv::Point(
                (int)((points.at<float>(i, 0) - d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) - d) * (1 << shift) + 0.5f));
            pj = cv::Point(
                (int)((points.at<float>(i, 0) + d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) + d) * (1 << shift) + 0.5f));
            cv::line(img, pi, pj, color, thickness, cv::LINE_8, shift);
            pi = cv::Point(
                (int)((points.at<float>(i, 0) - d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) + d) * (1 << shift) + 0.5f));
            pj = cv::Point(
                (int)((points.at<float>(i, 0) + d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) - d) * (1 << shift) + 0.5f));
            cv::line(img, pi, pj, color, thickness, cv::LINE_8, shift);
        }
        // 'square'
        else if (symbol.compare("square") == 0) {
            int d = size / 2;
            cv::Point pi, pj;
            pi = cv::Point(
                (int)((points.at<float>(i, 0) - d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) - d) * (1 << shift) + 0.5f));
            pj = cv::Point(
                (int)((points.at<float>(i, 0) + d) * (1 << shift) + 0.5f),
                (int)((points.at<float>(i, 1) + d) * (1 << shift) + 0.5f));
            cv::rectangle(img, pi, pj, color, thickness, cv::LINE_8, shift);
        }

        // add text
        if (putText >= 0) {
            char strbuf[10];
            int fontFace = cv::FONT_HERSHEY_DUPLEX;
            double fontScale = size / 8.0;
            int thickness = std::max(size / 6, 1);
            int lineType = 8;
            bool bottomLeftOri = false;
            snprintf(strbuf, 10, "%d", i + 1);
            cv::putText(img, std::string(strbuf),
                cv::Point((int)(points.at<float>(i, 0) + 0.5f), (int)(points.at<float>(i, 1) + 0.5f)),
                fontFace, fontScale, color, thickness, lineType, bottomLeftOri);
        }

    } // end of loop of each point

    // do transparency
    cv::addWeighted(img, alpha, imgCpy, 1 - alpha, 0, img);

    return 0;
}
