#ifndef PUBLICFANSDAL_H
#define PUBLICFANSDAL_H

#include "dal/Basedal.h"


#include <string>
#include <vector>
#include "TypeDef.h"
#include "PublicFansEntity.h"
#include "UserDataEntity.h"
using namespace std;

/**
 *Copyright(C),TTD
 * 公众号粉丝数据访问
 * @author suyp
 * @modify suyp
 */


class PublicFansDAL : public BaseDal
{

public:

    /**
     * 创建表格
     * @param
     * @return
     */
    int CreateTable();

    /**
     * 是否存在
     * @param Id 公众号 lFansId 粉丝
     * @return 0 存在
     */
    int IsExist( long  Id,long lFansId);

    /**
     * 添加一条记录
     * @param entity
     * @return 0 sucess
     */
    int AddData( const PublicFansEntity & entity);

    /**
     * 获取一条记录
     * @param id 公众号 lFansId 粉丝
     * @param entity
     * @return 0 sucess
     */
    int GetEntity(long  id,long lFansId,PublicFansEntity & entity);


    /**
     * 增加消费次数
     * @param id 公众号 lFansId 粉丝
     * @param iIncreen 增加的消费次数
     * @return 0 sucess
     */
    int UpdateCount(long Id, long lFansId, int iIncreen);



    /**
     * retset消费次数
     * @param id 公众号 lFansId 粉丝
     * @param iCount 次数
     * @return 0 sucess
     */
    int ResetCount(long Id, long lFansId);

    /**
     * 变新粉丝类型
     * @param id 公众号 lFansId 粉丝
     * @param iType 粉丝类型
     * @return 0 sucess
     */
    int UpdateType(long Id, long lFansId, int iType);


    /**
     * 获取我消费3次以上的平台
     * @param lUserId 我id
     * @return 0 sucess publics
     */
    int GetMyPublic(long lUserId,PublicFansList & publics);

    /**
     * 获到公众号上的消费粉丝数
     * @param Id 公众号id
     * @return 0 sucess iCount 消费粉丝数
     */
    int GetConsumer(long Id,int & iCount);


    /**
     * 获到公众号上的消费粉丝（昵称头像等）
     * @param Id 公众号id
     * @param iIndex
     * @param iCount
     * @return 0 sucess userDataLst 消费粉丝
     */
    int GetConsumer(long Id,int iIndex ,int iCount, UserDataList & userDataLst);


    /**
     * 获取近一段时间内的消费粉丝数
     * @param Id 公众号id
     * @param lTime
     * @return 0 sucess iCount 消费粉丝数
     */
    int GetCountsConsumer(long Id,long lTime ,int & iCount);

    /**
     * 获到公众号上的男性消费粉丝数
     * @param Id 公众号id
     * @return 0 sucess userDataLst 消费粉丝
     */
    int GetMaleConsumer(long Id ,int & iCount);



private:
    const string msTableName = "public_fans";

};

#endif // PUBLICFANSDAL_H
