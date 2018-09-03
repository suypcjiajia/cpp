#ifndef NOTICEDAL_H
#define NOTICEDAL_H


/*
 *系统通知mysql访问层
 * */

#include <string>
#include <vector>
#include "NoticeEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class NoticeDAL
{

public:

    int CreateTable();

    int AddData( const NoticeEntity & entity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const string & sKey,NoticeEntity & entity);

    //获取未读通知
    //lUserId :
    int GetMyNotice( const long & lUserId,int iType,NoticeList & Lst);

    //更新为已读
    //lUserId  :
    int UpdateReaded(long lUserId,const NoticeList & lst);


    //获取红点情况
    int GetCountByLastTime(long lUserId, long lLastTime,int & iCount);
    //获取最后一条记录
    int GetLastRecord(long lUserId,NoticeEntity & entity);
private:
    const string msTableName = "notice";

};

#endif // NOTICEDAL_H
