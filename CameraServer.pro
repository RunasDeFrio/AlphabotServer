QT += core
QT += network
QT -= gui

TARGET = SERVER
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += main.cpp\
    myserver.cpp \
    myrunnable.cpp \
    socketadapter.cpp \
    CameraIPSystem.cpp

HEADERS += \
    myserver.h \
    myrunnable.h \
    socketadapter.h \
    CameraIPSystem.h

INCLUDEPATH += /usr/local/include/raspicam

LIBS += /opt/vc/lib/libmmal.so
LIBS += /opt/vc/lib/libmmal_core.so
LIBS += /opt/vc/lib/libmmal_util.so
LIBS += /usr/local/lib/libraspicam.so 
LIBS += /usr/local/lib/libraspicam_cv.so

LIBS +=	-lopencv_world