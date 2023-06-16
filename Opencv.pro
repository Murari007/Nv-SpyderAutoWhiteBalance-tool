QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    inputdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    inputdialog.h \
    mainwindow.h

FORMS += \
    inputdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_calib3d460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_core460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_dnn460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_features2d460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_flann460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_gapi460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_highgui460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_imgcodecs460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_imgproc460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_ml460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_objdetect460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_photo460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_stitching460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_video460.dll
LIBS += -LC:/Opencv-4.6.0/opencv/build/install/x64/mingw/lib/ -llibopencv_videoio460.dll


INCLUDEPATH += C:/Opencv-4.6.0/opencv/build/include
DEPENDPATH += C:/Opencv-4.6.0/opencv/build/include

DISTFILES += \
    C:/Users/gmura/Downloads/SpyBot/SpyBot.qss


