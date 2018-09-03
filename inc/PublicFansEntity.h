#ifndef PUBLICFANSENTITY_H
#define PUBLICFANSENTITY_H

#include <string>
#include <vector>
using namespace std;

#include <tr1/unordered_map>


struct PublicFansEntity
{
    long Id;           //公众号Id
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    int iType ;
    long lFansId ;
    int iCount ;

};

typedef vector<PublicFansEntity> PublicFansList;

struct PublicFansQueryEntity
{
};




#endif // PUBLICFANSENTITY_H
