#ifndef DEFINE_H
#define DEFINE_H
#include <string>
#include <vector>
using namespace std;

enum EnumCashBack{
    enum_cashback_close = 0,
    enum_cashback_open = 1
};


enum EnumBuyRestrict{
    enum_buyrestrict_false = 0,
    enum_buyrestrict_true = 1
};

enum EnumCoinBack{
    enum_coinback_close = 0,
    enum_coinback_open = 1
};

enum EnumPark{
    enum_park_close = 0,
    enum_park_open = 1
};

enum EnumWifi{
    enum_wifi_close = 0,
    enum_wifi_open = 1
};

enum EnumDelivery{
    enum_delivery_close = 0,
    enum_delivery_open = 1
};

//0未审核 1 审核通过 2 审核不过
enum EnumVerify{
    enum_verify_un = 0,
    enum_verify_yes = 1,
    enum_verify_no = 2
};

//显示 1 隐藏 0
enum EnumShow{
    enum_show_no = 0,
    enum_show_yes = 1
};


//购物币币种 0-普通1-专属
enum EnumCoinType{
    enum_cointype_normal = 0,
    enum_cointype_private = 1
};

struct ShopSetting
{
    long lPlatformId;//平台ID
    long lAddTime;
    int iCashBack;//返现开关 0 -关  1 -开
    int iCoinBack;//返购物币开关 0 -关  1 -开
    long lStartTime;//返现生效时间
    int iRecommend;//推荐开关（0 -关 1-开）
    int iRestrict;//限购开关 0-不限  1-限
    int iPark;//停车  0-不免费   1-免费
    int iWifi;//WIFI  0-没wifi   1-有wifi
    int iDelivery;//送货  0-没有 1-免费送货
    string sCheckPwd;//检查查看密码
};

struct ZoneAgentContentEntity
{
    ::std::string sAccount;
    ::std::string sPhone;
    ::std::string sPassWord;
    ::std::string sName;
    ::std::string sUnit;
    ::std::string sID;
    ::std::string sCity;
    ::std::string sRemark;
};
//区代理
struct ZoneAgentEntity
{
    ::std::string sPrimaryKey;
    long lAddTime;
    long lUpTime;
    int iState;
    long lUpdateId;
    long lUserId;
    ZoneAgentContentEntity contentEntity;
};

struct ZoneAgentQueryEntity
{
    ::std::string sAccount;
    ::std::string sPhone;
    ::std::string sPassWord;
    ::std::string sName;
    ::std::string sUnit;
    ::std::string sID;
    ::std::string sCity;
    ::std::string sRemark;
    long lUserId;
};

typedef vector< ZoneAgentEntity> ZoneAgentList;


struct ShopInfoContentEntity
{
    long lUserId;
    ::std::string sAccount;
    ::std::string sPassWord;
    ::std::string sShopName;
    ::std::string sCompanyName;
    ::std::string sShopPhone;
    ::std::string sBusiness;
    ::std::string sShopType;
    int iOffLineDisCount;
    ::std::string sLicensePhoto;
    ::std::string sIdPhoto;
    ::std::string sShopFrontPhoto;
    ::std::string sShopDetailPhoto;
    ::std::string sAddress;
    ::std::string sCtiy;
    double dGpsLat;
    double dGpsLon;
    ::std::string sHours;
    ::std::string sPosNum;
    long lPerAgentId;
    int iAwardingPoints;
    long lServiceUserId;
    ::std::string sSevvicePhone;
    long lDistance;
    int iAvgConsume;
    int iShowNearby;
    int iProrate;
    int iVerify;
    int iAuth;
    std::string sRemark;
    std::string sOtherCerPhoto;
    std::string sTradeMark;
    std::string sPushArea;
    int iCoinType;
    int iCoinPercent;
    int iPromotionDiscount;
};
//商家信息
struct ShopInfoEntity
{
    ::std::string sPrimaryKey;
    long lAddTime;
    long lUpTime;
    int iState;
    long lUpdateId;
    ShopInfoContentEntity contentEntity;
};

struct ShopQueryEntity
{
    ShopQueryEntity(){
        dGpsLat = -1.0;
        dGpsLon = -1.0;
        iVerify = -1;
        iAuth = -1;
        iApplyDiscount = -1;
        lPerAgentId = -1;
        iShowNearby = -1;
        lStartTime  = -1;
        lEndTime = -1;
        bStar = false;
        iEnumDiscount =-1;
        bCityZone = true;
        lUserId = -1;
    }

    ::std::string sShopType;
    ::std::string sCtiy;
    bool bCityZone;
    ::std::string sShopName;
    double dGpsLat;
    double dGpsLon;
    int iVerify;
    int iAuth;
    int iApplyDiscount;
    ::std::string sShopPhone;
    long lPerAgentId;
    int iShowNearby;
    long lStartTime;
    long lEndTime;
    bool bStar;
    int iEnumDiscount;
    string sTown;
    string sArea ;
    string sPlatformId;
    long lUserId;
};

typedef ::std::vector< ShopInfoEntity> ShopInfoList;

struct WalletAccountContentEntity
{
    ::std::string sAccount;
    ::std::string sPhone;
    ::std::string sPassWord;
    long lCash;
    long lBandCash;
    long lShopTicket;
    ::std::string sAddress;
    ::std::string sBankCard;
    long lPushId;
    long lPlatformId;
    ::std::string sQuestion;
    ::std::string sAnswer;
};
//钱包帐号
struct WalletAccountEntity
{
    ::std::string sPrimaryKey;
    long lAddTime;
    long lUpTime;
    int iState;
    long lUpdateId;
    long lUserId;
    WalletAccountContentEntity contentEntity;
};

typedef ::std::vector< WalletAccountEntity> WalletAccountList;

struct WalletAccountQueryEntity
{
    ::std::string sAccount;
    ::std::string sPhone;
    ::std::string sAddress;
    ::std::string sBankCard;
};


struct WalletDayBookContentEntity
{
    ::std::string sAccount;
    ::std::string sPhone;
    ::std::string sSerialNum;
    ::std::string sOrderNum;
    int iDir;
    long lMoney;
    int iMoneyType;
    int iDiscount;
    long lPlatformId;
    ::std::string sQRcode;
    ::std::string sRemark;
    ::std::string sBusinessRemark;
    int iStep;
};
//流水帐数据
struct WalletDayBookEntity
{
    ::std::string sPrimaryKey;
    long lAddTime;
    long lUpTime;
    int iState;
    long lUpdateId;
    long lUserId;
    WalletDayBookContentEntity contentEntity;
};
typedef ::std::vector< WalletDayBookEntity> WalletDayBookList;

struct WalletDayBookQueryEntity
{
    long lUserId;
    string sOrderNum;
    int iStep;
    int iMoneyType;
};

//个人代理数据
struct PersonalAgentEntity
{
    ::std::string sPrimaryKey;//个人代理用户Id
    long lAddTime;
    long lUpTime;
    int iState;
    long lUpdateId;
    ::std::string sPhone;//个代手机号
    int iActivationState;//个代激活状态。 0：未开通，1：试用中，2：试用结束，3已开通
    long lActivationTime; //个代激活时间
    ::std::string sOrderNo;//个代开通的订单号
    long lMoney; //开通的金额
    long lExpireTime; //到期时间
    long lDealTime; //开通时间
    ::std::string sCity; //区代理
    long lPushId; //推荐人用户ID
};
typedef ::std::vector< PersonalAgentEntity> PersonalAgentList;
struct PersonalAgentQueryEntity
{
    int iActivationState;
    long lActivationTimeStart;
    long lActivationTimeEnd;
    string sPhone;
    string sCity;
};

//银行卡数据
struct BankCardEntity
{
    ::std::string sPrimaryKey;//银行卡号+用户ID
    long lAddTime;
    long lUpTime;
    int iState;
    long lUpdateId;
    ::std::string sBankCard; //银行卡号
    long lUserId; //用户ID
    ::std::string sName;//银行卡所属人名称
    ::std::string sBank;//归属银行
    ::std::string sBranch;//所属支行
    int iCerType;//证件类型 1 -身份证
    ::std::string sCerId;//证件号码
    ::std::string sCustomerNm;//银行卡所属人名称
    ::std::string sCredit1;//信用卡1
    ::std::string sCredit2;//信用卡2
    ::std::string sBindId;//银联绑定银行卡的ID
    ::std::string sOrderNum;//银联绑定的订单号
    ::std::string sMerId;//银联商户ID
    long lBandTime;//绑定时间

};
typedef ::std::vector< BankCardEntity> BankCardList;
struct BankCardQueryEntity
{
    long lUserId;
};


struct BankEntity
{
    long lPlatformId;
    long lAddTime;
    string sName;
    string sAccount;
    string sBranch;
    string sBank;
};



struct CashBackShop
{
    long lPlatformId;
    long lStartTime;
    string sShopPhone;
};
typedef vector<CashBackShop> CashBackShopLst;

//广告
struct ShopAdv{
    long id;
    long lPlatformId;
    long lAddTime;
    string sAdvLink;
    int iVerify;
    int iState;
};
typedef vector<ShopAdv> ShopAdvLst;


//评论
struct CommentEntity{
    long lPlatformId;//平台ID
    long idProduct;//
    long lUserId;//评论者ID
    string sNickName;//评论者昵称
    string sHeadImage;//评论者头像
    long lAddTime;
    string sContent;//内容
    int iScore;//评分（满分100）
    string sPics;//评论图片数组

};
typedef vector<CommentEntity> CommentEntityLst;

//展示产品
struct ShowPro{
    long id;//产品Id
    long lPlatformId;//平台ID
    long lAddTime;
    string sPicLink;//产品图片链接（OSS地址或其它）
    string sName;//产品名称
    double dPrice;//价格
    double dCoin;//购物币
    int iCoinType;
    int iVerify;
    int iState;
    string sContent;
    string sExpireTime;
    string sCanot;
    string sUtime;
    string sLaw;
    string sDetailPics;
    string sSpecification;


    string sShopName;

};
typedef vector<ShowPro> ShowProLst;
struct QueryShowPro
{
    string sShopName;
    string sPlatformId;
};


//专题
struct Subject{
    long Id;//专题Id
    long lAddTime;
    string sBigTopPic;//图片链接（OSS地址或其它）
    string sBigBottonPic;
    string sSmallTopPic;
    string sSmallBottonPic;
    string sRemark;
    string sName;//专题名称
    int iState;
};
typedef vector<Subject> SubjectLst;


//联盟商家分类
struct NearCalss{
    long Id;//Id
    string sBigPic;//图片链接（OSS地址或其它）
    string sSmallPic;
    string sName;//名称
    string sAction;

};
typedef vector<NearCalss> NearCalssLst;


//用户权限
struct UserPower{
    int power;
    long idPlatform;
    long userId;
    string account;
    string nickName;
    string headImage;

};
typedef vector<UserPower> UserPowerLst;


struct CouponCode
{
    CouponCode(){
        state = -1;
        mark = -1;
    }

    int state;
    string backTime;
    string consumTime;
    string reqTime;
    string code;
    string reason;
    string rejReason;
    int mark;
};


enum enum_couponcode_state
{
    //1可使用 2待评价 3退款中 4退款成功 5拒绝退款 6已消费 7已过期 8 已评价
    couponcode_state_canuser = 1,
    couponcode_state_tomark = 2,
    couponcode_state_backing = 3,
    couponcode_state_backed  =4,
    couponcode_state_reqback = 5,
    couponcode_state_beused = 6,
    couponcode_state_timeout = 7,
    couponcode_state_bemarked = 8

};
//优惠券订单
struct Coupon{
    Coupon(){
        userId = -1;
        addTime = -1;
        idPlatform = -1;
        idProduct = -1;
        price = -1.0;
        coin = -1.0;
        num = -1;
        dGpsLat = -1.0;
        dGpsLon = -1.0;
    }

    long userId;

    string orderNo;
    long addTime;
    long idPlatform;
    long idProduct;
    string expireTime;
    string payTime ;
    string endTime ;
    double price ;
    double coin ;
    int num ;

    CouponCode couponCode;

    string shopName;
    string shopType;
    string shopPhone;
    string shopAddr;
    double dGpsLat;
    double dGpsLon;
    int iOffLineDisCount;
    int iCoinType;

    string content;
    string picLink;
    string name;
    string specification;

};
typedef vector<Coupon> CouponLst;


static const string gShopTb = "shopdal";
static const string gAccountTb = "accountdal";
static const string gDayBookTb = "serialdal";
static const string gZoneAgentTb = "zoneagentdal";
static const string gBankCardTb = "bankcarddal";
//////////////////////////////////////////////////////
//货币类型
//现金
#define CASHTYPE 1
//绑定现金
#define BANDCASHTYPE 2
//购物币
#define SHOPTICKETTYPE 3
///////////////////////////////////////
//
#define SERIALDIRIN 1
#define SERIALDIROUT 2
//////////////////////////////////////////
//流水动作
//1-【购买】
#define STEPBUY 1
//2-【收款】
#define STEPREVMONEY 2
//3-【折扣】
#define STEPDISCOUNT 3
//4-【折扣收益】
#define STEPDISCMONEY 4
//5-【推广】
#define STEPFIRSTPUSH 5
//6-【推广收益】
#define STEPFIRSTPUSHMONEY 6
//7-【平台】
//8-【平台收益】
#define STEPPLTGETMONEY 8
//9-【合作商】
//10-【合作商收益】
#define STEPPERAGENTMONEY 21
//11-【市代理】
//12-【市代理收益】
#define STEPCITYAGENTMONEY 12
//15-【送购物币】
#define STEPGIVESHOPTICKET 15
//16-【第三方支付】
#define STEPTHIRDPAY 16
//17-【绑定现金手续费】
#define STEPBANDSXFSY 17
//18-【手续费】
#define STEPSXF 18
//19-【手续费收益】
#define STEPSXFSY 19
//20-【省代理】
//21-【省代理收益】
#define STEPPROAGENTMONEY 21
//22-【钱包】充值
#define STEPINMONEY 22
//25-【钱包】提现（现金）
#define STEPOUTCASH 25
//26-【钱包】提现失败（现金）
#define STEPOUTCASHFAIL 26
//27-全民推广（绑定现金）
#define STEPALLPUSH 26
//28【送购物币】首次进入【系统赠送】（购物币）
//29【送购物币】消费（购物币）
//30【利息收益】利息【绑定现金的利息】（购物币）
//31-财神宝
//32-2级推广
#define STEPSECONDPUSH 32
//33-2级推广收益
#define STEPSECONDPUSHMONEY 33
//34-3级推广
#define STEPTHIRDPUSH 34
//35-3级推广收益
#define STEPTHIRDPUSHMONEY 35
//37-退款
#define STEPBACKPAY 37
//38 -购物币收款
#define STEPREVSHOPTICKET 38
//购物币商城超出商品价收入
#define STEPCOST 39
//个代开通推荐人收益
#define STEPPERAGNENTPUSH 40
//个代开通送购物币
#define STEPOPENPERAGENTGIVETICKET 41
//42-【个代收益】区代理分成
#define STEPPERAGNETZONE 42
//个代收益开通费收款
#define STEPPERAGENTOPENMONEY 44
//46- 【个代所在区代收益】“商家所在区代”不同于“个代所在区代”时,个代所在区代收益
#define STEPZONNEEPERGETMONEY 46
///////////////////////////////////////////////////////////////////////////
//支付方式：现金
#define PAYTYPECASH 1
//支付方式: 绑定现金
#define PAYTYPEBANDCASH 2
//支付方式：购物币
#define PAYTYPESHOPTICKET 3
//支付方式：购物币+现金
#define PAYTYPESTC 4
//支付方式：购物币+绑定现金
#define PAYTYPESTB 5
////////////////////////////////////////////////////////////////////////////
#define YESDDIVIED "YD"
#define NODDIVIED "ND"

///////////////////////////////////////////////////////////////////////////
//集团公司钱包
#define JTWALLET 10021344
#define JTUSER 18
//手续费钱包
#define SXWALLET 10021346
#define SXWUSER 28
//绑定现金手续费帐号
#define  BANDSXWALLET 10021345
#define BANDSXUSER 38
//购物币商城超出商品价收入
#define COSTWALLET 10037829
#define COSTUSER 75
//个人代理费钱包
#define PERAGENTUSER 71
#define PERAGENTWALLET 10025450
///////////////////////////////////////////////////////////
#define AGENTCITY "市代理"
#define AGENTPRO "省代理"
#endif // DEFINE_H
