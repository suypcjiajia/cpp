#ifndef SHOWDAL_H
#define SHOWDAL_H
#include "dal/TMysqlDAL.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"
#include "define.h"
#include "dal/Basedal.h"


class ShowDAL : public BaseDal
{
public:
    ShowDAL(){
        msTableName = "showpro";
        msAllColumn = " id,lPlatformId,lAddTime,sPicLink,sName,dPrice,dCoin,iCoinType,iVerify,iState,sContent,sExpireTime,sCanot,sUtime,sLaw,sDetailPics,sSpecification ";
    }

    int Add( const ShowPro & entity );
    int IsExist( long id);
    //查询数据
    int GetData(  long  lPlatformId ,int iPage,int iCount,int iVerify,int iState, string sName,ShowProLst & lst,int & iTotalCount);
    int GetData(const QueryShowPro & query,int iPage, int iCount,ShowProLst & lst,int & iTotalCount);
    int UpVerify(long id, int iValue );
    int Del(long id);
    int Update(long id, string sPicLink,string sName ,double dPrice, double coin,
               int coinType, string canot,string content, string detailPics,string expireTime, string utime
               ,string law);


    string GetSqlIncreemSallNum(long id ,int iSallNum);
    int IncreemSallNum(long id ,int iSallNum);

    int GetEntity(long id ,ShowPro & entity);

    //首页商品轮播
    int GetLoop(string town,string area,ShowProLst & shows);

    //商品分类数量
    int TypeInfo(string town,string area,StringLongStdMap & typeinfo);

    //商品分区分类分页
    int GetPageByType(string town,string area,double gpsLat,double gpsLon,int page, string type,int sort,ShowProLst & shows);



private:

    void RowToEntity(MYSQL_ROW row , ShowPro & entity);

    TMutex mMutex;//线程锁
};

#endif // SHOWDAL_H
