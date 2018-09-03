#ifndef PRIMARY_KEY_DAL_H
#define PRIMARY_KEY_DAL_H

#include <string>
#include <vector>
#include <deque>
#include <hash_map>
#include "TMutex.hpp"
#include "TypeDef.h"
#include "dal/TMysqlDAL.h"
#include <bll/ShopInfoBLL.h>
#include "dal/Basedal.h"
using namespace std;


class ShopInfoDAL:public BaseDal
{
    // 定义单键
public:  static ShopInfoDAL* GetInstance();
private: static ShopInfoDAL* mInstance;
private: TMutex mMutex;//线程锁
public:


    //增加
    int Insert(const ShopInfoEntity& entity );
    //删除
    int Delete( const string& psPrimaryKey, const long lUptTime,const long lUpdateId );
    //修改
    int Update( const string& psPrimaryKey,const ShopInfoEntity& pEntity );
    //根据平台号或帐号或用户ID获取商家信息
    int GetEntity(const string & pCond,int piType ,ShopInfoEntity &  entity);
    //查询数据
    int GetList( const ShopQueryEntity & pQueryEntity , int piIndex, int piCount, ShopInfoList & plstOut, int & piTotalCount);
    //查询数据
    int GetData( const ShopQueryEntity & pQueryEntity , int piIndex, int piCount, ShopInfoList & plstOut,  int & piTotalCount);

    //
    int GetMyShop( const long & plUserId ,int iType, ShopInfoList & plstOut);

    //更新密码
    int ModifyPwd(const string & psPrimaryKey,const string & psNewPwd, const long & plUpdateId);
    int UpRemark( const long plPlatformId,const string & psRemark,const long & plUpdateId);
    //更新折扣
    int UpDiscount( const long plPlatformId,const int & piDiscount,const string & psRemark,const long & plUpdateId);
    //更新商家平均分
    int UpAvgScore(long lPlatformId,int avg);
    //获取最大平台号
    int GetMaxPlatformId(long & plPlatformId);
    //获取一个地区下的最大帐号
    int GetMaxCityAccount( const string &  psCity, string & psAccount);
    //更换手机号
    int UpdatePhone(long lPlatfromId, const string & psNewPhone, long lNewUserId);
    //获取开的店
    int Shops( const long & plUserId ,int iType, ShopInfoList & plstOut);
    //获取个代开的供应商
    int OpenByPerAgent( const long & plUserId , ShopInfoList & plstOut);

    int GetState(long lPlatfromId, int & iState);

    int GetCashBackShops(int iPage, int iCount,CashBackShopLst & lst);

    //推荐的商家优先显示，接下来是符合所在地址并按距离排序显示
    int RecommendShops(const ShopQueryEntity & pQueryEntity,int iPage, int iCount, ShopInfoList & lst,int & iTotalCount);

    int GetPlatformIdByShopName(string sShopName,long & lPlatformId);

    int CityShopCount(long ,CommonStructVector &);

    int GetEmptyGpsZone(ShopInfoList & lst);
    int UpdateGpsZone(const string & sPlatfromId, const string & sZone);

    int SearchShowPro();

    //特殊促销配置
    int PromotionConfig(  const string& sPlatformId,int iCoinPersend ,int iPromotionDiscount);

    //获取属于我授权商店(收款记录)
    int MyImpowerShop(long userId,ShopInfoList & lst);

private:


};

#endif // PRIMARY_KEY_DAL_H
