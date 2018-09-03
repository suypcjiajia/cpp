#ifndef PUBLICDATADAL_H
#define PUBLICDATADAL_H

#include "dal/Basedal.h"


#include <string>
#include <vector>
#include "TypeDef.h"
#include "PublicDataEntity.h"
using namespace std;

/**
 *Copyright(C),TTD
 * 公众号资料数据访问
 * @author suyp
 * @modify suyp
 */


class PublicDataDAL : public BaseDal
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
     * @param Id 公众号
     * @return 0 存在
     */
    int IsExist( long  Id);

    /**
     * 添加一条记录
     * @param entity
     * @return 0 sucess
     */
    int AddData( const PublicDataEntity & entity);

    /**
     * 添加一条记录
     * @param id 公众号
     * @param entity
     * @return 0 sucess -5 数据不存在
     */
    int GetEntity(long  id,PublicDataEntity & entity);


    /**
     * 更新名称
     * @param id 公众号
     * @param sValue 头像
     * @return 0 sucess
     */
    int UpdateName(long Id, string sValue);

    /**
     * 更新头像
     * @param id 公众号
     * @param sValue 头像
     * @return 0 sucess
     */
    int UpdateHead(long Id, string sValue);


private:
    const string msTableName = "public_data";

};

#endif // PUBLICDATADAL_H
