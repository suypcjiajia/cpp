#include "ConnHashMapBLL.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "TBLL.h"
#include "MyAES.h"
#include "TMd5.h"
#include "com/PostsAes.h"
#include "com/RetJsonMsg.hpp"
#include "bll/PushDataBLL.h"
using namespace std;
ConnHashMapBLL* ConnHashMapBLL::mInstance = NULL;
ConnHashMapBLL* ConnHashMapBLL::GetInstance()
{
        if (mInstance == NULL){
                mInstance = new ConnHashMapBLL;
        }
        return mInstance;
}
//添加用户链接
int ConnHashMapBLL::Add(const long plUserId, const ConnHashMapEntity& pConnHashMapEntity)
{
    ScopeLock lock( this->mMutex );

    this->mOnlineUserHashMap[plUserId] = pConnHashMapEntity;

    return 0;
}
//删除用户链接
int ConnHashMapBLL::Remove(const long plUserId)
{
    ScopeLock lock( this->mMutex );
    OnlineUserHashMap::iterator iter= this->mOnlineUserHashMap.find( plUserId );
    if( iter != this->mOnlineUserHashMap.end() ) {
            this->mOnlineUserHashMap.erase(plUserId);
    }
    return 0;
}
//查询用户链接
int ConnHashMapBLL::Find( const long plUserId, ConnHashMapEntity& pConnHashMapEntity )
{
    ScopeLock lock( this->mMutex );

    OnlineUserHashMap::iterator iter= this->mOnlineUserHashMap.find( plUserId );
    if( iter != this->mOnlineUserHashMap.end() ) {
        pConnHashMapEntity = (iter->second);
    } else {
        return -1;
    }
    return 0;
}

int ConnHashMapBLL::SetMsgCount(const long plUserId,int iCount)
{
    ScopeLock lock( this->mMutex );

    OnlineUserHashMap::iterator iter= this->mOnlineUserHashMap.find( plUserId );
    if( iter != this->mOnlineUserHashMap.end() ) {
        ConnHashMapEntity & entity = (iter->second);
        entity.iMsgCount = iCount;
    } else {
        return -1;
    }
    return 0;

}

int ConnHashMapBLL::Copy(OnlineUserHashMap & connHashMap)
{
    ScopeLock lock( this->mMutex );
    connHashMap = mOnlineUserHashMap;
    return 0;
}


