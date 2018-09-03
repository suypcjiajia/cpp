#ifndef ThirdpayEntiy_H
#define ThirdpayEntiy_H

#include <string>
#include <vector>
using namespace std;


struct ThirdpayEntiy
{
    string id ;// '唯一标记（unix秒+6位随机数)',
    string openid ;// '第三方身份标记',
    string phone ;// '关联手机号',
    long platformId ;//'平台号',
    long pushId ;// '推广人',
    string addTime ;// '时间',
    double money ;// '金额',
    int thirdType ;// '第三方类型',
};
typedef vector<ThirdpayEntiy> ThirdpayList;

#endif // ThirdpayEntiy_H
