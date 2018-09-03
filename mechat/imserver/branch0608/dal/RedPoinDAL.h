#ifndef REDPOINDAL_H
#define REDPOINDAL_H


/*
 *系统通知mysql访问层
 * */

#include <string>
#include <vector>

using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class RedPointDAL
{

public:

    int CreateTable();

    int Replace(long lUserId, long lLastTime,int iType,const string &  sMark );

    long GetLastTime(long lUserId, int iType,const string &  sMark);

private:
    const string msTableName = "redpoint";


};

#endif // REDPOINDAL_H
