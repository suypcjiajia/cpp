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
    dal/UserDataDAL.cpp \
    bll/UserDataBLL.cpp \
    dal/TDAL.cpp \
    ../../../libsrc/com/TCurl.cpp \
    ../../../libsrc/com/EvhtpSvr.cpp \
    ../../../libsrc/com/PostsAes.cpp \
    dal/UserStateDAL.cpp \
    bll/UserStateBLL.cpp \
    dal/FriendResquestDAL.cpp \
    dal/PrivateChatDAL.cpp \
    dal/UserRelationDAL.cpp \
    bll/FriendResquestBLL.cpp \
    ../../../libsrc/com/PushDataCOM.cpp \
    bll/UserRelationBLL.cpp \
    bll/PrivateChatBLL.cpp \
    dal/UserSetDAL.cpp \
    bll/UserSetBLL.cpp \
    dal/OrderNumDAL.cpp \
    bll/PushBLL.cpp \
    dal/GroupDataDAL.cpp \
    dal/GroupMemberDAL.cpp \
    dal/GroupChatDAL.cpp \
    bll/GroupDataBLL.cpp \
    bll/GroupMemberBLL.cpp \
    bll/GroupChatBLL.cpp \
    bll/LukyMoneyBLL.cpp \
    dal/NoticeDAL.cpp \
    bll/NoticeBLL.cpp \
    cache/UserDataCache.cpp \
    dal/UserGradeDAL.cpp \
    common/userutil.cpp \
    ../../../libsrc/log/Logger.cpp \
    ../../../libsrc/log/Buffer.cpp \
    ../../../libsrc/MyUtil.cpp \
    dal/RedPoinDAL.cpp \
    dal/PublicDataDAL.cpp \
    dal/PublicFansDAL.cpp \
    dal/PublicInfoDAL.cpp \
    bll/PublicFansBLL.cpp \
    ../../../libsrc/com/Request.cpp \
    dal/UserSignDAL.cpp \
    bll/FansBankBLL.cpp \
    dal/FansBankOrderDAL.cpp \
    dal/FansBankUserValueDAL.cpp \
    dal/FansBankKValueDAL.cpp \
    dal/FansBankSallingDAL.cpp \
    param/Param.cpp \
    dal/FansBankFansmoveDAL.cpp \
    ../../../libsrc/dal/Basedal.cpp \
    dal/UserVideoDAL.cpp \
    ../../../libsrc/Token.cpp \
    bll/CommunityBLL.cpp \
    dal/CommunityPublishDAL.cpp \
    dal/CommunityRemarkDAL.cpp \
    dal/ThirdpayDAL.cpp \
    ../../../libsrc/Hmac.cpp \
    dal/FansbankKeyvaleDAL.cpp \
    dal/ThirdUserDAL.cpp \
    dal/FansbankCountDAL.cpp \
    dal/FenZhangDAL.cpp \
    dal/UserDeviceDAL.cpp

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
    ../../../libsrc/dal/MysqlConnect.h \
    ../../../libsrc/dal/TMultiMysqlDAL.h \
    bll/TBLL.h \   
    dal/UserDataDAL.h \
    bll/UserDataBLL.h \
    dal/TDAL.h \
    ../../../libsrc/com/PostsAes.h \
    dal/UserStateDAL.h \
    bll/UserStateBLL.h \
    ../../../libsrc/com/RetJsonMsg.hpp \
    dal/FriendResquestDAL.h \
    dal/PrivateChatDAL.h \
    dal/UserRelationDAL.h \
    bll/FriendResquestBLL.h \
    ../../../libsrc/com/PushDataCOM.h \
    bll/UserRelationBLL.h \
    bll/PrivateChatBLL.h \
    dal/UserSetDAL.h \
    bll/UserSetBLL.h \
    dal/OrderNumDAL.h \
    bll/PushBLL.h \
    ../../../inc/CmdValue.h \
    ../../../inc/ErrDef.h \
    dal/GroupDataDAL.h \
    dal/GroupMemberDAL.h \
    dal/GroupChatDAL.h \
    bll/GroupDataBLL.h \
    bll/GroupMemberBLL.h \
    bll/GroupChatBLL.h \
    bll/LukyMoneyBLL.h \
    ../../../inc/UserStateEntity.h \
    ../../../inc/UserSetEntity.h \
    ../../../inc/UserRelationEntity.h \
    ../../../inc/UserDataEntity.h \
    ../../../inc/TypeDef.h \
    ../../../inc/PrivateChatEntity.h \
    ../../../inc/GroupMemberEntity.h \
    ../../../inc/GroupDataEntity.h \
    ../../../inc/GroupChatEntity.h \
    ../../../inc/FriendResquestEntity.h \
    dal/NoticeDAL.h \
    ../../../inc/NoticeEntity.h \
    bll/NoticeBLL.h \
    cache/UserDataCache.h \
    dal/UserGradeDAL.h \
    common/userutil.h \
    ../../../libsrc/log/Logger.h \
    ../../../libsrc/log/Buffer.h \
    ../../../libsrc/MyUtil.h \
    dal/RedPoinDAL.h \
    dal/PublicDataDAL.h \
    dal/PublicFansDAL.h \
    dal/PublicInfoDAL.h \
    bll/PublicFansBLL.h \
    ../../../libsrc/com/Request.h \
    dal/UserSignDAL.h \
    bll/FansBankBLL.h \
    dal/FansBankOrderDAL.h \
    dal/FansBankUserValueDAL.h \
    dal/FansBankKValueDAL.h \
    dal/FansBankSallingDAL.h \
    param/Param.h \
    dal/FansBankFansmoveDAL.h \
    ../../../libsrc/dal/Basedal.h \
    dal/UserVideoDAL.h \
    ../../../libsrc/Token.h \
    bll/CommunityBLL.h \
    dal/CommunityPublishDAL.h \
    dal/CommunityRemarkDAL.h \
    dal/ThirdpayDAL.h \
    ../../../libsrc/Hmac.h \
    dal/FansbankKeyvaleDAL.h \
    dal/ThirdUserDAL.h \
    dal/FansbankCountDAL.h \
    dal/FenZhangDAL.h \
    dal/UserDeviceDAL.h

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
LIBS += -L/tiyoume/lib
LIBS += -levhtp -levent -levent_openssl -levent_pthreads -lcrypto -lcryptopp -lpthread -lssl -ljson_linux-gcc-4.8_libmt -luuid
LIBS += -lcurl -lmysqlclient -lboost_system -lboost_regex

