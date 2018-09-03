#ifndef ThirdpayDAL_H
#define ThirdpayDAL_H

#include <string>
#include <vector>
#include "dal/Basedal.h"
#include "ThirdpayEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class ThirdpayDAL:public BaseDal
{

public:
    ThirdpayDAL(){
        msTableName = "thirdpay";
        msAllColumn = " id,openid,phone,platformId,pushId,addTime,money,thirdType ";
    }

    int Insert( ThirdpayEntiy& entity );
    int GetByPhone(string sPhone, int page,ThirdpayList & lst);

private:
    int Fetch( string sSql ,ThirdpayList & lst);
    int RowToEntity(MYSQL_ROW row ,ThirdpayEntiy & entity);

};

#endif // ThirdpayDAL_H
