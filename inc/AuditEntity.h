#ifndef AUDITENTITY_H
#define AUDITENTITY_H

#include <string>
#include <vector>
using namespace std;

struct IfAuditEntity
{
    long lUserId;           //调用者Id
    long lAddTime;          //第一次调用的时间
    long lUpTime;           //最后一次调用的时间
    int iCount;             //调用次数
    string sInterface; //调用的接口
};

typedef vector<IfAuditEntity> IfAuditList;

struct IfAuditQueryEntity
{
};


#endif // AUDITENTITY_H
