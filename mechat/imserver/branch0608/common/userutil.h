#ifndef USERUTIL_H
#define USERUTIL_H

#include "UserDataEntity.h"
class UserUtil
{
public:
    UserUtil();

    static bool  GetUserData(long lUserId,const UserDataList & userdataLst, UserDataEntity & entity);
};

#endif // USERUTIL_H
