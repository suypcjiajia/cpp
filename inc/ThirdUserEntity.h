#ifndef ThirdUserEntityENTITY_H
#define ThirdUserEntityENTITY_H

#include <string>
#include <vector>
using namespace std;

struct ThirdUserEntity
{
    string addTime;          //添加时间
    int sex;
    string headImage;
    long pushManId;
    long platformId;
    string nickName;
    string id;
    int type;
};

typedef vector<ThirdUserEntity> ThirdUserLst;



#endif // ThirdUserEntity
