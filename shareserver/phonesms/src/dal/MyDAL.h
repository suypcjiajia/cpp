#ifndef USERSETDAL_H
#define USERSETDAL_H

#include <string>
#include <vector>
#include "UserSetEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class MyDAL
{

public:

    int CreateTable();


    int AddData( const string ip);

    int GetEntity(const string ip, int & count,string & first ,string & last);

    int UpFirstTime(const string ip);

    int IncreemOne( const string ip);

private:
    const string msTableName = "phonesms";

};

#endif // USERSETDAL_H
