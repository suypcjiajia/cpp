TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
SOURCES += main.cpp \
    ../../../libsrc/com/EvhtpSvr.cpp \
    ../../../libsrc/com/TCurl.cpp \
    ../../../libsrc/TConvert.cpp \
    ../../../libsrc/TFile.cpp \
    ../../../libsrc/TMd5.cpp \
    ../../../libsrc/MyAES.cpp \
    ../../../libsrc/Base64EnDecode.cpp \
    bll/TBLL.cpp \
    bll/PhoneCodeBLL.cpp \
    bll/PhoneCodeMapBLL.cpp \
    ../inc/top-sdk-c/src/curlResponse.c \
    ../inc/top-sdk-c/src/globalConfig.c \
    ../inc/top-sdk-c/src/json.c \
    ../inc/top-sdk-c/src/Md5Util.c \
    ../inc/top-sdk-c/src/SingleMap.c \
    ../inc/top-sdk-c/src/TaobaoClient.c \
    ../inc/top-sdk-c/src/TopRequest.c \
    ../inc/top-sdk-c/src/TopResponse.c \
    ../inc/top-sdk-c/src/WebUtils.c \
    ../../../libsrc/log/Logger.cpp \
    ../../../libsrc/log/Buffer.cpp \
    ../../../libsrc/MyUtil.cpp \
    dal/MyDAL.cpp \
    ../../../libsrc/dal/Basedal.cpp \
    ../../../libsrc/dal/MysqlConnect.cpp \
    ../../../libsrc/dal/TMultiMysqlDAL.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \    
    ../../../libsrc/com/EvhtpSvr.h \
    ../../../libsrc/com/TCurl.h \    
    ../../../libsrc/TConvert.h \
    ../../../libsrc/TFile.h \
    ../../../libsrc/TMd5.h \
    ../../../libsrc/MyAES.h \
    ../../../libsrc/Base64EnDecode.h \
    bll/TBLL.h \
    bll/PhoneCodeBLL.h \
    bll/PhoneCodeMapBLL.h \
    ../inc/top-sdk-c/topsdk.h \
    ../inc/top-sdk-c/src/curlResponse.h \
    ../inc/top-sdk-c/src/json.h \
    ../inc/top-sdk-c/src/Md5Util.h \
    ../inc/top-sdk-c/src/SingleMap.h \
    ../inc/top-sdk-c/src/TopResponse.h \
    ../inc/top-sdk-c/src/WebUtils.h \
    ../../../libsrc/com/RetJsonMsg.hpp \
    ../../../libsrc/log/Logger.h \
    ../../../libsrc/log/Buffer.h \
    ../../../libsrc/MyUtil.h \
    dal/MyDAL.h \
    ../../../libsrc/dal/Basedal.h \
    ../../../libsrc/dal/MysqlConnect.h \
    ../../../libsrc/dal/TMultiMysqlDAL.h





APP_PATH = /home/cpp
APP_LIBSRC = $$APP_PATH/libsrc
APP_INC = $$APP_PATH/inc

DESTDIR += $$APP_PATH/bin
INCLUDEPATH += $$APP_PATH/libsrc
INCLUDEPATH += $$APP_PATH/libso
INCLUDEPATH += $$APP_PATH/inc/evhtp
INCLUDEPATH += $$APP_INC


LIBS += -L/lib
LIBS += -levhtp -levent -levent_openssl -levent_pthreads -lcrypto -lcryptopp -lpthread -lssl -ljson_linux-gcc-4.8_libmt -luuid -lmysqlclient
LIBS += -lcurl



