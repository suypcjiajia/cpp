#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>
#include "MyAES.h"
using namespace std;
class Token
{
public:
    //生成：登陆成功后的token
    static string MakeLoginToken(const string & sOrg);
    //unpack登陆成功后的token
    static string UnAesLoginToken(const string & sAesStr);
    //unpack登陆成功后的token(乐兑商城的)
    static string UnLeiduLoginToken(const string & sAesStr);
    static MyAES mMyaes;
    static byte mKey[32];
    static int mApplendLen;

    static byte mLeiduiKey[32];//乐兑商城 key
    static int mLeiduApplendLen;//乐兑商城 applend len

};

#endif // TOKEN_H
