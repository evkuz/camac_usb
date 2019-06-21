#-------------------------------------------------
#
# Project created by QtCreator 2019-05-06T13:01:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KK_Tab_14_start_acquisition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mythread.cpp \
    ftdi_d2xx.cpp \
    tab_camac_paint.cpp \
    graph.cpp \
    #camac.cpp

HEADERS  += mainwindow.h \
    evkuz.h \
    mythread.h \
    ttx_device.h \
    ftdi_d2xx.h \
    graph.h \
    tab_camac_paint.h \
    stat.h \
    command_list.h \
    buffer_of_hyst_points.h


FORMS    += mainwindow.ui

CONFIG += c++11
CONFIG += qwt


INCLUDEPATH += $$PWD/../../../FPGA/ftdi_drivers_v2.12.28/



INCLUDEPATH += d:\Qt\boost_1_61_0\

LIBS += d:\Qt\boost_chrono\boost\bin.v2\libs\system\build\gcc-mingw-4.8.2\debug\libboost_system-mgw48-d-1_61.dll
LIBS += d:\Qt\boost_chrono\boost\bin.v2\libs\chrono\build\gcc-mingw-4.8.2\release\libboost_chrono-mgw48-1_61.dll



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../FPGA/ftdi_drivers_v2.12.28/i386/ -lftd2xx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../FPGA/ftdi_drivers_v2.12.28/i386/ -lftd2xx
