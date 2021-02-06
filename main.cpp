#include "CvimMainWindow.h"

#include <QApplication>

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CvimMainWindow w;
    w.show();
    return a.exec();
}
