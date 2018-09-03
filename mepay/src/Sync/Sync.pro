#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T14:55:41
#
#-------------------------------------------------


QT       -= gui

TARGET = Sync
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
CONFIG += c++11
APP_PATH = /home/cpp
DESTDIR += $$APP_PATH/bin

INCLUDEPATH += /usr/local/include/evhtp
INCLUDEPATH += $$APP_PATH/libsrc
INCLUDEPATH += $$APP_PATH/inc
INCLUDEPATH += $$APP_PATH/mepay/inc

LIBS += -L$$APP_PATH/inc/Ice-3.6.1/lib
LIBS += -lpthread -lmysqlclient
LIBS += -L/lib
LIBS += -levhtp -levent -levent_openssl -levent_pthreads -lcrypto -lcryptopp -ldl -lrt -lpthread -lssl -ljson_linux-gcc-4.8_libmt -luuid -lcurl



SOURCES += main.cpp \
    dal/ShopInfoDAL.cpp \
    dal/KeySecretDAL.cpp \
    ../../../libsrc/dal/MysqlConnect.cpp \
    ../../../libsrc/dal/TMultiMysqlDAL.cpp \
    ../../../libsrc/TConvert.cpp \
    ../../../libsrc/log/Buffer.cpp \
    ../../../libsrc/log/Logger.cpp \
    ../../../libsrc/sha.cpp \
    ../../../libsrc/MyUtil.cpp \
    ../../../libsrc/com/Request.cpp \
    ../../../libsrc/MyAES.cpp \
    ../../../libsrc/Base64EnDecode.cpp \
    ../../../libsrc/com/TCurl.cpp \
    ../../../libsrc/TMd5.cpp \
    bll/agentsync.cpp \
    dal/OfflineOrderDAL.cpp \
    ../../../libsrc/TFile.cpp \
    dal/PerAgentDAL.cpp

HEADERS += \
    dal/ShopInfoDAL.h \
    dal/KeySecretDAL.h \
    ../../../libsrc/dal/MysqlConnect.h \
    ../../../libsrc/dal/TMultiMysqlDAL.h \
    ../../../libsrc/TConvert.h \
    ../../../libsrc/log/Buffer.h \
    ../../../libsrc/log/Logger.h \
    ../../../libsrc/sha.h \
    ../../../libsrc/MyUtil.h \
    ../../../libsrc/com/Request.h \
    ../../../libsrc/MyAES.h \
    ../../../libsrc/Base64EnDecode.h \
    ../../../libsrc/com/TCurl.h \
    ../../../libsrc/TMd5.h \
    bll/agentsync.h \
    dal/OfflineOrderDAL.h \
    ../../../libsrc/TFile.h \
    dal/PerAgentDAL.h
