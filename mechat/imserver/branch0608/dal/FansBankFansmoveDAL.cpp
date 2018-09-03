#include "FansBankFansmoveDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "param/Param.h"


FansBankFansmoveDAL::FansBankFansmoveDAL()
{
    msAllColumn = "select orderNo,fans from ";
    msTableName = "fansbank_fansmove";
}

int FansBankFansmoveDAL::Add( string sOrder, string sFans    )
{
    string  sSql = GetSqlAdd(sOrder,sFans);
    return BaseQuery(sSql);

}

string FansBankFansmoveDAL::GetSqlAdd( string sOrder, string sFans   )
{
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( orderNo,fans)" +
            "VALUES( '"+
            mMulti.RealEscapeString( sOrder )+"','" +
             sFans +
            "')";

    return sSql;

}

