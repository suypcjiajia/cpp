#ifndef UserStateEntity_H
#define UserStateEntity_H

#include <string>
using namespace std;
//文件信息结构
typedef struct tsUserStateEntity
{
    string sPrimaryKey;     //主键
    long lUserId;           //用户ID（唯一、递增）
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    int iState;             //状态  0默认 333删除
    long lUpdateId;         //更新ID
    int iSync;              //同步状态

    string sVersion;        //版本
    double dGpsLon;         //经度
    double dGpsLat;         //纬度
    string sNickName;       //昵称
    int iSex;               //性别 0女 1男
    string sSignature;      //个性签名
    string sHeadImage;      //头像路径(小大头像路径自己分隔管理)
    long lDistance;         //距离
    long lLoginTime;
}UserStateEntity;

#endif // UserStateEntity_H
