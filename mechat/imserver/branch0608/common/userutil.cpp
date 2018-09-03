#include "userutil.h"
#include "UserRelationEntity.h"

UserUtil::UserUtil()
{

}



bool  UserUtil::GetUserData(long lUserId,const UserDataList & userdataLst,UserDataEntity & entity)
{
    for( UserDataList::const_iterator iter = userdataLst.begin(); iter != userdataLst.end(); iter++)
    {
        const UserDataEntity & uDataEntity = *iter ;
        if( uDataEntity.lUserId == lUserId){
            entity = uDataEntity;
            return true;
        }
    }
    return false;

}
