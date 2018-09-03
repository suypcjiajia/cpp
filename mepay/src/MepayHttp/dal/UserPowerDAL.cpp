#include "UserPowerDAL.h"





int UserPowerDAL::Add( const UserPower & entity  )
{
    string sSql   = "insert into " +  msTableName+"(" + msAllColumn  + ")" +
            "VALUES( '"+mtConvert.IntToStr( entity.power)+"','" +
            mtConvert.LongToStr(entity.idPlatform)+"','" +
            mtConvert.LongToStr(entity.userId)+"','" +
            mMulti.RealEscapeString(entity.account)+"','" +
            mMulti.RealEscapeString(entity.nickName)+"','" +
            mMulti.RealEscapeString(entity.headImage)+
            "')";

    return BaseQuery(sSql);
}



int UserPowerDAL::Del(long idPlatform,long userId,int power)
{

    string sSql = "delete from " + msTableName + " where idPlatform=" + mtConvert.LongToStr(idPlatform)+
            " and userId=" + mtConvert.LongToStr(userId) + " and power=" + mtConvert.IntToStr(power);

    return BaseQuery(sSql);

}

int UserPowerDAL::IsExist(long idPlatform,long userId,int power)
{

    string sWhere = " where idPlatform=" + mtConvert.LongToStr(idPlatform)+
            " and userId=" + mtConvert.LongToStr(userId) + " and power=" + mtConvert.IntToStr(power);

    return BaseDal::IsExist(sWhere);


}

//查询数据
int UserPowerDAL::GetData( long idPlatform, int iPower, int iPage,int iCount,long userId,string account, string nickName,UserPowerLst & lst,int & iTotalCount)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL multimysql;

    int iIndex = (iPage -1 )*iCount;

    string sCondition;

    string sSql = "select " + msAllColumn +  " from "+msTableName ;
    if( -1 != userId){
      sCondition += " userId like '%" + tConvert.LongToStr(userId) + "%' and";
    }
    if( -1 != idPlatform){
      sCondition += " idPlatform=" + tConvert.LongToStr(idPlatform) + " and";
    }
    if( !account.empty()){
      sCondition += " account like '%" +multimysql.RealEscapeString(account) + "%' and";
    }
    if( ! nickName.empty()){
        sCondition += " nickName like '%" + multimysql.RealEscapeString(nickName) + "%' and";
    }
    if( iPower != -1){
        sCondition += " power=" + tConvert.IntToStr(iPower) + " and";
    }

    if( !sCondition.empty()){
        sCondition = sCondition.substr(0,sCondition.length() - 3);
    }
    if( !sCondition.empty()){
        sCondition = " where " + sCondition;
        sSql +=  sCondition;
    }
    sSql += " order by id desc limit " +  tConvert.IntToStr(iIndex) + "," + tConvert.IntToStr(iCount);


    BaseQueryResult(sSql,&result);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {
            UserPower  entity;
            RowToEntity(row,entity);

            lst.push_back(entity);

        }
    }

    GetCount(sCondition,iTotalCount);

    // 释放内存
    FreeResult( result );
    return iRet;

}

int UserPowerDAL::GetEntity(long idPlatform,long userId,int power,UserPower & entity)
{
    int iRet = -1;
    string sWhere = " where idPlatform=" + mtConvert.LongToStr(idPlatform)+
            " and userId=" + mtConvert.LongToStr(userId) + " and power=" + mtConvert.IntToStr(power);
    string sSql = "select " + msAllColumn  +  " from "+msTableName  + sWhere;

    MYSQL_RES* result;

    MYSQL_ROW row = NULL;
    BaseQueryResult(sSql, &result);

    if( result != NULL){



        if ( NULL != (row = mysql_fetch_row(result) )) {

            iRet = 0;

            RowToEntity(row,entity);
        }
    }

    FreeResult( result );
    return iRet;

}

void UserPowerDAL::RowToEntity(MYSQL_ROW row , UserPower & entity)
{
    int iIndex = 0;

    entity.power =  atoi(row[iIndex++]);
    entity.idPlatform =  atol(row[iIndex++]);
    entity.userId = atol(row[iIndex++]);
    entity.account = row[iIndex++];
    entity.nickName = row[iIndex++];
    entity.headImage = row[iIndex++];

}

