#include "TDAL.h"
#include "dal/MysqlConnect.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "TConvert.h"
#include "UserDataDAL.h"
#include "UserStateDAL.h"
#include "FriendResquestDAL.h"
#include "UserRelationDAL.h"
#include "PrivateChatDAL.h"
#include "UserSetDAL.h"
#include "OrderNumDAL.h"
#include "GroupChatDAL.h"
#include "GroupDataDAL.h"
#include "GroupMemberDAL.h"
#include "NoticeDAL.h"
#include "UserGradeDAL.h"
#include "RedPoinDAL.h"
#include "dal/PublicFansDAL.h"
#include "dal/PublicDataDAL.h"
#include "dal/PublicInfoDAL.h"
#include "dal/UserSignDAL.h"
#include "dal/UserVideoDAL.h"
TDAL* TDAL::mInstance = NULL;
TDAL* TDAL::GetInstance()
{
        if (mInstance == NULL){
                mInstance = new TDAL;
        }
        return mInstance;
}
//初始化
int TDAL::Init()
{
    //初始化要操作的数据库
    MysqlConnect::GetInstance()->Init("mechatdb");

    return 0;
}
