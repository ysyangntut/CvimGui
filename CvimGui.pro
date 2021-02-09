QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CvimMainWindow.cpp \
    CvimProject.cpp \
    FileSeq.cpp \
    ImagePointsPicker.cpp \
    ImageSequence.cpp \
    IntrinsicCalibrator.cpp \
    Target.cpp \
    UserPointCalibrationDialog.cpp \
    constrained3dPositioning.cpp \
    cvimCalibration.cpp \
    cvimUtil.cpp \
    draw.cpp \
    enhancedCorrelationWithReference.cpp \
    generalConstrained3dPosition_newton.cpp \
    improCalib.cpp \
    improFileIO.cpp \
    improMath.cpp \
    improStrings.cpp \
    impro_util.cpp \
    main.cpp \
    matchTemplateWithRot.cpp \
    matchTemplateWithRotPyr.cpp \
    octaveLike.cpp \
    pickAPoint.cpp \
    qcustomplot.cpp \
    smoothZoomAndShow.cpp \
    sync.cpp \
    trackings.cpp \
    triangulatePoints2.cpp \
    warpToStrain.cpp

HEADERS += \
    CvimMainWindow.h \
    CvimProject.h \
    FileSeq.h \
    ImagePointsPicker.h \
    ImageSequence.h \
    IntrinsicCalibrator.h \
    Target.h \
    UserPointCalibrationDialog.h \
    constrained3dPositioning.h \
    cvim.h \
    cvimCalibration.h \
    cvimUtil.h \
    cvimUtilQt.h \
    draw.h \
    enhancedCorrelationWithReference.h \
    improCalib.h \
    improFileIO.h \
    improMath.h \
    improStrings.h \
    impro_util.h \
    impro_util.h \
    matchTemplateWithRot.h \
    matchTemplateWithRotPyr.h \
    octaveLike.h \
    pickAPoint.h \
    pickAPoint.h \
    qcustomplot.h \
    smoothZoomAndShow.h \
    sync.h \
    trackings.h \
    triangulatepoints2.h \
    warpToStrain.h

FORMS += \
    CvimMainWindow.ui \
    UserPointCalibrationDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# OpenCV 4.4
#win32:CONFIG(release, debug|release): LIBS += -LC:/opencv/opencv440/opencv/build/x64/vc15/lib/ -lopencv_world440
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv/opencv440/opencv/build/x64/vc15/lib/ -lopencv_world440d
#INCLUDEPATH += C:/opencv/opencv440/opencv/build/include
#DEPENDPATH += C:/opencv/opencv440/opencv/build/include

# OpenCV
win32:CONFIG(release, debug|release): LIBS += -LC:/opencv/opencv451x/opencv-4.5.1/build/install/x64/vc16/lib/ -lopencv_world451
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv/opencv451x/opencv-4.5.1/build/install/x64/vc16/lib/ -lopencv_world451d
else:unix: LIBS += -LC:/opencv/opencv451x/opencv-4.5.1/build/install/x64/vc16/lib/ -lopencv_world451
INCLUDEPATH += C:/opencv/opencv451x/opencv-4.5.1/build/install/include
DEPENDPATH += C:/opencv/opencv451x/opencv-4.5.1/build/install/include
