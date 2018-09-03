#ifndef GROUPDATADAL_H
#define GROUPDATADAL_H


/*
 *群资料mysql访问层
 * */

#include <string>
#include <vector>
#include "GroupDataEntity.h"
#include "GroupMemberEntity.h"
using namespace std;
typedef vector<long> LongLst;
class HeadDAL
{

public:


    int GetGroupId(LongLst & lGroupIds,int iIndex);

    //一个群的所有成员(只包括群成员)
    int GetRealMemberById(long lGroupId,LongLst & lMemberIds);

    int GetUserHeadUrl(long lUserId, string & sHeadImage);

    int UpGroupHead(long lGroupId, string sHeadUrl);


private:


};

#endif //
