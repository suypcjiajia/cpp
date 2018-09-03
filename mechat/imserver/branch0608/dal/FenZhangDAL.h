#ifndef FENZHANGDAL_H
#define FENZHANGDAL_H

#include <string>
#include <vector>
#include "dal/Basedal.h"
#include "FenZhangEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class FenZhangDAL:public BaseDal
{

public:
    FenZhangDAL(){
        msTableName = "fenzhang";
        msAllColumn = " orderNo,addTime,info ";
    }

    int Insert( FenZhangEntiy& entity );

private:
    int Fetch( string sSql ,FenZhangList & lst);
    int RowToEntity(MYSQL_ROW row ,FenZhangEntiy & entity);

};

#endif // FENZHANGDAL_H
