#ifndef COMMENTDAL_H
#define COMMENTDAL_H
#include "dal/TMysqlDAL.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"
#include "define.h"
#include "dal/Basedal.h"
class CommentDAL :public BaseDal
{
public:
    CommentDAL(){

            msTableName = "comment";
            msAllColumn = " lPlatformId,lProductId,lAddTime,lUserId,sNickName,sHeadImage,sContent,sPics,iScore ";


    }

    int Add( const CommentEntity & entity );
    //查询数据
    int GetData(  long  lPlatformId ,long lProductId,long lAddTime,int iCount, CommentEntityLst & lst,int & iTotal);
    //获取综合评分
    int GetAvgSorce( long lPlatformId,long lProductId);


    int GetOneById( long lPlatformId,long lProductId,long lUserId,CommentEntity & entity);


private:

    int RowToEntity(MYSQL_ROW row,CommentEntity & entity);

    TMutex mMutex;//线程锁
};

#endif // COMMENTDAL_H
