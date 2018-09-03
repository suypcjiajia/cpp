#ifndef FENZHANGEntiy_H
#define FENZHANGEntiy_H

#include <string>
#include <vector>
using namespace std;


struct FenZhangEntiy
{
    string orderNo ;// 订单号
    string addTime ;// 添加时间
    string info;//分帐记录
};
typedef vector<FenZhangEntiy> FenZhangList;

#endif // FENZHANGEntiy_H
