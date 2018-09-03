#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <string>
#include <vector>
#include <map>
#include <tr1/unordered_map>
using namespace std;
typedef vector<string> StringLst;
typedef vector<long> LongLst;
typedef vector<long> longs;

struct CommonStruct{
    string v1;
    string v2;
    string v3;
    string v4;
};

typedef vector<CommonStruct> CommonStructVector;

//主键-哈唏表
typedef std::tr1::unordered_map<long,string> LongStringMap;
typedef std::tr1::unordered_map<long,long> LongLongMap;
typedef std::tr1::unordered_map<string,long> StringLongMap;
typedef std::map<string,long> StringLongStdMap;


static string err1 = "此账号违规操作，暂时冻结(如有疑问，请拨打客服电话400-0555-162)";
#endif
