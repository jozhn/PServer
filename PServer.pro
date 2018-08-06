#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T14:37:11
#
#-------------------------------------------------

QMAKE_CXXFLAGS+=-std=c++11
CONFIG += c++11

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qss/qss.cpp \
    login.cpp \
    controller/recogize.cpp \
    src/core/chars_identify.cpp \
    src/core/chars_recognise.cpp \
    src/core/chars_segment.cpp \
    src/core/core_func.cpp \
    src/core/feature.cpp \
    src/core/params.cpp \
    src/core/plate_detect.cpp \
    src/core/plate_judge.cpp \
    src/core/plate_locate.cpp \
    src/core/plate_recognize.cpp \
    src/train/ann_train.cpp \
    src/train/annCh_train.cpp \
    src/train/create_data.cpp \
    src/train/svm_train.cpp \
    src/train/train.cpp \
    src/util/kv.cpp \
    src/util/program_options.cpp \
    src/util/util.cpp \
    thirdparty/LBP/helper.cpp \
    thirdparty/LBP/lbp.cpp \
    thirdparty/mser/mser2.cpp \
    thirdparty/svm/corrected_svm.cpp \
    thirdparty/textDetect/erfilter.cpp \
    thirdparty/xmlParser/xmlParser.cpp \
    server.cpp \
    serverthread.cpp \
    controller/fileutil.cpp \
    entity/platerecord.cpp \
    controller/personutil.cpp \
    controller/recordutil.cpp

HEADERS  += mainwindow.h \
    qss/qss.h \
    login.h \
    controller/recogize.h \
    include/easypr/core/character.hpp \
    include/easypr/core/chars_identify.h \
    include/easypr/core/chars_recognise.h \
    include/easypr/core/chars_segment.h \
    include/easypr/core/core_func.h \
    include/easypr/core/feature.h \
    include/easypr/core/params.h \
    include/easypr/core/plate.hpp \
    include/easypr/core/plate_detect.h \
    include/easypr/core/plate_judge.h \
    include/easypr/core/plate_locate.h \
    include/easypr/core/plate_recognize.h \
    include/easypr/train/ann_train.h \
    include/easypr/train/annCh_train.h \
    include/easypr/train/create_data.h \
    include/easypr/train/svm_train.h \
    include/easypr/train/train.h \
    include/easypr/util/kv.h \
    include/easypr/util/program_options.h \
    include/easypr/util/switch.hpp \
    include/easypr/util/util.h \
    include/easypr/api.hpp \
    include/easypr/config.h \
    include/easypr/version.h \
    include/easypr.h \
    thirdparty/LBP/helper.hpp \
    thirdparty/LBP/lbp.hpp \
    thirdparty/mser/mser2.hpp \
    thirdparty/svm/precomp.hpp \
    thirdparty/textDetect/erfilter.hpp \
    thirdparty/xmlParser/xmlParser.h \
    controller/database.h \
    server.h \
    serverthread.h \
    controller/fileutil.h \
    entity/platerecord.h \
    controller/personutil.h \
    controller/recordutil.h

FORMS    += mainwindow.ui \
    login.ui

RESOURCES += \
    qss/qss.qrc \
    app.qrc

INCLUDEPATH+=$$PWD\include

win32{
INCLUDEPATH+=C:\opencv\build\install\include
LIBS += C:\opencv\build\install\x86\mingw\bin\libopencv_*.dll
}
