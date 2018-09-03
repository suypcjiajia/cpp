#include "Token.h"
#include <stdio.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "TConvert.h"

MyAES Token::mMyaes;
byte Token::mKey[32] =  "ttdqwertyu";
int Token::mApplendLen = 8;

byte Token::mLeiduiKey[32] =  "meizhifu";
int Token::mLeiduApplendLen = 4;
//生成：登陆成功后的token
string Token::MakeLoginToken(const string & sOrg)
{
    mMyaes.SetKey(mKey,strlen((char*)mKey));
    return mMyaes.Encrypt(sOrg,mApplendLen);
}

//unpack登陆成功后的token
string Token::UnAesLoginToken(const string & sAesStr)
{
    mMyaes.SetKey(mKey,strlen((char*)mKey));
    return mMyaes.Decrypt(sAesStr,mApplendLen);
}


//unpack登陆成功后的token(乐兑商城的)
string Token::UnLeiduLoginToken(const string & sAesStr)
{

    mMyaes.SetKey(mLeiduiKey,strlen((char*)mLeiduiKey));
    return mMyaes.Decrypt(sAesStr,mLeiduApplendLen);
}
