#-------------------------------------------------
#
# Project created by QtCreator 2017-07-18T15:40:12
#
#-------------------------------------------------
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
#CONFIG -= qt
CONFIG += c++11
APP_PATH = /home/cpp
DESTDIR += $$APP_PATH/bin


TARGET = fansbank_dispatcher
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += /usr/local/include/evhtp
INCLUDEPATH += $$APP_PATH/libsrc
INCLUDEPATH += $$APP_PATH/inc
INCLUDEPATH += $$APP_PATH/mepay/inc

LIBS += -L$$APP_PATH/inc/Ice-3.6.1/lib
LIBS += -lpthread -lmysqlclient
LIBS += -L/lib
LIBS += -levhtp -levent -levent_openssl -levent_pthreads -lcrypto -lcryptopp -ldl -lrt -lpthread -lssl -ljson_linux-gcc-4.8_libmt -luuid -lcurl



SOURCES += main.cpp \
    bll/Priceispatch.cpp \
    bll/TBLL.cpp \
    dal/UserSignDAL.cpp \
    dal/UserValueDAL.cpp \
    dal/KValueDAL.cpp \
    dal/UserDataDAL.cpp \
    dal/ConfigDAL.cpp \
    ../../libsrc/log/Buffer.cpp \
    ../../libsrc/log/Logger.cpp \
    ../../libsrc/Base64EnDecode.cpp \
    ../../libsrc/MyAES.cpp \
    ../../libsrc/MyUtil.cpp \
    ../../libsrc/TConvert.cpp \
    ../../libsrc/TFile.cpp \
    ../../libsrc/TMd5.cpp \
    ../../libsrc/dal/MysqlConnect.cpp \
    ../../libsrc/dal/TMultiMysqlDAL.cpp \
    ../../libsrc/com/Request.cpp \
    ../../libsrc/com/PostsAes.cpp \
    ../../libsrc/com/TCurl.cpp \
    ../../libsrc/dal/Basedal.cpp \
    ../../libsrc/Hmac.cpp \
    dal/FansbankKeyvaleDAL.cpp

HEADERS += \
    bll/Priceispatch.h \
    bll/TBLL.h \
    dal/UserSignDAL.h \
    ../../inc/UserSignEntity.h \
    ../../inc/TypeDef.h \
    dal/UserValueDAL.h \
    dal/KValueDAL.h \
    dal/UserDataDAL.h \
    ../../inc/KVlaueEntity.h \
    dal/ConfigDAL.h \
    ../../libsrc/log/Buffer.h \
    ../../libsrc/log/Logger.h \
    ../../libsrc/Base64EnDecode.h \
    ../../libsrc/MyAES.h \
    ../../libsrc/MyUtil.h \
    ../../libsrc/TConvert.h \
    ../../libsrc/TFile.h \
    ../../libsrc/TMd5.h \
    ../../libsrc/dal/MysqlConnect.h \
    ../../libsrc/dal/TMultiMysqlDAL.h \
    ../../libsrc/com/Request.h \
    ../../libsrc/com/PostsAes.h \
    ../../libsrc/com/TCurl.h \
    ../../libsrc/dal/Basedal.h \
    ../../libsrc/Hmac.h \
    dal/FansbankKeyvaleDAL.h
