#ifndef ThirdUserDAL_H
#define ThirdUserDAL_H

#include <string>
#include <vector>
#include "dal/Basedal.h"
#include "ThirdUserEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class ThirdUserDAL:public BaseDal
{

public:
    ThirdUserDAL(){
        msTableName = "third_user";
        msAllColumn = " addTime,sex,headImage,pushManId,platformId,nickName,id,type ";
    }

    int Insert( ThirdUserEntity& entity );
    int GetById(string id ,ThirdUserEntity & entity);


private:
    int Fetch( string sSql ,ThirdUserLst & lst);
    int RowToEntity(MYSQL_ROW row ,ThirdUserEntity & entity);

};

#endif // ThirdUserDAL
