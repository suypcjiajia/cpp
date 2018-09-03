#ifndef FriendResquestEntity_H
#define FriendResquestEntity_H

#include <string>
#include <vector>
using namespace std;
//文件信息结构
typedef struct tsFriendResquestEntity
{
    long lUserId;           //用户ID（唯一、递增）
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    int iState;             //状态  0默认 333删除
    long lToUserId;         //对方用户ID
    int iAction;            //类型： 1   申请 2 同意 3 不同意
    string sRemark; //请求说明
}FriendResquestEntity;

typedef vector<FriendResquestEntity> FriendResquestList;

struct FriendResquestQueryEntity
{
    long lUserId;
    long lToUserId;
    long lUpdateId;
    int iAction;
};


enum EnumFriResqState
{
    enum_friresq_default = 0,
    enum_friresq_resq = 1,//申请
    enum_friresq_agree = 2,//同意
    enum_friresq_disagree = 3 //不同意
};

#endif // FriendResquestEntity_H
