#ifndef ConnHashMapBLL_H
#define ConnHashMapBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include <entity/ConnHashMapEntity.h>
#include "dal/UserDAL.h"
using namespace std;

typedef std::tr1::unordered_map<long,ConnHashMapEntity> OnlineUserHashMap;
class ConnHashMapBLL
{
public:  static ConnHashMapBLL* GetInstance();
private: static ConnHashMapBLL* mInstance;
private: TMutex mMutex;                  //线程锁
public:
    //添加用户链接
    int Add( const long plUserId, const ConnHashMapEntity& pConnHashMapEntity );
    //删除用户链接
    int Remove( const long plUserId );
    //查询用户链接
    int Find( const long plUserId, ConnHashMapEntity& pConnHashMapEntity );
    int SetMsgCount(const long plUserId,int iCount);
    int Copy(OnlineUserHashMap & connHashMap);
private:    


    //内存:用户链接
    OnlineUserHashMap mOnlineUserHashMap;
    UserDAL mUserDAL;
};

#endif // ConnHashMapBLL_H
