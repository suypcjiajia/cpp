TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
#CONFIG -= qt
CONFIG += c++11
APP_PATH = /home/cpp
DESTDIR += $$APP_PATH/bin

TARGET = MepayHttp
SOURCES += main.cpp \
    bll/ZoneAgnetBLL.cpp \
    dal/ZoneAgentDAL.cpp \
    bll/TBLL.cpp \
    bll/ShopInfoBLL.cpp \
    dal/ShopInfoDAL.cpp \
    bll/WalletAccountBLL.cpp \
    dal/WalletAccountDAL.cpp \
    bll/WalletDayBookBLL.cpp \
    dal/WalletDayBookDAL.cpp \
    ../../../libsrc/Base64EnDecode.cpp \
    ../../../libsrc/MyAES.cpp \
    ../../../libsrc/TConvert.cpp \
    ../../../libsrc/TMd5.cpp \
    ../../../libsrc/TFile.cpp \
    ../../../libsrc/com/EvhtpSvr.cpp \
    ../../../libsrc/dal/TMultiMysqlDAL.cpp \
    ../../../libsrc/dal/TMysqlDAL.cpp \
    ../../../libsrc/HangPing.cpp \
    dal/PersonalAgentDAL.cpp \
    bll/PersonalAgentBLL.cpp \
    ../../../libsrc/MyUtil.cpp \
    ../../../libsrc/com/PostsAes.cpp \
    ../../../libsrc/com/TCurl.cpp \
    bll/ServerBLL.cpp \
    dal/BankCardDAL.cpp \
    bll/BankCardBLL.cpp \
    dal/BankDAL.cpp \
    dal/ShopSettingDAL.cpp \
    ../../../libsrc/log/Buffer.cpp \
    ../../../libsrc/log/Logger.cpp \
    dal/AdvDAL.cpp \
    dal/CommentDAL.cpp \
    dal/ShowDAL.cpp \
    ../../../libsrc/dal/MysqlConnect.cpp \
    dal/KeySecretDAL.cpp \
    ../../../libsrc/sha.cpp \
    dal/SubjectDAL.cpp \
    dal/ClassDAL.cpp \
    ../../../libsrc/dal/Basedal.cpp \
    bll/Dispathcer.cpp \
    bll/CouponBLL.cpp \
    dal/CouponDAL.cpp \
    dal/CouponCodeDAL.cpp \
    ../../../libsrc/com/Request.cpp \
    dal/UserPowerDAL.cpp \
    bll/PowerBLL.cpp \
    bll/basebll.cpp \
    ../../../libsrc/Hmac.cpp

include(deployment.pri)
qtcAddDeployment()


INCLUDEPATH += /usr/local/include/evhtp
INCLUDEPATH += $$APP_PATH/libsrc
INCLUDEPATH += $$APP_PATH/inc
INCLUDEPATH += $$APP_PATH/mepay/inc

LIBS += -L$$APP_PATH/inc/Ice-3.6.1/lib
LIBS += -lpthread -lmysqlclient
LIBS += -L/lib
LIBS += -levhtp -levent -levent_openssl -levent_pthreads -lcrypto -lcryptopp -ldl -lrt -lpthread -lssl -ljson_linux-gcc-4.8_libmt -luuid -lcurl

HEADERS +=  bll/ZoneAgnetBLL.h \
    dal/ZoneAgentDAL.h \
    bll/TBLL.h \
    bll/basebll.h \
    bll/ShopInfoBLL.h \
    dal/ShopInfoDAL.h \
    bll/WalletAccountBLL.h \
    dal/WalletAccountDAL.h \
    bll/WalletDayBookBLL.h \
    dal/WalletDayBookDAL.h \
    ../../../libsrc/Base64EnDecode.h \
    ../../../libsrc/MyAES.h \
    ../../../libsrc/TConvert.h \
    ../../../libsrc/TMd5.h \
    ../../../libsrc/TFile.h \
    ../../../inc/ErrDef.h \
    ../../inc/define.h \
    ../../../libsrc/com/EvhtpSvr.h \
    ../../../libsrc/dal/TMultiMysqlDAL.h \
    ../../../libsrc/dal/TMysqlDAL.h \
    ../../../libsrc/HangPing.h \
    dal/PersonalAgentDAL.h \
    bll/PersonalAgentBLL.h \
    ../../../libsrc/MyUtil.h \
    ../../../libsrc/com/PostsAes.h \
    ../../../libsrc/com/TCurl.h \
    bll/ServerBLL.h \
    dal/BankCardDAL.h \
    bll/BankCardBLL.h \
    dal/BankDAL.h \
    dal/ShopSettingDAL.h \
    ../../../libsrc/log/Buffer.h \
    ../../../libsrc/log/Logger.h \
    dal/AdvDAL.h \
    dal/CommentDAL.h \
    dal/ShowDAL.h \
    ../../../libsrc/dal/MysqlConnect.h \
    dal/KeySecretDAL.h \
    ../../../libsrc/sha.h \
    dal/SubjectDAL.h \
    dal/ClassDAL.h \
    ../../../libsrc/dal/Basedal.h \
    bll/Dispathcer.h \
    bll/CouponBLL.h \
    dal/CouponDAL.h \
    dal/CouponCodeDAL.h \
    ../../../libsrc/com/Request.h \
    dal/UserPowerDAL.h \
    bll/PowerBLL.h \
    ../../../libsrc/Hmac.h

