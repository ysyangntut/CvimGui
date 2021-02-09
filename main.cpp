#include "CvimMainWindow.h"

#include <QApplication>

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>


int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);
    CvimMainWindow w;
    w.show();
    return qapp.exec();
}
