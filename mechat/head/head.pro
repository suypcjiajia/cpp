#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T15:02:51
#
#-------------------------------------------------

QT       += core
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = head
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    HeadDal.cpp \
    ../../libsrc/TMd5.cpp \
    ../../libsrc/TConvert.cpp \
    ../../libsrc/MyAES.cpp \
    ../../libsrc/HangPing.cpp \
    ../../libsrc/Base64EnDecode.cpp \
    ../../libsrc/dal/TMysqlDAL.cpp \
    ../../libsrc/dal/TMultiMysqlDAL.cpp \
    ../../libsrc/dal/MysqlConnect.cpp \
    ../../libsrc/OssManage.cpp \
    ../../libsrc/com/PostsAes.cpp \
    ../../libsrc/com/TCurl.cpp \
    ../../libsrc/log/Logger.cpp \
    ../../libsrc/log/Buffer.cpp \
    ../../libsrc/MyUtil.cpp

HEADERS  += mainwindow.h \
    HeadDal.h \
    ../../libsrc/TMutex.hpp \
    ../../libsrc/TMd5.h \
    ../../libsrc/TConvert.h \
    ../../libsrc/MyAES.h \
    ../../libsrc/mxml.h \
    ../../libsrc/HangPing.h \
    ../../libsrc/Base64EnDecode.h \
    ../../libsrc/dal/TMysqlDAL.h \
    ../../libsrc/dal/TMultiMysqlDAL.h \
    ../../libsrc/dal/MysqlConnect.h \
    ../../libsrc/OssManage.h \
    ../../libsrc/com/PostsAes.h \
    ../../libsrc/com/TCurl.h \
    ../../libsrc/log/Logger.h \
    ../../libsrc/log/Buffer.h \
    ../../libsrc/MyUtil.h

FORMS    +=


APP_PATH = /home/cpp
APP_LIBSRC = $$APP_PATH/libsrc
APP_INC = $$APP_PATH/inc

DESTDIR += $$APP_PATH/bin
INCLUDEPATH += $$APP_PATH/libsrc
INCLUDEPATH += $$APP_PATH/libso
INCLUDEPATH += $$APP_PATH/inc/evhtp
INCLUDEPATH += $$APP_PATH/
INCLUDEPATH += $$APP_PATH/inc
INCLUDEPATH += /home/cpp/libsrc/oss/oss_c_sdk
INCLUDEPATH += /home/cpp/libsrc/apr

LIBS += -L/lib
LIBS += -levhtp -levent -levent_openssl -levent_pthreads -lcrypto -lcryptopp -lpthread -lssl -ljson_linux-gcc-4.8_libmt
LIBS += -lcurl -lmysqlclient -luuid
LIBS += -L/usr/local/apr/lib -L/usr/local/apr/lib/apr-util-1
LIBS += -lapr-1 -laprutil-1   /usr/local/lib/liboss_c_sdk_static.a -lmxml -lcurl /usr/local/apr/lib/libapr-1.a /usr/local/apr/lib/libaprutil-1.a

