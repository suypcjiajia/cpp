#ifndef USERDEVICEDAL_H
#define USERDEVICEDAL_H

#include <string>
#include <vector>
#include "UserDeviceEntity.h"
#include "dal/Basedal.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:会员设备
 *Others:
*/
class UserDeviceDAL:public BaseDal
{
public:
    UserDeviceDAL(){
        msTableName = "user_device";
        msAllColumn = " userId,deviceId ";
    }
public:


    int replace( long  lUserId, const  string & deviceId  );

    int GetById(long id, UserDeviceEntity & entity);



private:
    int RowToEntity(MYSQL_ROW row ,UserDeviceEntity & entity);

};

#endif // USERSIGNDAL_H
