//#include "WalletDayBookDAL.h"
//#include "dal/TMysqlDAL.h"
//#include "log/Logger.h"
//#include "stdlib.h"
//#include "unistd.h"
//#include "string.h"
//#include "TConvert.h"
//#include "TFile.h"
//#include "dal/MysqlConnect.h"
//#include "ErrDef.h"
//WalletDayBookDAL* WalletDayBookDAL::mInstance = NULL;
//WalletDayBookDAL* WalletDayBookDAL::GetInstance()
//{
//    if (mInstance == NULL){
//        mInstance = new WalletDayBookDAL;
//    }
//    return mInstance;
//}

////创建表
//int WalletDayBookDAL::CreateTable()
//{
//    ScopeLock lock( this->mMutex );
//    // SQL
//    string sSql = "create table if not exists "+gDayBookTb+" ("+
//            "sPrimaryKey VARCHAR(50) NOT NULL,"+
//            "lUserId BIGINT  NOT NULL,"+
//            "lAddTime BIGINT NOT NULL,"+
//            "lUpTime BIGINT NOT NULL,"+
//            "iState INT default 0,"+
//            "lUpdateId BIGINT default 0,"+
//            "iSync INT default 0,"+
//            "sAccount VARCHAR(50) default '' ,"+
//            "sPhone VARCHAR(24) default '' ,"+
//            "sSerialNum VARCHAR(50) UNIQUE NOT NULL,"+
//            "iMoneyType INT NOT NULL," +
//            "iStep INT NOT NULL," +
//            "sOrderNum VARCHAR(50)  NOT NULL,"+
//            "iDir INT NOT NULL ,"+
//            "lMoney BIGINT NOT NULL ,"+
//            "lPlatformId BIGINT  default 0 ,"+
//            "iDiscount INT NOT NULL,"+
//            "sQRcode VARCHAR(50) default '',"+
//            "sRemark VARCHAR(200)  default '',"+
//            "sBusinessRemark VARCHAR(200)  default '',"+
//            "PRIMARY KEY(sPrimaryKey) ,  UNIQUE(sOrderNum,iStep,iMoneyType)"+
//            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";
//    //    if ( 0 != TMysqlDAL::GetInstance()->Query( sSql ) ) {
//    //        return -1;
//    //    }
//    //    return 0;

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);
//}
////添加
//int WalletDayBookDAL::Insert( const WalletDayBookEntity& entity  )
//{
//    //ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    vector<string> QueryVct;
//    TMultiMysqlDAL tMultiMysqlDAL;
//    // SQL
//    string sSql = "insert into ";

//    sSql += gDayBookTb+"( sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync," +
//            "sAccount,sPhone,sSerialNum,sOrderNum,iDir,lMoney,iMoneyType,iDiscount,sQRcode,sRemark,iStep,lPlatformId,sBusinessRemark)" +
//            "VALUES( '"+entity.sPrimaryKey+"','" +
//            tConvert.LongToStr(entity.lUserId)+"','" +
//            tConvert.LongToStr(entity.lAddTime)+"','" +
//            tConvert.LongToStr(entity.lUpTime)+"','" +
//            tConvert.IntToStr(entity.iState)+"','" +
//            tConvert.LongToStr(entity.lUpdateId)+"','" +
//            "0','" +
//            entity.contentEntity.sAccount+"','" +
//            entity.contentEntity.sPhone+"','" +
//            entity.contentEntity.sSerialNum+"','"+
//            entity.contentEntity.sOrderNum+"','"+
//            tConvert.IntToStr(entity.contentEntity.iDir)+"','"+
//            tConvert.LongToStr(entity.contentEntity.lMoney)+"','"+
//            tConvert.IntToStr(entity.contentEntity.iMoneyType)+"','" +
//            tConvert.IntToStr(entity.contentEntity.iDiscount)+"','" +
//            entity.contentEntity.sQRcode+"','" +
//            tMultiMysqlDAL.RealEscapeString(entity.contentEntity.sRemark)+"','" +
//            tConvert.IntToStr(entity.contentEntity.iStep)+"','" +
//            tConvert.LongToStr(entity.contentEntity.lPlatformId)+"','"+
//            tMultiMysqlDAL.RealEscapeString(entity.contentEntity.sBusinessRemark)+
//            "')";
//    QueryVct.push_back(sSql);
//    if( 99 != entity.contentEntity.iStep ){
//        sSql = "update "+gAccountTb+" set "+
//                "lUpdateId = '" + tConvert.LongToStr(entity.lUpdateId) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(entity.lUpTime) + "' "+
//                ",iState = '" + tConvert.LongToStr(entity.iState) + "' ";
//        long lMoney = entity.contentEntity.lMoney;
//        if( entity.contentEntity.iDir == SERIALDIROUT){
//            lMoney = -lMoney;
//        }
//        if( entity.contentEntity.iMoneyType ==CASHTYPE ){
//            sSql += ",lCash =lCash+ '" + tConvert.LongToStr( lMoney ) + "' ";

//        }else  if( entity.contentEntity.iMoneyType ==BANDCASHTYPE ){
//            sSql += ",lBandCash =lBandCash+ '" + tConvert.LongToStr( lMoney ) + "' ";

//        }else  if( entity.contentEntity.iMoneyType ==SHOPTICKETTYPE ){
//            sSql += ",lShopTicket =lShopTicket+ '" + tConvert.LongToStr( lMoney ) + "' ";
//        }

//        sSql += "where lUserId = '"+tConvert.LongToStr(entity.lUserId)+"'";

//        QueryVct.push_back(sSql);
//    }

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}
////删除
//int WalletDayBookDAL::Delete( const string& sPrimaryKey, const long lUptTime,const long lUpdateId )
//{
//    //  ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    // SQL
//    string sSql = "update "+gDayBookTb+" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lUpdateId) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lUptTime) + "' "+
//            ",iState = '333' "+
//            ",iSync = '0'" ;
//    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";
//    //    if ( 0 != TMysqlDAL::GetInstance()->Query( sSql ) ) {
//    //        return -1;
//    //    }
//    //    return 0;
//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);
//}
////修改
//int WalletDayBookDAL::Update(  const string& sPrimaryKey,const WalletDayBookEntity& entity )
//{
//    //  ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    // SQL
//    string sSql = "update "+gDayBookTb+" set "+
//            "lUpdateId = '" + tConvert.LongToStr(entity.lUpdateId) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(entity.lUpTime) + "' "+
//            ",iState = '" + tConvert.LongToStr(entity.iState) + "' "+
//            ",iSync = '0'";
//    if (-1 != entity.contentEntity.iMoneyType ) {
//        sSql += ",iMoneyType = '" + tConvert.IntToStr(entity.contentEntity.iMoneyType) + "' ";
//    }
//    if (-1 != entity.contentEntity.iDir ) {
//        sSql += ",iDir = '" + tConvert.IntToStr(entity.contentEntity.iDir) + "' ";
//    }
//    if (-1 != entity.contentEntity.iDiscount ) {
//        sSql += ",iDiscount = '" + tConvert.IntToStr(entity.contentEntity.iDiscount) + "' ";
//    }
//    if (-1!= entity.contentEntity.lMoney ) {
//        sSql += ",lMoney = '" + tConvert.LongToStr(entity.contentEntity.lMoney) + "' ";
//    }
//    if (! entity.contentEntity.sAccount.empty() ) {
//        sSql += ",sAccount = '" + entity.contentEntity.sAccount+ "' ";
//    }
//    if ( !entity.contentEntity.sBusinessRemark.empty() ) {
//        sSql += ",sBusinessRemark = '" + TMysqlDAL::GetInstance()->RealEscapeString(entity.contentEntity.sBusinessRemark) + "' ";
//    }
//    if ( !entity.contentEntity.sSerialNum.empty() ) {
//        sSql += ",sSerialNum = '" + entity.contentEntity.sSerialNum + "' ";
//    }
//    if ( !entity.contentEntity.sOrderNum.empty() ) {
//        sSql += ",sOrderNum = '" + entity.contentEntity.sOrderNum + "' ";
//    }
//    if ( !entity.contentEntity.sPhone.empty() ) {
//        sSql += ",sPhone = '" + entity.contentEntity.sPhone + "' ";
//    }
//    if ( !entity.contentEntity.sQRcode.empty() ) {
//        sSql += ",sQRcode = '" + entity.contentEntity.sQRcode + "' ";
//    }
//    if ( !entity.contentEntity.sRemark.empty() ) {
//        sSql += ",sRemark = '" + TMysqlDAL::GetInstance()->RealEscapeString(entity.contentEntity.sRemark) + "' ";
//    }

//    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);
//}

////增加,批量
//int WalletDayBookDAL::InsertBatch( const WalletDayBookList & pLst ){
//    //ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    // SQL
//    string sSql = "replace into "+gDayBookTb+"( sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync," +
//            "sAccount,sPhone,sSerialNum,sOrderNum,iDir,lMoney,iMoneyType,iDiscount,sQRcode,sRemark,sBusinessRemark)"
//            "VALUES";
//    WalletDayBookList::const_iterator it = pLst.begin();
//    for ( ; it != pLst.end() ; it++){
//        const WalletDayBookEntity & entity = *it;
//        sSql +="('" +entity.sPrimaryKey+"','" +
//                tConvert.LongToStr(entity.lUserId)+"','" +
//                tConvert.LongToStr(entity.lAddTime)+"','" +
//                tConvert.LongToStr(entity.lUpTime)+"','" +
//                tConvert.IntToStr(entity.iState)+"','" +
//                tConvert.LongToStr(entity.lUpdateId)+"','" +
//                "0','" +
//                entity.contentEntity.sAccount+"','" +
//                entity.contentEntity.sPhone+"','" +
//                entity.contentEntity.sPhone+"','" +
//                entity.contentEntity.sSerialNum+"','"+
//                entity.contentEntity.sOrderNum+"','"+
//                tConvert.IntToStr(entity.contentEntity.iDir)+"','"+
//                tConvert.LongToStr(entity.contentEntity.lMoney)+"','"+
//                tConvert.IntToStr(entity.contentEntity.iMoneyType)+"','" +
//                tConvert.IntToStr(entity.contentEntity.iDiscount)+"','" +
//                entity.contentEntity.sQRcode+"','" +
//                entity.contentEntity.sRemark+"','" +
//                entity.contentEntity.sBusinessRemark+
//                "')";
//        sSql += ",";
//    }
//    sSql = sSql.substr(0,sSql.length() -1);


//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);

//}

////获取单个（根据订单号)
//int WalletDayBookDAL::GetEntityByOrderNumStepMoneyType(const string & psOrderNum ,const int & piStep,const int & piMoneyType,WalletDayBookEntity & pOut)
//{

//    TConvert tConvert;

//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = NULL;

//    // SQL
//    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,"
//                  "sAccount,sPhone,sSerialNum,sOrderNum,iDir,lMoney,iMoneyType,iDiscount,sQRcode,sRemark,sBusinessRemark"
//                  " from "+gDayBookTb;
//    sSql+=" where sOrderNum='"+psOrderNum+"' and iStep="+tConvert.IntToStr(piStep) + " and iMoneyType=" + tConvert.IntToStr(piMoneyType);
//    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    result =  con->QueryResult(sSql);

//    if ( NULL != result ) {
//        iRet = ERRNOTDATA;
//        MYSQL_ROW row = NULL;
//        row = mysql_fetch_row( result );
//        if ( NULL != row ) {
//            int iIndex = 0;
//            pOut.sPrimaryKey =row[iIndex++];
//            pOut.lUserId = atol( row[iIndex++] );
//            pOut.lAddTime = atol( row[iIndex++] );
//            pOut.lUpTime = atol( row[iIndex++] );
//            pOut.iState = atoi( row[iIndex++] );
//            pOut.lUpdateId = atol( row[iIndex++] );
//            pOut.contentEntity.sAccount = row[iIndex++];
//            pOut.contentEntity.sPhone = row[iIndex++];
//            pOut.contentEntity.sSerialNum = row[iIndex++];
//            pOut.contentEntity.sOrderNum = row[iIndex++];
//            pOut.contentEntity.iDir = atoi( row[iIndex++] );
//            pOut.contentEntity.lMoney = atol( row[iIndex++] );
//            pOut.contentEntity.iMoneyType = atoi( row[iIndex++] );
//            pOut.contentEntity.iDiscount = atoi( row[iIndex++] );
//            pOut.contentEntity.sQRcode = row[iIndex++] ;
//            pOut.contentEntity.sRemark =  row[iIndex++] ;
//            pOut.contentEntity.sBusinessRemark =  row[iIndex++] ;
//            iRet = 0;
//        }
//    }
//    // 释放内存
//    // TMysqlDAL::GetInstance()->FreeResult( result );
//    con->FreeResult( result );

//    return iRet;

//}


//int WalletDayBookDAL::GetEntityBySerialNum(const string & psSerialNum,WalletDayBookEntity & pOut)
//{
//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = NULL;

//    // SQL
//    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,"
//                  "sAccount,sPhone,sSerialNum,sOrderNum,iDir,lMoney,iMoneyType,iDiscount,sQRcode,sRemark,sBusinessRemark"
//                  " from "+gDayBookTb;
//    sSql+=" where iState!=333 and sSerialNum='"+psSerialNum+"'";
//    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    result =  con->QueryResult(sSql);

//    if ( NULL != result ) {
//        iRet = ERRNOTDATA;
//        MYSQL_ROW row = NULL;
//        row = mysql_fetch_row( result );
//        if ( NULL != row ) {
//            int iIndex = 0;
//            pOut.sPrimaryKey =row[iIndex++];
//            pOut.lUserId = atol( row[iIndex++] );
//            pOut.lAddTime = atol( row[iIndex++] );
//            pOut.lUpTime = atol( row[iIndex++] );
//            pOut.iState = atoi( row[iIndex++] );
//            pOut.lUpdateId = atol( row[iIndex++] );
//            pOut.contentEntity.sAccount = row[iIndex++];
//            pOut.contentEntity.sPhone = row[iIndex++];
//            pOut.contentEntity.sSerialNum = row[iIndex++];
//            pOut.contentEntity.sOrderNum = row[iIndex++];
//            pOut.contentEntity.iDir = atoi( row[iIndex++] );
//            pOut.contentEntity.lMoney = atol( row[iIndex++] );
//            pOut.contentEntity.iMoneyType = atoi( row[iIndex++] );
//            pOut.contentEntity.iDiscount = atoi( row[iIndex++] );
//            pOut.contentEntity.sQRcode = row[iIndex++] ;
//            pOut.contentEntity.sRemark =  row[iIndex++] ;
//            pOut.contentEntity.sBusinessRemark =  row[iIndex++] ;
//            iRet = 0;
//        }
//    }
//    // 释放内存
//    con->FreeResult( result );

//    return iRet;

//}

//int WalletDayBookDAL::GetListByOrderNumStep( const string & psOrderNum ,const int & piStep, WalletDayBookList & plstOut )
//{
//    TConvert tConvert;
//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = NULL;

//    // SQL
//    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
//                  "sAccount,sPhone,sSerialNum,sOrderNum,iDir,lMoney,iMoneyType,iDiscount,sQRcode,sRemark,sBusinessRemark,lPlatformId"
//                  " from "+gDayBookTb ;

//    sSql+=" where iState!=333 and  sOrderNum='"+psOrderNum+"' and iStep="+tConvert.IntToStr(piStep);

//    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    result =  con->QueryResult(sSql);
//    MYSQL_ROW row = NULL;
//    if( result != NULL){
//        iRet = ERRNOTDATA;

//        while( NULL != (row  =  mysql_fetch_row( result ))) {
//            iRet = 0;
//            WalletDayBookEntity entity;
//            int iIndex = 0;

//            entity.sPrimaryKey =row[iIndex++];
//            entity.lUserId = atol( row[iIndex++] );
//            entity.lAddTime = atol( row[iIndex++] );
//            entity.lUpTime = atol( row[iIndex++] );
//            entity.iState = atoi( row[iIndex++] );
//            entity.lUpdateId = atol( row[iIndex++] );
//            iIndex++;
//            entity.contentEntity.sAccount = row[iIndex++];
//            entity.contentEntity.sPhone = row[iIndex++];
//            entity.contentEntity.sSerialNum = row[iIndex++];
//            entity.contentEntity.sOrderNum = row[iIndex++];
//            entity.contentEntity.iDir = atoi( row[iIndex++] );
//            entity.contentEntity.lMoney = atol( row[iIndex++] );
//            entity.contentEntity.iMoneyType = atoi( row[iIndex++] );
//            entity.contentEntity.iDiscount = atoi( row[iIndex++] );
//            entity.contentEntity.sQRcode = row[iIndex++] ;
//            entity.contentEntity.sRemark =  row[iIndex++] ;
//            entity.contentEntity.sBusinessRemark =  row[iIndex++] ;
//            entity.contentEntity.lPlatformId =  atol( row[iIndex++] );
//            plstOut.push_back(entity);

//        }
//    }
//    // 释放内存
//    con->FreeResult( result );
//    return iRet;
//}

//int WalletDayBookDAL::GetList(const WalletDayBookQueryEntity & psQuery, int piIndex, int piCount,WalletDayBookList & plstOut, int & piTotalCount)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = NULL;

//    // SQL
//    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,"
//                  "sAccount,sPhone,sSerialNum,sOrderNum,iDir,lMoney,iMoneyType,iDiscount,sQRcode,sRemark,sBusinessRemark,iStep"
//                  " from "+gDayBookTb ;
//    string sCond = " where iState!=333 and";
//    if( psQuery.lUserId > 0){
//        sCond += " lUserId ='" + tConvert.LongToStr( psQuery.lUserId ) + "' and";
//    }
//    if( psQuery.iMoneyType > 0){
//        sCond += " iMoneyType ='" + tConvert.LongToStr( psQuery.iMoneyType ) + "' and";
//    }
//    if( psQuery.iStep > 0){
//        sCond += " iStep ='" + tConvert.LongToStr( psQuery.iStep ) + "' and";
//    }
//    if( !psQuery.sOrderNum.empty()){
//        sCond += " sOrderNum ='" +  psQuery.sOrderNum  + "' and";
//    }
//    sCond = sCond.substr(0,sCond.length() - 3);
//    sSql += sCond;
//    sSql += " limit  " + tConvert.IntToStr(piIndex) + "," + tConvert.IntToStr(piCount);

//    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    result =  con->QueryResult(sSql);


//    MYSQL_ROW row = NULL;

//    if( result != NULL){
//        iRet = 0;

//        while( NULL != (row  =  mysql_fetch_row( result ))) {

//            WalletDayBookEntity entity;
//            int iIndex = 0;

//            entity.sPrimaryKey =row[iIndex++];
//            entity.lUserId = atol( row[iIndex++] );
//            entity.lAddTime = atol( row[iIndex++] );
//            entity.lUpTime = atol( row[iIndex++] );
//            entity.iState = atoi( row[iIndex++] );
//            entity.lUpdateId = atol( row[iIndex++] );
//            entity.contentEntity.sAccount = row[iIndex++];
//            entity.contentEntity.sPhone = row[iIndex++];
//            entity.contentEntity.sSerialNum = row[iIndex++];
//            entity.contentEntity.sOrderNum = row[iIndex++];
//            entity.contentEntity.iDir = atoi( row[iIndex++] );
//            entity.contentEntity.lMoney = atol( row[iIndex++] );
//            entity.contentEntity.iMoneyType = atoi( row[iIndex++] );
//            entity.contentEntity.iDiscount = atoi( row[iIndex++] );
//            entity.contentEntity.sQRcode = row[iIndex++] ;
//            entity.contentEntity.sRemark =  row[iIndex++] ;
//            entity.contentEntity.sBusinessRemark =  row[iIndex++] ;
//            entity.contentEntity.iStep = atoi( row[iIndex++] );
//            plstOut.push_back(entity);
//        }

//        sSql = "select count(*)  from " + gDayBookTb ;
//        sSql  +=  sCond;

//        GetQueryTotalCount(sSql,piTotalCount);
//    }
//    // 释放内存

//    con->FreeResult( result );

//    return iRet;
//}

//int WalletDayBookDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
//{
//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = NULL;
//    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    result =  con->QueryResult(psQuery);

//    MYSQL_ROW row = NULL;

//    if( result != NULL){
//        iRet = 0;
//        piCount = 0;
//        if ( NULL != (row = mysql_fetch_row(result) )){
//            piCount = atoi(row[0]);
//        }
//    }
//    return iRet;
//}


////事务处理（只能是增册改的操作）
//int WalletDayBookDAL::Transaction( TMultiMysqlDAL * con,const vector<string>  & pQueryLst)
//{
//    //事务处理
//    con->AutoCommit(false);
//    int iIndex = 0;
//    int iRet = -1;
//    while( iIndex < pQueryLst.size()){
//        iRet =  con->Query(pQueryLst[iIndex++]);
//        if(iRet != 0){
//            break;
//        }
//    }
//    if( iRet != 0){
//        con->RollBack();
//    }else{
//        con->Commit();
//    }
//    con->AutoCommit(true);
//    return iRet;
//}
