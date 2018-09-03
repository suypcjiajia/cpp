TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    ../../../libsrc/TConvert.cpp \
    ../../../libsrc/TFile.cpp \
    ../../../libsrc/TMd5.cpp \    
    ../../../libsrc/MyAES.cpp \
    ../../../libsrc/Base64EnDecode.cpp \
    ../../../libsrc/dal/MysqlConnect.cpp \
    ../../../libsrc/dal/TMultiMysqlDAL.cpp \
    bll/TBLL.cpp \
    dal/TDAL.cpp \
    ../../../libsrc/com/TCurl.cpp \
    ../../../libsrc/com/PostsAes.cpp \
    bll/ConnHashMapBLL.cpp \
    bll/WsServerBLL.cpp \
    ../../../libsrc/com/EvhtpSvr.cpp \
    bll/PushDataBLL.cpp \
    ../../../libsrc/com/WsSendCOM.cpp \
    bll/ConnKeepLiveBLL.cpp \
    ../../../libsrc/log/Logger.cpp \
    ../../../libsrc/log/Buffer.cpp \
    ../../../libsrc/MyUtil.cpp \
    ../../../libsrc/dal/RedisCli.cpp \
    dal/ImDAL.cpp \
    dal/UserDAL.cpp \
    ../../../libsrc/dal/RedisConnect.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../../../libsrc/com/TCurl.h \
    ../../../libsrc/TConvert.h \
    ../../../libsrc/TFile.h \
    ../../../libsrc/TMd5.h \
    ../../../libsrc/MyAES.h \    
    ../../../libsrc/Base64EnDecode.h \
    ../../../libsrc/dal/MysqlConnect.h \
    ../../../libsrc/dal/TMultiMysqlDAL.h \
    bll/TBLL.h \   
    dal/TDAL.h \
    ../../../libsrc/com/PostsAes.h \
    entity/PushUserEntity.h \
    bll/ConnHashMapBLL.h \
    entity/ConnHashMapEntity.h \
    bll/WsServerBLL.h \
    ../../../libsrc/com/RetJsonMsg.hpp \
    ../../../libsrc/com/EvhtpSvr.h \
    bll/PushDataBLL.h \
    ../../../libsrc/com/WsSendCOM.h \
    ../../../libsrc/com/ios_push/original_ssl_client.h \
    bll/ConnKeepLiveBLL.h \
    ../../../libsrc/log/Logger.h \
    ../../../libsrc/log/Buffer.h \
    ../../../libsrc/MyUtil.h \
    ../../../libsrc/dal/RedisCli.h \
    dal/ImDAL.h \
    dal/UserDAL.h \
    ../../../libsrc/dal/RedisConnect.h

APP_PATH = /home/cpp
APP_LIBSRC = $$APP_PATH/libsrc
APP_INC = $$APP_PATH/inc

DESTDIR += $$APP_PATH/bin
INCLUDEPATH += $$APP_PATH/libsrc
INCLUDEPATH += $$APP_PATH/libsrc/com/websocketpp-master
INCLUDEPATH += $$APP_PATH/libso
INCLUDEPATH += $$APP_PATH/inc/evhtp
INCLUDEPATH += $$APP_PATH/libsrc/com/hiredis-vip-master
INCLUDEPATH += $$APP_PATH/inc

LIBS += -L/tiyoume/lib
LIBS += -levhtp -levent -levent_openssl -levent_pthreads -lcrypto -lcryptopp -lpthread -lssl -ljson_linux-gcc-4.8_libmt -lhiredis_vip -luuid
#LIBS += -lcrypto -lcryptopp -lpthread -ljson_linux-gcc-4.8_libmt
LIBS += -lcurl -lmysqlclient -lboost_system
LIBS += -L$$APP_PATH/inc/Ice-3.6.1/lib
LIBS += -lIce -lIceUtil
