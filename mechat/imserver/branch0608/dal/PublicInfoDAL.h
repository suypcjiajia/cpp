#ifndef PUBLICINFODAL_H
#define PUBLICINFODAL_H



#include <string>
#include <vector>
#include "TypeDef.h"
#include "dal/Basedal.h"
#include "PublicInfoEntity.h"
using namespace std;

/**
 *Copyright(C),TTD
 * 公众号信息数据访问
 * @author suyp
 * @modify suyp
 */

class PublicInfoDAL : public BaseDal
{

public:

    /**
     * 创建表格
     * @param
     * @return
     */
    int CreateTable();

    /**
     * 添加一条记录
     * @param entity
     * @return 0 sucess
     */
    int AddData( const PublicInfoEntity & entity);

    /**
     * 获取一条记录
     * @param sKey 记录Id
     * @param entity
     * @return 0 sucess
     */
    int GetEntity(string sKey,PublicInfoEntity & entity);


    /**
     * 获取未读
     * @param entity
     * @return 0 sucess
     */
    int GetMyInfo(  long  lUserId, const longs & ids, long  lAddTime,  int  iCount,PublicInfoList & infos);

    int UpdateReaded(long lUserId,const PublicInfoList & lst);
private:

    int Execute(const string & sSql,PublicInfoList & infos);
    const string msTableName = "public_info";

};

#endif // PUBLICINFODAL_H
