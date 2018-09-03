//#include "WalletAccountDAL.h"
//#include "WalletDayBookDAL.h"
//#include "PersonalAgentDAL.h"
//#include "dal/TMysqlDAL.h"
//#include "log/Logger.h"
//#include "stdlib.h"
//#include "unistd.h"
//#include "string.h"
//#include "TConvert.h"
//#include "TFile.h"
//#include "bll/TBLL.h"
//#include "ShopInfoDAL.h"
//#include "ZoneAgentDAL.h"
//#include "PersonalAgentDAL.h"
//#include "ErrDef.h"
//WalletAccountDAL* WalletAccountDAL::mInstance = NULL;
//WalletAccountDAL* WalletAccountDAL::GetInstance()
//{
//    if (mInstance == NULL){
//        mInstance = new WalletAccountDAL;
//    }
//    return mInstance;
//}

////创建表
//int WalletAccountDAL::CreateTable()
//{
//    //  ScopeLock lock( this->mMutex );
//    // SQL
//    string sSql = "create table if not exists "+gAccountTb+" ("+
//            "sPrimaryKey VARCHAR(50) NOT NULL,"+
//            "lUserId BIGINT UNIQUE NOT NULL,"+
//            "lAddTime BIGINT NOT NULL,"+
//            "lUpTime BIGINT NOT NULL,"+
//            "iState INT default 0,"+
//            "lUpdateId BIGINT default 0,"+
//            "iSync INT default 0,"+
//            "sAccount VARCHAR(50) UNIQUE NOT NULL,"+
//            "sPhone VARCHAR(24) default '' ,"+
//            "sPassWord VARCHAR(50) NOT NULL,"+
//            "lCash BIGINT  default 0,"+
//            "lBandCash BIGINT  default 0,"+
//            "lShopTicket BIGINT  default 0,"+
//            "sAddress VARCHAR(2000)  default '',"+
//            "sBankCard VARCHAR(2000)  default '',"+
//            "lPushId BIGINT  default 0,"+
//            "lPlatformId BIGINT  default 0," +
//            "sQuestion VARCHAR(100)  default '',"+
//            "sAnswer VARCHAR(100)  default '',"+
//            "PRIMARY KEY(sPrimaryKey)"+
//            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);
//}
////现金充值
//int WalletAccountDAL::InCash( const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    WalletAccountEntity accountEntity;
//    iRet = GetEntityByUseId(lUserId,accountEntity);
//    if( iRet != 0){
//        return iRet;
//    }
//    // SQL
//    vector<string> QueryVct;

//    //增加现金
//    string sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash+ '" + tConvert.LongToStr(lValue) + "' " ;
//    sSql += "where lUserId = '"+tConvert.LongToStr(lUserId)+"'";
//    QueryVct.push_back(sSql);

//    //构建流水
//    string sRemark = "【钱包】现金充值【银行卡-";
//    sRemark += sCard + "】";
//    sSql = MakeSqlInsertDayBook(lUserId,lValue,0,sOrderNum,SERIALDIRIN,CASHTYPE,STEPINMONEY,0,"",sRemark,"");
//    QueryVct.push_back(sSql);

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return iRet;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

////绑定现金充值
//int WalletAccountDAL::InBandCash(const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    WalletAccountEntity accountEntity;
//    iRet = GetEntityByUseId(lUserId,accountEntity);
//    if( iRet != 0){
//        return iRet;
//    }
//    // SQL
//    vector<string> QueryVct;

//    //增加绑定现金,增加50%购物币
//    string sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lBandCash =lBandCash+ '" + tConvert.LongToStr(lValue) + "' " +
//            ",lShopTicket =lShopTicket+ '" + tConvert.LongToStr(lValue/2) + "' " ;
//    sSql += " where lUserId = '"+tConvert.LongToStr(lUserId)+"'";
//    QueryVct.push_back(sSql);


//    //构建流水
//    string sRemark = "【钱包】绑定现金充值【银行卡-";
//    sRemark += sCard + "】";
//    sSql = MakeSqlInsertDayBook(lUserId,lValue,0,sOrderNum,SERIALDIRIN,BANDCASHTYPE,STEPINMONEY,0,"",sRemark,"");
//    QueryVct.push_back(sSql);

//    sRemark = "【钱包】送购物币【银行卡-";
//    sRemark += sCard + "】";
//    sSql = MakeSqlInsertDayBook(lUserId,lValue/2,0,sOrderNum,SERIALDIRIN,SHOPTICKETTYPE,STEPGIVESHOPTICKET,0,"",sRemark,"");//送购物币流水
//    QueryVct.push_back(sSql);


//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

////购物币充值
//int WalletAccountDAL::InShopTicket(const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    WalletAccountEntity accountEntity;
//    iRet = GetEntityByUseId(lUserId,accountEntity);
//    if( iRet != 0){
//        return iRet;
//    }
//    // SQL
//    vector<string> QueryVct;

//    //增加购物币值
//    string sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lShopTicket =lShopTicket+ '" + tConvert.LongToStr(lValue) + "' " ;
//    sSql += "where lUserId = '"+tConvert.LongToStr(lUserId)+"'";
//    QueryVct.push_back(sSql);

//    //构建流水
//    string sRemark = "【钱包】购物币充值【银行卡-";
//    sRemark += sCard + "】";
//    sSql = MakeSqlInsertDayBook(lUserId,lValue,0,sOrderNum,SERIALDIRIN,SHOPTICKETTYPE, STEPINMONEY,0,"",sRemark,"");
//    QueryVct.push_back(sSql);

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);

//}

////现金提现
//int WalletAccountDAL::OutCash(const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    vector<string> QueryVct;

//    //减少现金
//    string sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash-'" + tConvert.LongToStr(lValue) + "' " ;
//    sSql += "where lUserId = '"+tConvert.LongToStr(lUserId)+"'";
//    QueryVct.push_back(sSql);

//    //构建流水
//    string sRemark = "【钱包】提现【银行卡-";
//    sRemark += sCard + "】";
//    sSql = MakeSqlInsertDayBook(lUserId,lValue,0,sOrderNum,SERIALDIRIN,CASHTYPE,STEPOUTCASH,0,"", sRemark, "");
//    QueryVct.push_back(sSql);

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);

//}

////现金提现退回
//int WalletAccountDAL::BackCash(const long & lUserId, const string & sRemark,const string & sOrderNum)
//{

//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    // SQL

//    //查找提现流水
//    WalletDayBookEntity entityDayBook;
//    iRet = WalletDayBookDAL::GetInstance()->GetEntityByOrderNumStepMoneyType(sOrderNum,STEPOUTCASH, CASHTYPE,entityDayBook);
//    if( 0 != iRet){
//        return iRet;
//    }

//    WalletAccountEntity accountEntity;
//    iRet = GetEntityByUseId(lUserId,accountEntity);
//    if( iRet != 0){
//        return iRet;
//    }

//    vector<string> QueryVct;

//    //退回现金
//    string sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash+ '" + tConvert.LongToStr(entityDayBook.contentEntity.lMoney) + "' " ;
//    sSql += "where lUserId = '"+tConvert.LongToStr(lUserId)+"'";
//    QueryVct.push_back(sSql);

//    //构建流水
//    string sMyRemark = "【钱包】提现失败【";
//    sMyRemark += sRemark + "】";
//    sSql = MakeSqlInsertDayBook(lUserId,entityDayBook.contentEntity.lMoney,0,sOrderNum,SERIALDIRIN,CASHTYPE,STEPOUTCASHFAIL,0,"",sMyRemark,"");
//    QueryVct.push_back(sSql);

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);

//}

////第三方支付
//int WalletAccountDAL::ThirdPartyPay(const long & lUserId,const long & lValue, const long & lPlatformId,const string & sRemark,const string & sOrderNum)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    // SQL
//    //帐号余额不变化
//    vector<string> QueryVct;

//    // 构建流水
//    string sMyRemark = "【第三方支付】";
//    string sSql = MakeSqlInsertDayBook(lUserId,lValue,0,sOrderNum,SERIALDIRIN,CASHTYPE,STEPINMONEY,lPlatformId,"",sRemark,"");//现金充值流水
//    QueryVct.push_back(sSql);

//    sSql = MakeSqlInsertDayBook(lUserId,lValue,0,sOrderNum,SERIALDIROUT,CASHTYPE, STEPBUY,lPlatformId,"",sMyRemark,"");//购买流水
//    QueryVct.push_back(sSql);

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

//// 钱包支付
//int WalletAccountDAL::Pay(const long & lUserId,const long & lValue, const long & lValue2, const int & iPayType,
//                          const long & lPlatformId,const string & sRemark,const string & sOrderNum,const int & iDiveied)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    // SQL
//    WalletAccountEntity entity;
//    iRet =  GetEntityByUseId(lUserId,entity) ;
//    if(iRet != 0){
//        return iRet;
//    }

//    vector<string> QueryVct;

//    //双币制减少金额
//    string sSql = "update " + gAccountTb +" set "+
//            " lUpdateId=" + tConvert.LongToStr(lCurrentTime) +
//            ",lUpTime=" + tConvert.LongToStr(lCurrentTime) ;

//    //jude the balance, (如果是绑定现金,包括了绑定现金手续费)
//    if( PAYTYPECASH == iPayType || PAYTYPESTC == iPayType){
//        if( entity.contentEntity.lCash - lValue < 0){
//            return ERRDATAOPERATE;
//        }
//        sSql += ",lCash =" + tConvert.LongToStr(entity.contentEntity.lCash - lValue) ;
//        if(PAYTYPESTC == iPayType && entity.contentEntity.lShopTicket - lValue2 < 0 ){
//            return ERRDATAOPERATE;
//        }
//        sSql += ",lShopTicket =" + tConvert.LongToStr(entity.contentEntity.lBandCash - lValue2) ;
//    }else if( PAYTYPEBANDCASH == iPayType || PAYTYPESTB == iPayType){
//        if( entity.contentEntity.lBandCash - lValue < 0){
//            return ERRDATAOPERATE;
//        }
//        sSql +=",lBandCash =" + tConvert.LongToStr(entity.contentEntity.lCash - lValue) ;
//        if(PAYTYPESTB == iPayType && entity.contentEntity.lShopTicket - lValue2 < 0 ){
//            return ERRDATAOPERATE;
//        }
//        sSql += ",lShopTicket =" + tConvert.LongToStr(entity.contentEntity.lBandCash - lValue2) ;

//    }else if( PAYTYPESHOPTICKET == iPayType){
//        if( entity.contentEntity.lShopTicket - lValue < 0){
//            return ERRDATAOPERATE;
//        }
//        sSql +=",lShopTicket =" + tConvert.LongToStr(entity.contentEntity.lShopTicket - lValue) ;
//    }
//    sSql += " where lUserId = "+tConvert.LongToStr(lUserId);
//    QueryVct.push_back(sSql);

//    //得到商家信息
//    ShopInfoEntity   enShopInfo;
//    iRet = ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr( lPlatformId), 1,  enShopInfo);
//    if( 0 != iRet){
//        return iRet;
//    }
//    string sDiveied;
//    if(  1 == iDiveied){
//        sDiveied = YESDDIVIED;
//    }else if( 2 == iDiveied){
//        sDiveied = NODDIVIED;
//    }

//    // 构建流水
//    string sMyRemark = "【购买】";
//    sMyRemark += sRemark ;
//    if( PAYTYPECASH == iPayType || PAYTYPESTC == iPayType ){
//        sSql = MakeSqlInsertDayBook(lUserId,lValue,enShopInfo.contentEntity.iOffLineDisCount,
//                                    sOrderNum,SERIALDIROUT,CASHTYPE,STEPBUY,lPlatformId,"",sMyRemark,sDiveied);//现金支付流水
//        QueryVct.push_back(sSql);
//    }
//    if( PAYTYPEBANDCASH == iPayType || PAYTYPESTB == iPayType){
//        sSql = MakeSqlInsertDayBook(lUserId,lValue,enShopInfo.contentEntity.iOffLineDisCount,
//                                    sOrderNum,SERIALDIROUT,BANDCASHTYPE,STEPBUY,lPlatformId,"",sRemark,sDiveied);
//        QueryVct.push_back(sSql);
//    }
//    if( PAYTYPESHOPTICKET == iPayType || PAYTYPESTB == iPayType  ||  PAYTYPESTC == iPayType ){
//        sSql = MakeSqlInsertDayBook(lUserId,lValue2,enShopInfo.contentEntity.iOffLineDisCount,
//                                    sOrderNum,SERIALDIROUT,SHOPTICKETTYPE,STEPBUY,lPlatformId,"",sMyRemark,sDiveied);//购物币支付流水
//        QueryVct.push_back(sSql);
//    }

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

////退款
//int WalletAccountDAL::BackPay(const long & lUserId,const string & sRemark,const string & sOrderNum)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    // SQL

//    //双币制可能存在多条购买流水

//    WalletDayBookList lstDayBook;
//    iRet = WalletDayBookDAL::GetInstance()->GetListByOrderNumStep(sOrderNum,STEPBUY,lstDayBook);
//    if( 0 != iRet){
//        return iRet;
//    }

//    WalletAccountEntity entity;
//    iRet =  GetEntityByUseId(lUserId,entity) ;
//    if(iRet != 0){
//        return iRet;
//    }

//    vector<string> QueryVct;

//    //  构建流水
//    string sMyRemark = "【退款】";
//    sMyRemark += sRemark ;


//    //退款
//    string sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
//    string sSql2;

//    WalletDayBookList::iterator it = lstDayBook.begin();
//    for ( ; it != lstDayBook.end() ; it ++){
//        WalletDayBookEntity &  enDayBook = *it;
//        if( enDayBook.contentEntity.iMoneyType == CASHTYPE){
//            sSql += ",lCash =lCash+ '" + tConvert.LongToStr(enDayBook.contentEntity.lMoney) + "' " ;
//            sSql2 = MakeSqlInsertDayBook( lUserId, enDayBook.contentEntity.lMoney,
//                                          0,sOrderNum,SERIALDIRIN,CASHTYPE,STEPBACKPAY,0,"",sMyRemark,"");
//            QueryVct.push_back(sSql2);
//        }
//        if( enDayBook.contentEntity.iMoneyType == BANDCASHTYPE){
//            sSql += ",lBandCash =lBandCash+ '" + tConvert.LongToStr(enDayBook.contentEntity.lMoney) + "' " ;

//            sSql2 = MakeSqlInsertDayBook(lUserId,enDayBook.contentEntity.lMoney,
//                                         0,sOrderNum,SERIALDIRIN ,BANDCASHTYPE,STEPBACKPAY,0,"",sMyRemark,"");
//            QueryVct.push_back(sSql2);
//        }
//        if( enDayBook.contentEntity.iMoneyType == SHOPTICKETTYPE){
//            sSql += ",lShopTicket =lShopTicket+ '" + tConvert.LongToStr(enDayBook.contentEntity.lMoney) + "' " ;
//            sSql2 = MakeSqlInsertDayBook(lUserId,enDayBook.contentEntity.lPlatformId,
//                                         0, sOrderNum,SERIALDIRIN, SHOPTICKETTYPE, STEPBACKPAY,0,"",sMyRemark, "");
//            QueryVct.push_back(sSql2);
//        }
//    }
//    sSql += "where lUserId = '"+tConvert.LongToStr(lUserId)+"'";
//    QueryVct.push_back(sSql);

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

////确认支付(简易版分成)
//int WalletAccountDAL::OkPay(const long & lUserId,const string & sOrderNum, const long & lCost)
//{
//    TConvert tConvert;
//    WalletAccountEntity accountEntity;
//    int iRet = -1;
//    long lCurrentTime = tConvert.GetCurrentTime();


//    WalletDayBookList lstDayBook;
//    iRet = WalletDayBookDAL::GetInstance()->GetListByOrderNumStep(sOrderNum,STEPBUY,lstDayBook);
//    if( 0 != iRet){
//        return iRet;
//    }

//    //双币制计算交易金额
//    long lPlatformId = 0;//交易的平台
//    int iDiscount = 100;
//    long lCash = 0;//现金交易金额
//    long lBandCash = 0;//绑定现金交易金额(包括了绑定现金手续费)
//    long lShopTicket = 0;//购物币交易金额
//    string sBusinessRemark;
//    for( WalletDayBookList::iterator it = lstDayBook.begin() ; it != lstDayBook.end(); it++){

//        WalletDayBookEntity & en = *it;
//        lPlatformId = en.contentEntity.lPlatformId;
//        iDiscount = en.contentEntity.iDiscount;
//        sBusinessRemark = en.contentEntity.sBusinessRemark;
//        if( en.contentEntity.iMoneyType == CASHTYPE){
//            lCash= en.contentEntity.lMoney;
//        }
//        if( en.contentEntity.iMoneyType == BANDCASHTYPE){
//            lBandCash= en.contentEntity.lMoney;
//        }
//        if( en.contentEntity.iMoneyType == SHOPTICKETTYPE){
//            lShopTicket= en.contentEntity.lMoney;
//        }
//    }
//    //购物币转现金(汇率10：1)
//    long lShopTtoMoney = 0;
//    if( lShopTicket > 10){
//        lShopTtoMoney  = lShopTicket/10;
//    }
//    //绑定现金要去掉绑定现金手续费
//    lBandCash =  lBandCash /11*10;
//    //交易总额(包括购物币)
//    long lMoney = lCash + lBandCash + lShopTtoMoney ;

//    long lDisCostMoney = 0;
//    //差价浮动
//    if( lCost < lMoney && lCost > 0){
//        lDisCostMoney = lMoney -  lCost;//购物币商城超出商品价收入
//        lMoney = lCost;
//    }

//    //是否要分帐
//    string sDivied = sBusinessRemark.substr(0,2);

//    //得到商家信息
//    ShopInfoEntity   enShopInfo;
//    iRet = ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr( lPlatformId ),  1, enShopInfo);
//    if( 0 != iRet){
//        return iRet;
//    }

//    //折扣的钱
//    long lDiCountMoney = lMoney -  lMoney *  iDiscount / 100 ;
//    //一步一步被瓜分后的余额
//    long lLeftMoney = lDiCountMoney;
//    //手续费
//    long lSxMoney = 0;
//    if(sDivied == YESDDIVIED ){
//        lSxMoney = lMoney*TBLL::GetInstance()->GetPeRun();
//    }else{
//        lSxMoney = lMoney*TBLL::GetInstance()->GetNDPeRun();
//    }
//    //第一次关注平台主的收益额
//    long lFstAttPltMoney = lMoney * TBLL::GetInstance()->GetFstAttPltAg();


//    vector<string> QueryVct;

//    // 开始构建多条SQL语句
//    ///////////////////////////////////////////////////////

//    //商家收益额
//    long lShopAddMoney = 0;
//    if(sDivied == YESDDIVIED ){
//        lShopAddMoney = lMoney - lDiCountMoney ;//需要扣去折扣金额
//    }else{
//        lShopAddMoney = lMoney - lSxMoney; //不需要扣去折扣, 需要扣去手续费
//    }

//    long lPlatfromUser =  enShopInfo.contentEntity.lUserId;
//    iRet = GetEntityByUseId(lPlatfromUser,accountEntity);
//    if( iRet != 0){
//        return iRet;
//    }
//    //第一次关注平台主
//    long lFstAttPltId = accountEntity.contentEntity.lPlatformId;
//    //商家收益
//    string sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash+ '" + tConvert.LongToStr( lShopAddMoney ) + "' " ;
//    sSql += "where lUserId = '"+tConvert.LongToStr(lPlatfromUser)+"'";
//    QueryVct.push_back(sSql);

//    string sMyRemark = "【收款】";
//    sMyRemark +=  " 订单【"  + sOrderNum + "】";
//    //商家收益流水
//    sSql = MakeSqlInsertDayBook(lPlatfromUser,lShopAddMoney,
//                                0,sOrderNum,SERIALDIRIN,CASHTYPE,STEPREVMONEY,lPlatformId,"",sMyRemark,"");
//    QueryVct.push_back(sSql);

//    ////////////////////////////////


//    iRet = GetEntityByUseId(SXWUSER,accountEntity);
//    if( iRet != 0){
//        return iRet;
//    }
//    //增加手续费帐号金额
//    sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash+ '" + tConvert.LongToStr( lSxMoney) + "' " ;
//    sSql += "where lUserId = '"+tConvert.LongToStr(SXWUSER)+"'";
//    //【手续费收益】附近消费【POS机10061503】订单【19390043】（现金-增加）
//    QueryVct.push_back(sSql);

//    //手续费收益流水
//    sMyRemark = "【手续费收益】" ;
//    sMyRemark += "订单【" + sOrderNum + "】";
//    //lUserId =?, set to 0
//    sSql = MakeSqlInsertDayBook(SXWUSER,lSxMoney,0,sOrderNum,
//                                SERIALDIRIN,CASHTYPE,STEPSXF,lPlatformId,"",sMyRemark,"");
//    QueryVct.push_back(sSql);
//    lLeftMoney -= lSxMoney;
//    if( lLeftMoney < 0  && sDivied == YESDDIVIED){
//        appendlog(TTDLogger::LOG_ERROR, "WalletAccountDAL::OKPay LeftMoney:%ld", lLeftMoney);
//        return -1;
//    }

//    /////////////////////////

//    //绑定现金手续费
//    long lBandSXMoney = lBandCash/11;
//    if( lBandSXMoney  > 0){

//        iRet = GetEntityByUseId(BANDSXUSER,accountEntity);
//        if( iRet != 0){
//            return iRet;
//        }

//        sSql = "update " + gAccountTb +" set "+
//                "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                ",lCash =lCash+ '" + tConvert.LongToStr(lBandSXMoney) + "' " ;
//        sSql += "where lUserId = '"+tConvert.LongToStr(BANDSXUSER)+"'";
//        QueryVct.push_back(sSql);

//        //绑定现金手续费收益流水
//        sMyRemark = "【绑定现金手续费收益】" ;
//        sMyRemark += "订单【" + sOrderNum + "】";
//        sSql = MakeSqlInsertDayBook( BANDSXUSER, lBandSXMoney ,0,
//                                     sOrderNum,SERIALDIRIN, CASHTYPE, STEPBANDSXFSY,lPlatformId,"",sMyRemark,"");
//        QueryVct.push_back(sSql);
//    }

//    if( lDisCostMoney > 0 ){
//        iRet = GetEntityByUseId(COSTUSER,accountEntity);
//        if( iRet != 0){
//            return iRet;
//        }
//        //购物币商城超出商品价收入
//        sSql = "update " + gAccountTb +" set "+
//                "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                ",lCash =lCash+ '" + tConvert.LongToStr(lDisCostMoney) + "' " ;
//        sSql += "where lUserId = '"+tConvert.LongToStr(COSTUSER)+"'";
//        QueryVct.push_back(sSql);

//        // 流水(购物币商城超出商品价收入)
//        sMyRemark = "【绑定现金手续费收益】" ;
//        sMyRemark += "订单【" + sOrderNum + "】";
//        sSql = MakeSqlInsertDayBook( COSTUSER, lDisCostMoney ,0,
//                                     sOrderNum,SERIALDIRIN, CASHTYPE, STEPCOST,lPlatformId,"",sMyRemark,"");
//        QueryVct.push_back(sSql);
//    }

//    ///////////////////////////////////////////

//    //需要分帐
//    if( sDivied == YESDDIVIED){

//        /////////////////////////////////////

//        iRet = GetEntityByUseId(lUserId,accountEntity);
//        if( iRet != 0){
//            return iRet;
//        }
//        //交易送购物币
//        long lShopTck = lMoney  * TBLL::GetInstance()->GetPeSpTicket(iDiscount);
//        sSql = "update " + gAccountTb +" set "+
//                "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                ",lShopTicket =lShopTicket+" + tConvert.LongToStr(lShopTck)  ;
//        sSql += " where lUserId ="+tConvert.LongToStr(lUserId);
//        QueryVct.push_back(sSql);

//        //送购物币流水
//        sMyRemark = "【送购物币】" ;
//        sMyRemark += "订单【" + sOrderNum + "】";
//        sSql = MakeSqlInsertDayBook( lUserId, lShopTck, 0,sOrderNum, SERIALDIRIN, SHOPTICKETTYPE,STEPGIVESHOPTICKET,lPlatformId,
//                                     "",sMyRemark,"");
//        QueryVct.push_back(sSql);

//        //扣送购物币成本
//        lLeftMoney -=  lShopTck/10;

//        /////////////////////////////////////

//        //推广员收益
//        long lPushMoney = lMoney * TBLL::GetInstance()->GetPePush();
//        WalletAccountEntity enAccount ,enAccount2, enAccount3;
//        enAccount.contentEntity.lPushId = 0;
//        GetEntityByUseId(lUserId,enAccount);
//        if( enAccount.contentEntity.lPushId > 0){

//            //1级推广员
//            sSql = "update " + gAccountTb +" set "+
//                    "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                    ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                    ",lCash =lCash+" + tConvert.LongToStr(lPushMoney)  ;
//            sSql += " where lUserId ="+tConvert.LongToStr(enAccount.contentEntity.lPushId);
//            QueryVct.push_back(sSql);

//            //1级推广员流水
//            sMyRemark = "【推广收益】" ;
//            sMyRemark += "订单【" + sOrderNum + "】";
//            sSql = MakeSqlInsertDayBook( enAccount.contentEntity.lPushId, lPushMoney,0,sOrderNum,
//                                         SERIALDIRIN,CASHTYPE,STEPFIRSTPUSHMONEY,lPlatformId,"",sMyRemark,"");
//            QueryVct.push_back(sSql);

//            //扣推广员收益
//            lLeftMoney -= lPushMoney;

//            enAccount2.contentEntity.lPushId = 0;
//            GetEntityByUseId(enAccount.contentEntity.lPushId,enAccount2);
//            if( enAccount2.contentEntity.lPushId > 0){
//                //2级推广员
//                sSql = "update " + gAccountTb +" set "+
//                        "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                        ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                        ",lCash =lCash+" + tConvert.LongToStr( lPushMoney )  ;
//                sSql += " where lUserId ="+tConvert.LongToStr(enAccount2.contentEntity.lPushId);
//                QueryVct.push_back(sSql);

//                //2级推广员流水
//                sMyRemark = "【推广收益】" ;
//                sMyRemark += "订单【" + sOrderNum + "】";
//                sSql = MakeSqlInsertDayBook( enAccount2.contentEntity.lPushId, lPushMoney,0,sOrderNum,
//                                             SERIALDIRIN,CASHTYPE,STEPSECONDPUSHMONEY,lPlatformId,"",sMyRemark,"");
//                QueryVct.push_back(sSql);

//                //扣推广员收益
//                lLeftMoney -= lPushMoney;

//                enAccount3.contentEntity.lPushId = 0;
//                GetEntityByUseId(enAccount2.contentEntity.lPushId,enAccount3);
//                if( enAccount3.contentEntity.lPushId > 0){
//                    //3级推广员
//                    sSql = "update " + gAccountTb +" set "+
//                            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                            ",lCash =lCash+" + tConvert.LongToStr( lPushMoney )  ;
//                    sSql += " where lUserId ="+tConvert.LongToStr(enAccount3.contentEntity.lPushId);
//                    QueryVct.push_back(sSql);
//                    //3级推广员流水
//                    sMyRemark = "【推广收益】" ;
//                    sMyRemark += "订单【" + sOrderNum + "】";
//                    sSql = MakeSqlInsertDayBook( enAccount3.contentEntity.lPushId,lPushMoney, 0,sOrderNum,SERIALDIRIN,
//                                                 CASHTYPE,STEPTHIRDPUSHMONEY, lPlatformId,"",sMyRemark,"");
//                    QueryVct.push_back(sSql);

//                    //扣推广员收益
//                    lLeftMoney -= lPushMoney;

//                }
//            }
//        }
//        //给代理瓜分的钱
//        long lForAgentMoney = lLeftMoney;

//        ZoneAgentList lstZoneAgent;
//        ZoneAgentDAL::GetInstance()->GetListByCity(enShopInfo.contentEntity.sCtiy,lstZoneAgent) ;
//        //区代理收益
//        if( lstZoneAgent.size() > 0){
//            int i = 0;
//            while( i < lstZoneAgent.size()){
//                ZoneAgentEntity & enZoneAgent= lstZoneAgent[i];
//                if ( enZoneAgent.contentEntity.sID == AGENTCITY ){
//                    iRet = GetEntityByUseId(enZoneAgent.lUserId,accountEntity);
//                    if( iRet != 0){
//                        return iRet;
//                    }
//                    long lAgentCityMoney = lForAgentMoney * TBLL::GetInstance()->GetPeCountyAg();
//                    sSql = "update " + gAccountTb +" set "+
//                            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                            ",lCash =lCash+" + tConvert.LongToStr( lAgentCityMoney )  ;
//                    sSql += " where lUserId ="+tConvert.LongToStr(enZoneAgent.lUserId);
//                    QueryVct.push_back(sSql);

//                    //区代理收益流水
//                    sMyRemark = "【区代理收益】" ;
//                    sMyRemark += "订单【" + sOrderNum + "】";
//                    sSql = MakeSqlInsertDayBook(enZoneAgent.lUserId, lAgentCityMoney, 0,sOrderNum,SERIALDIRIN,CASHTYPE,
//                                                STEPCITYAGENTMONEY,lPlatformId,"",sMyRemark, "");
//                    QueryVct.push_back(sSql);

//                    //扣区代理收益
//                    lLeftMoney -= lAgentCityMoney;

//                }
//                if( enZoneAgent.contentEntity.sID == AGENTPRO){

//                    iRet = GetEntityByUseId(enZoneAgent.lUserId,accountEntity);
//                    if( iRet != 0){
//                        return iRet;
//                    }
//                    long lAgentProMoney = lForAgentMoney * TBLL::GetInstance()->GetPeProAg();
//                    sSql = "update " + gAccountTb +" set "+
//                            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                            ",lCash =lCash+" + tConvert.LongToStr( lAgentProMoney)  ;
//                    sSql += " where lUserId ="+tConvert.LongToStr(enZoneAgent.lUserId);
//                    QueryVct.push_back(sSql);

//                    //省代理收益流水
//                    sMyRemark = "【省代理收益】" ;
//                    sMyRemark += "订单【" + sOrderNum + "】";
//                    sSql = MakeSqlInsertDayBook( enZoneAgent.lUserId, lAgentProMoney, 0,sOrderNum, SERIALDIRIN, CASHTYPE,STEPPROAGENTMONEY,
//                                                 lPlatformId,"",sMyRemark, "");
//                    QueryVct.push_back(sSql);

//                    //扣省代理收益
//                    lLeftMoney -= lAgentProMoney;

//                }
//                i++;
//            }
//        }
//        //个代收益
//        if( enShopInfo.contentEntity.lPerAgentId > 0 ){

//            iRet = GetEntityByUseId(enShopInfo.contentEntity.lPerAgentId,accountEntity);
//            if( iRet != 0){
//                return iRet;
//            }
//            PersonalAgentEntity perAgentEntity;
//            PersonalAgentDAL::GetInstance()->GetEntityByKey( tConvert.LongToStr(enShopInfo.contentEntity.lPerAgentId),1,perAgentEntity);
//            long lAgentPerMoney = lForAgentMoney * TBLL::GetInstance()->GetPePerAg();
//            //“商家所在区代”不同于“个代所在区代”时,个代收益13.3%,个代所在区代收益13.3%
//            if( perAgentEntity.sCity != enShopInfo.contentEntity.sCtiy ){
//                lAgentPerMoney =  lForAgentMoney * 0.133;
//                long ZoneAgentId = 0;
//                ZoneAgentDAL::GetInstance()->GetZoneAgentIdByCity(perAgentEntity.sCity,ZoneAgentId);
//                iRet = GetEntityByUseId(ZoneAgentId,accountEntity);
//                if( iRet != 0){
//                    return iRet;
//                }
//                sSql = "update " + gAccountTb +" set "+
//                        "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                        ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                        ",lCash =lCash+" + tConvert.LongToStr(lAgentPerMoney)  ;
//                sSql += " where lUserId ="+tConvert.LongToStr(ZoneAgentId);
//                QueryVct.push_back(sSql);

//                //个代所在区代收益流水
//                sMyRemark = "【个代所在区代收益】" ;
//                sSql = MakeSqlInsertDayBook( ZoneAgentId,lAgentPerMoney,0,sOrderNum,
//                                             SERIALDIRIN, CASHTYPE, STEPZONNEEPERGETMONEY,lPlatformId, "",sMyRemark,"");
//                QueryVct.push_back(sSql);

//                //扣个代所在区代收益
//                lLeftMoney -= lAgentPerMoney;


//            }

//            sSql = "update " + gAccountTb +" set "+
//                    "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                    ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                    ",lCash =lCash+" + tConvert.LongToStr(lAgentPerMoney)  ;
//            sSql += " where lUserId ="+tConvert.LongToStr(enShopInfo.contentEntity.lPerAgentId);
//            QueryVct.push_back(sSql);

//            //个代理收益流水
//            sMyRemark = "【个代理收益】" ;
//            sSql = MakeSqlInsertDayBook( enShopInfo.contentEntity.lPerAgentId,lAgentPerMoney,0,sOrderNum,
//                                         SERIALDIRIN, CASHTYPE, STEPPERAGENTMONEY,lPlatformId, "",sMyRemark,"");
//            QueryVct.push_back(sSql);

//            //扣个代理收益
//            lLeftMoney -= lAgentPerMoney;
//        }


//        ShopInfoEntity fstAttShopEntity;
//        ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr(lFstAttPltId),1,fstAttShopEntity);
//        iRet = GetEntityByUseId(fstAttShopEntity.contentEntity.lUserId,accountEntity);
//        if( iRet != 0){
//            return iRet;
//        }
//        //第一次关注平台主的收益
//        sSql = "update " + gAccountTb +" set "+
//                "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                ",lCash =lCash+" + tConvert.LongToStr(lFstAttPltMoney)  ;
//        sSql += " where lUserId ="+tConvert.LongToStr( fstAttShopEntity.contentEntity.lUserId);
//        QueryVct.push_back(sSql);
//        //第一次关注平台主的收益流水
//        sMyRemark = "【平台主收益】" ;
//        sSql = MakeSqlInsertDayBook(fstAttShopEntity.contentEntity.lUserId , lFstAttPltMoney, 0, sOrderNum, SERIALDIRIN,CASHTYPE, STEPPLTGETMONEY,lPlatformId,
//                                    "",sMyRemark,"");
//        QueryVct.push_back(sSql);
//        lLeftMoney -= lFstAttPltMoney;

//        //瓜分后剩下的钱不能小于0（集团公司也要有收益）
//        if( lLeftMoney < 0){
//            appendlog(TTDLogger::LOG_ERROR, "WalletAccountDAL::OKPay LeftMoney:%ld", lLeftMoney);
//            return -1;
//        }else{
//            iRet = GetEntityByUseId(JTUSER,accountEntity);
//            if( iRet != 0){
//                return iRet;
//            }
//            //集团公司钱包折扣收益
//            sSql = "update " + gAccountTb +" set "+
//                    "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                    ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                    ",lCash =lCash+" + tConvert.LongToStr(lLeftMoney)  ;
//            sSql += " where lUserId ="+tConvert.LongToStr( JTUSER);
//            QueryVct.push_back(sSql);

//            //集团公司钱包折扣收益流水
//            sMyRemark = "【折扣收益】" ;
//            sMyRemark += "订单【" + sOrderNum + "】";
//            sSql = MakeSqlInsertDayBook(JTUSER , lLeftMoney, 0, sOrderNum, SERIALDIRIN,CASHTYPE, STEPDISCMONEY,lPlatformId,
//                                        "",sMyRemark,"");
//            QueryVct.push_back(sSql);
//        }

//    }

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

////个代付费
//int WalletAccountDAL::PerAgentPay(const long & lUserId, const long & lValue, const int & iPayType, const string & sOrderNum, const string & sCity, const long & lPushId)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    long lMoney = lValue;
//    int iMeneyType = PAYTYPECASH;

//    //绑定现金除去绑定手续费
//    if( iPayType ==PAYTYPEBANDCASH ){
//        lMoney = lValue/11*10;
//        iMeneyType = PAYTYPEBANDCASH;
//    }
//    //个代自身送100%购物币
//    long lShopTicket = lMoney;
//    //运营手续费
//    long lSxMoney = lMoney*TBLL::GetInstance()->GetNDPeRun();
//    lMoney -= lSxMoney;
//    //推荐人费
//    long lPushMeney = lMoney /10;
//    //区代理费
//    long lAgentMeney = lMoney * 3 /10;
//    long lLeftMoney = lMoney - lPushMeney - lAgentMeney - lShopTicket/10;
//    long lCurrentTime =  tConvert.GetCurrentTime();
//    //
//    string sSql ;
//    string sMyRemark;
//    vector<string> QueryVct;

//    //更新个代表格数据，设置为开通等等
//    sSql = "update " + PersonalAgentDAL::msTableName +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",sCity ='" + sCity + "' " +
//            ",sOrderNo ='" + sOrderNum + "' " +
//            ",lPushId ='" + tConvert.LongToStr(lPushId) + "' " +
//            ",lMoney ='" + tConvert.LongToStr(lValue) + "' " +
//            ",iActivationState ='" + tConvert.LongToStr(3) + "' " +
//            ",lDealTime ='" + tConvert.LongToStr(lCurrentTime) + "' " +
//            sSql += " where lUserId ="+tConvert.LongToStr( lUserId);
//    QueryVct.push_back(sSql);

//    if( iPayType ==PAYTYPEBANDCASH ){
//        //绑定现金手续费帐号
//        sSql = "update " + gAccountTb +" set "+
//                "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                ",lCash =lCash+" + tConvert.LongToStr(lValue/11)  ;
//        sSql += " where lUserId =" + tConvert.LongToStr(BANDSXUSER);
//        QueryVct.push_back(sSql);

//        //绑定现金收益流水
//        sMyRemark = "【绑定现金手续费收益】" ;
//        sMyRemark += "订单【" + sOrderNum + "】";
//        sSql = MakeSqlInsertDayBook(BANDSXUSER , lValue/11, 0, sOrderNum, SERIALDIRIN,CASHTYPE, STEPBANDSXFSY,0,
//                                    "",sMyRemark,"");
//    }


//    string sTmp;
//    //个代费
//    if( iPayType ==PAYTYPECASH ){
//        sTmp = ",lCash =lCash-" + tConvert.LongToStr(lValue) + " ";
//    }else {
//        sTmp =  ",lBandCash=lBandCash-" + tConvert.LongToStr(lValue) + " ";
//    }
//    sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            sTmp + //减少个代用户的金额
//            ",lShopTicket =lShopTicket+" + tConvert.LongToStr(lShopTicket)  ;//个代自身送100%购物币
//    sSql += " where lUserId ="+tConvert.LongToStr( lUserId);
//    QueryVct.push_back(sSql);
//    //个代费流水
//    sMyRemark = "【个代费】" ;
//    sMyRemark += "订单【" + sOrderNum + "】";

//    sSql = MakeSqlInsertDayBook(lUserId , lValue, 0, sOrderNum, SERIALDIRIN,iMeneyType, STEPBUY,0,
//                                "",sMyRemark,"");
//    QueryVct.push_back(sSql);
//    //个代自身送100%购物币流水
//    sMyRemark = "【送购物币】用户开通个代后赠送购物币（购物币）" ;
//    sSql = MakeSqlInsertDayBook(lUserId , lShopTicket, 0, sOrderNum, SERIALDIRIN,SHOPTICKETTYPE, STEPOPENPERAGENTGIVETICKET,0,
//                                "",sMyRemark,"");
//    QueryVct.push_back(sSql);

//    //推荐人收益
//    sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash+" + tConvert.LongToStr(lPushMeney)  ;
//    sSql += " where lUserId ="+tConvert.LongToStr( lPushId);
//    QueryVct.push_back(sSql);
//    //推荐人收益流水
//    sMyRemark = "【个代费】" ;
//    sSql = MakeSqlInsertDayBook(lPushId , lPushMeney, 0, sOrderNum, SERIALDIRIN,CASHTYPE, STEPPERAGNENTPUSH,0,
//                                "",sMyRemark,"");
//    QueryVct.push_back(sSql);
//    PersonalAgentEntity peragentEntity;
//    iRet = PersonalAgentDAL::GetInstance()->GetEntityByKey( tConvert.LongToStr(lPushId),1,peragentEntity);
//    if( iRet == 0){
//        //二级推荐人收益
//        sSql = "update " + gAccountTb +" set "+
//                "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                ",lBandCash =lBandCash+" + tConvert.LongToStr(lPushMeney)  ;
//        sSql += " where lUserId ="+tConvert.LongToStr( peragentEntity.lPushId);
//        QueryVct.push_back(sSql);
//        //二级推荐人收益流水
//        sMyRemark = "【个代费】" ;
//        sSql = MakeSqlInsertDayBook(peragentEntity.lPushId , lPushMeney, 0, sOrderNum, SERIALDIRIN,BANDCASHTYPE, STEPPERAGNENTPUSH,0,
//                                    "",sMyRemark,"");
//        QueryVct.push_back(sSql);
//        lLeftMoney -= lPushMeney;
//    }


//    ZoneAgentList lstZoneAgent;
//    ZoneAgentDAL::GetInstance()->GetListByCity(sCity,lstZoneAgent) ;

//    //区代理收益
//    if( lstZoneAgent.size() > 0){
//        int i = 0;
//        while( i < lstZoneAgent.size()){
//            ZoneAgentEntity & enZoneAgent= lstZoneAgent[i];
//            if ( enZoneAgent.contentEntity.sID == AGENTCITY ){
//                sSql = "update " + gAccountTb +" set "+
//                        "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                        ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                        ",lCash =lCash+" + tConvert.LongToStr( lAgentMeney )  ;
//                sSql += " where lUserId ="+tConvert.LongToStr(enZoneAgent.lUserId);
//                QueryVct.push_back(sSql);

//                //区代理收益流水
//                sMyRemark = "【个代收益】区代理分成" ;
//                sSql = MakeSqlInsertDayBook(enZoneAgent.lUserId, lAgentMeney, 0,sOrderNum,SERIALDIRIN,CASHTYPE,
//                                            STEPPERAGNETZONE,0,"",sMyRemark, "");
//                QueryVct.push_back(sSql);
//                break;
//            }
//            i++;
//        }
//    }

//    //剩下的个代费打给个人代理费钱包
//    sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash+" + tConvert.LongToStr(lLeftMoney)  ;
//    sSql += " where lUserId =" + tConvert.LongToStr(PERAGENTUSER);
//    QueryVct.push_back(sSql);
//    // 流水(剩下的个代费打给个人代理费钱包)
//    sMyRemark = "【个代收益】开通费收款" ;
//    sSql = MakeSqlInsertDayBook(PERAGENTUSER , lLeftMoney, 0, sOrderNum, SERIALDIRIN,CASHTYPE, STEPPERAGENTOPENMONEY,0,
//                                "",sMyRemark,"");
//    QueryVct.push_back(sSql);

//    //运营手续费收益
//    sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lCash =lCash+ '" + tConvert.LongToStr( lSxMoney) + "' " ;
//    sSql += "where lUserId = '"+tConvert.LongToStr(SXWUSER)+"'";
//    QueryVct.push_back(sSql);
//    //运营手续费收益流水
//    sMyRemark = "【手续费收益】" ;
//    sSql = MakeSqlInsertDayBook(SXWUSER,lSxMoney,0,sOrderNum,
//                                SERIALDIRIN,CASHTYPE,STEPSXF,0,"",sMyRemark,"");
//    QueryVct.push_back(sSql);

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

////全民推广收益
//int WalletAccountDAL::AllPush( const long & plPushId, const long & plPlatformId,const string & psOrderNum)
//{
//    vector<string >QueryVct;
//    TConvert tConvert;
//    string sSql;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    //推荐人收益
//    sSql = "update " + gAccountTb +" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//            ",lShopTicket =lShopTicket+ '" + tConvert.LongToStr(1000) + "' " +//送10元购物币
//            ",lCash =lCash+" + tConvert.LongToStr(80)  ;//送0.8元现金
//    sSql += " where lUserId ="+tConvert.LongToStr( plPushId);
//    QueryVct.push_back(sSql);
//    //全民推广收益流水
//    string sMyRemark = "【全民推广】" ;
//    sSql = MakeSqlInsertDayBook(plPushId , 1000, 0, psOrderNum, SERIALDIRIN,SHOPTICKETTYPE, STEPALLPUSH,0,
//                                "",sMyRemark,"");
//    QueryVct.push_back(sSql);
//    sSql = MakeSqlInsertDayBook(plPushId , 80, 0, psOrderNum, SERIALDIRIN,CASHTYPE, STEPALLPUSH,0,
//                                "",sMyRemark,"");
//    QueryVct.push_back(sSql);
//    ShopInfoEntity shopEntity;
//    ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr(plPlatformId),1,shopEntity);
//    if( shopEntity.contentEntity.lUserId > 0){
//        sSql = "update " + gAccountTb +" set "+
//                "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
//                ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' " +
//                ",lCash =lCash+" + tConvert.LongToStr(20)  ;//送0.8元现金
//        sSql += " where lUserId ="+tConvert.LongToStr( shopEntity.contentEntity.lUserId);
//        QueryVct.push_back(sSql);
//        //全民推广收益流水
//        string sMyRemark = "【全民推广】" ;
//        sSql = MakeSqlInsertDayBook(shopEntity.contentEntity.lUserId , 20, 0, psOrderNum, SERIALDIRIN,CASHTYPE, STEPALLPUSH,0,
//                                    "",sMyRemark,"");
//        QueryVct.push_back(sSql);
//    }
//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    //事务处理
//    return Transaction(con,QueryVct);
//}

////添加
//int WalletAccountDAL::Insert( const WalletAccountEntity& entity  )
//{
//    //  ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    // SQL

//    string sSql = "insert into ";

//    sSql += gAccountTb+"( sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync," +
//            "sAccount,sPhone,sPassWord,lCash,lBandCash,lShopTicket,sAddress,sBankCard,lPushId,sQuestion,sAnswer,lPlatformId)" +
//            "VALUES( '"+entity.sPrimaryKey+"','" +
//            tConvert.LongToStr(entity.lUserId)+"','" +
//            tConvert.LongToStr(entity.lAddTime)+"','" +
//            tConvert.LongToStr(entity.lUpTime)+"','" +
//            tConvert.IntToStr(entity.iState)+"','" +
//            tConvert.LongToStr(entity.lUpdateId)+"','" +
//            "0','" +
//            entity.contentEntity.sAccount+"','" +
//            entity.contentEntity.sPhone+"','" +
//            entity.contentEntity.sPassWord+"','" +
//            tConvert.LongToStr(entity.contentEntity.lCash)+"','"+
//            tConvert.LongToStr(entity.contentEntity.lBandCash)+"','"+
//            tConvert.LongToStr(entity.contentEntity.lShopTicket)+"','"+
//            entity.contentEntity.sAddress+"','" +
//            entity.contentEntity.sBankCard+"','" +
//            tConvert.LongToStr(entity.contentEntity.lPushId)+"','"+
//            entity.contentEntity.sQuestion+"','" +
//            entity.contentEntity.sAnswer+"','" +
//            tConvert.LongToStr(entity.contentEntity.lPlatformId)+
//            "')";
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
////删除
//int WalletAccountDAL::Delete( const string& sPrimaryKey, const long lUptTime,const long lUpdateId )
//{
//    // ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    // SQL
//    string sSql = "update "+gAccountTb+" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lUpdateId) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lUptTime) + "' "+
//            ",iState = '333' "+
//            ",iSync = '0'" ;
//    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);
//}
////修改
//int WalletAccountDAL::Update(  const string& sPrimaryKey,const WalletAccountEntity& entity)
//{
//    //ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    // SQL
//    string sSql = "update "+gAccountTb+" set "+
//            "lUpdateId = '" + tConvert.LongToStr(entity.lUpdateId) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(entity.lUpTime) + "' "+
//            ",iState = '" + tConvert.LongToStr(entity.iState) + "' "+
//            ",iSync = '0'";
//    if (-1 != entity.contentEntity.lBandCash ) {
//        sSql += ",lBandCash = '" + tConvert.LongToStr(entity.contentEntity.lBandCash) + "' ";
//    }
//    if (-1 != entity.contentEntity.lCash ) {
//        sSql += ",lCash = '" + tConvert.LongToStr(entity.contentEntity.lCash) + "' ";
//    }
//    if (0 < entity.contentEntity.lPushId ) {
//        sSql += ",lPushId = '" + tConvert.LongToStr(entity.contentEntity.lPushId) + "' ";
//    }
//    if (0 < entity.contentEntity.lPlatformId ) {
//        sSql += ",lPlatformId = '" + tConvert.LongToStr(entity.contentEntity.lPlatformId) + "' ";
//    }
//    if (-1 != entity.contentEntity.lShopTicket ) {
//        sSql += ",lShopTicket = '" + tConvert.LongToStr(entity.contentEntity.lShopTicket) + "' ";
//    }
//    if ( entity.contentEntity.sAddress != "-1nochange" ) {
//        sSql += ",sAddress = '" + TMysqlDAL::GetInstance()->RealEscapeString(entity.contentEntity.sAddress) + "' ";
//    }
//    if ( entity.contentEntity.sBankCard != "-1nochange" ) {
//        sSql += ",sBankCard = '" + TMysqlDAL::GetInstance()->RealEscapeString(entity.contentEntity.sBankCard) + "' ";
//    }
//    if ( entity.contentEntity.sAnswer != "-1nochange" ) {
//        sSql += ",sAnswer = '" + TMysqlDAL::GetInstance()->RealEscapeString(entity.contentEntity.sAnswer) + "' ";
//    }
//    if ( entity.contentEntity.sPhone != "-1nochange" ) {
//        sSql += ",sPhone = '" + TMysqlDAL::GetInstance()->RealEscapeString(entity.contentEntity.sPhone) + "' ";
//    }
//    if ( entity.contentEntity.sQuestion != "-1nochange" ) {
//        sSql += ",sQuestion = '" + TMysqlDAL::GetInstance()->RealEscapeString(entity.contentEntity.sQuestion) + "' ";
//    }
//    if ( entity.contentEntity.sPassWord != "-1nochange" ) {
//        sSql += ",sPassWord = '" + entity.contentEntity.sPassWord + "' ";
//    }

//    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);
//}
////获取单个
//int WalletAccountDAL::GetEntityByUseId(const long & plUserId ,WalletAccountEntity & pOut)
//{
//    TConvert tConvert;
//   return GetEntity(tConvert.LongToStr(plUserId),3,pOut);
//}

//int WalletAccountDAL::GetEntityByKey(const string & pKey, WalletAccountEntity & pOut)
//{
//    return GetEntity(pKey,2,pOut);
//}


//int WalletAccountDAL::GetEntity(const string & pCond,int piType, WalletAccountEntity & pOut)
//{
//    //  ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = NULL;

//    // SQL
//    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,"
//                  "sAccount,sPhone,sPassWord,lCash,lBandCash,lShopTicket,sAddress,sBankCard,lPushId,sQuestion,sAnswer,lPlatformId"
//                  " from "+gAccountTb  + " where iState!=333 and";
//    if( piType ==3){
//        sSql += " lUserId=" + pCond;
//    }else if( piType == 2){
//         sSql += " sPrimaryKey='" + pCond + "'";
//    }else{
//        sSql += " sPhone='" + pCond + "'";
//    }
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
//            iRet = 0;
//            int iIndex = 0;

//            pOut.sPrimaryKey =   (row[iIndex++]);
//            pOut.lUserId = atol( row[iIndex++] );
//            pOut.lAddTime = atol( row[iIndex++] );
//            pOut.lUpTime = atol( row[iIndex++] );
//            pOut.iState = atoi( row[iIndex++] );
//            pOut.lUpdateId = atol( row[iIndex++] );
//            pOut.contentEntity.sAccount =   (row[iIndex++]);
//            pOut.contentEntity.sPhone =   (row[iIndex++]);
//            pOut.contentEntity.sPassWord =   (row[iIndex++]);
//            pOut.contentEntity.lCash = atol(row[iIndex++]);
//            pOut.contentEntity.lBandCash = atol( row[iIndex++] );
//            pOut.contentEntity.lShopTicket = atol( row[iIndex++] );
//            pOut.contentEntity.sAddress =   ( row[iIndex++] );
//            pOut.contentEntity.sBankCard =   ( row[iIndex++] );
//            pOut.contentEntity.lPushId = atol( row[iIndex++] );
//            pOut.contentEntity.sQuestion =   ( row[iIndex++] );
//            pOut.contentEntity.sAnswer =   ( row[iIndex++] );
//            pOut.contentEntity.lPlatformId = atol( row[iIndex++] );
//        }
//    }

//    con->FreeResult( result );
//    return iRet;
//}

////获取记录
//int WalletAccountDAL::GetList(const WalletAccountQueryEntity & pQueryEntity, int piIndex, int piCount,WalletAccountList& plstOut, int & piTotalCount)
//{
//    TConvert tConvert;

//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = NULL;

//    // SQL
//    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
//                  "sAccount,sPhone,sPassWord,lCash,lBandCash,lShopTicket,sAddress,sBankCard,lPushId,sQuestion,sAnswer,lPlatformId"
//                  " from "+gAccountTb ;
//    string sCond = " where iState!=333 and";
//    if (! pQueryEntity.sAccount.empty()){
//        sCond+="  sAccount like '%"+ pQueryEntity.sAccount +"%' and";
//    }
//    if (! pQueryEntity.sPhone.empty()){
//        sCond+="  sPhone like '%"+ pQueryEntity.sPhone +"%' and";
//    }
//    sCond= sCond.substr(0,sCond.length() -3);
//    sSql += sCond;
//    sSql += " limit  " + tConvert.IntToStr(piIndex) + "," + tConvert.IntToStr(piCount);


//    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    result =  con->QueryResult(sSql);

//    MYSQL_ROW row = NULL;
//    if( result != NULL){
//        while ( NULL != (row =  mysql_fetch_row( result ))) {

//            iRet = 0;
//            WalletAccountEntity entity;
//            int iIndex = 0;

//            entity.sPrimaryKey =   (row[iIndex++]);
//            entity.lUserId = atol( row[iIndex++] );
//            entity.lAddTime = atol( row[iIndex++] );
//            entity.lUpTime = atol( row[iIndex++] );
//            entity.iState = atoi( row[iIndex++] );
//            entity.lUpdateId = atol( row[iIndex++] );
//            iIndex++;
//            entity.contentEntity.sAccount =   (row[iIndex++]);
//            entity.contentEntity.sPhone =   (row[iIndex++]);
//            entity.contentEntity.sPassWord =   (row[iIndex++]);
//            entity.contentEntity.lCash = atol(row[iIndex++]);
//            entity.contentEntity.lBandCash = atol( row[iIndex++] );
//            entity.contentEntity.lShopTicket = atol( row[iIndex++] );
//            entity.contentEntity.sAddress =   ( row[iIndex++] );
//            entity.contentEntity.sBankCard =   ( row[iIndex++] );
//            entity.contentEntity.lPushId = atol( row[iIndex++] );
//            entity.contentEntity.sQuestion =   ( row[iIndex++] );
//            entity.contentEntity.sAnswer =   ( row[iIndex++] );
//            entity.contentEntity.lPlatformId = atol( row[iIndex++] );
//            plstOut.push_back( entity);

//        }

//        sSql = "select count(*)  from " + gAccountTb ;
//        sSql  +=  sCond;

//        GetQueryTotalCount(sSql,piTotalCount);
//    }

//    // 释放内存
//    con->FreeResult( result );

//    return iRet;
//}
////更换手机号
//int WalletAccountDAL::UpdatePhone(const string & psPhone, const string & psNewPhone)
//{
//    TConvert tConvert;
//    long lCurT = tConvert.GetCurrentTime();
//    // SQL
//    string sSql = "update "+gZoneAgentTb+" set "+
//            "lUpdateId = '" + tConvert.LongToStr(lCurT) + "' "+
//            ",lUpTime = '" + tConvert.LongToStr(lCurT) + "' ";
//    sSql += ",sPhone='" +  psNewPhone + "'" ;

//    sSql += "where sPhone = '"+psPhone+"'";

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);

//}

//int WalletAccountDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
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


//int WalletAccountDAL::ModifyPwd(const string & psPrimaryKey,const string & psNewPwd,const long & plUpdateId){
//    ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    // SQL
//    string sSql = "update "+gAccountTb+" set "+
//            "lUpdateId='" + tConvert.LongToStr(plUpdateId) + "'"+
//            ",lUpTime='" + tConvert.LongToStr(plUpdateId) + "'" +
//            ",sPassWord='" + psNewPwd + "'";
//    sSql += " where sPrimaryKey ='"+psPrimaryKey+"'";

//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -22;
//    }
//    return con->Query(sSql);
//}

////构建插入流水表语句
//string WalletAccountDAL::MakeSqlInsertDayBook(const long & lUserId,const long & lValue,const int & iDiscount,const string & sOrderNum,const int & iDir,
//                                              const int & iType,const int & iStep,const long & lPlatformId,const string & sQRcode,const string & sRemark,const string & sBusinessRemark)
//{
//    TConvert tConvert;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    string sKey = tConvert.LongToStr(lCurrentTime) + tConvert.LongToStr(lUserId);

//    string   sSql =  "insert into " +  gDayBookTb+"( "
//                                                  "sPrimaryKey,"
//                                                  "lUserId,"
//                                                  "lAddTime,"
//                                                  "lUpTime,"
//                                                  "sSerialNum,"
//                                                  "sOrderNum,"
//                                                  "iDir,"
//                                                  "lMoney,"
//                                                  "iMoneyType,"
//                                                  "iDiscount,"
//                                                  "lPlatformId,"
//                                                  "iStep,"
//                                                  "sQRcode,"
//                                                  "sRemark,"
//                                                  "sBusinessRemark"
//                                                  ")"
//                                                  "VALUES( '";
//    sSql += sKey + "','";//sPrimaryKey
//    sSql += tConvert.LongToStr(lUserId) + "','";//lUserId
//    sSql += tConvert.LongToStr(lCurrentTime)+ "','";//lAddTime
//    sSql += tConvert.LongToStr(lCurrentTime)+ "','";//lUpTime
//    sSql += sKey + "','";//sSerialNum
//    sSql += sOrderNum + "','";//sOrderNum
//    sSql += tConvert.IntToStr(iDir)+ "','";//iDir
//    sSql += tConvert.LongToStr(lValue)+ "','";//lMoney
//    sSql += tConvert.IntToStr(iType)+ "','";//
//    sSql += tConvert.IntToStr(iDiscount)+ "','";//iDiscount
//    sSql += tConvert.LongToStr(lPlatformId)+ "','";//lPlatformId
//    sSql += tConvert.IntToStr(iStep)+ "','";//iStep
//    sSql += sQRcode + "','";//sQRcode
//    sSql += sRemark + "','";//sRemark
//    sSql += sBusinessRemark + "')";//sBusinessRemark
//    return sSql ;
//}

////事务处理（只能是增册改的操作）
//int WalletAccountDAL::Transaction( TMultiMysqlDAL * con,const vector<string>  & pQueryLst)
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
