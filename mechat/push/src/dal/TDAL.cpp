#include "TDAL.h"
#include "dal/MysqlConnect.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "TConvert.h"
#include "UserDAL.h"

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

    //创建表
    UserDAL dal;
    dal.CreateTable();

    return 0;
}
