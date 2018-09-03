var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
var urlencode = require('urlencode');
//https请求参数
var option = {

   //hostname: 'im.ttdomo.com',
   //hostname:"120.76.26.120",//外网测试
   hostname: '127.0.0.1',
    port: 0,
    path: '',
    method: 'POST',
    //passphrase: 'tymtym',
   // pfx: fs.readFileSync("../../certs/client.pfx"),
    rejectUnauthorized: false,
    headers: {
     "Content-Length": 0
 }
}

//测试用例列表
var unitsShop =
        [
            // {info:'测试 GetSysTime',port:30053,path:'/?sMethod=GetSysTime'
            // , post_data:{ iIndex : 0, iCount : 8}},

   //       {info:'测试 PhoneCode',port:13241,path:'/?sMethod=PhoneCode&sBusiness=UserRegister'
       //   , post_data:{ iServerID : 1, sAccount : "13411367357"}}

        //       {info:'测试 ModifyPwd',port:13241,path:'/?sMethod=ModifyPwd&sBusiness=UserRegister'
     //   , post_data:{ sPrimaryKey : '50037', sOldPwd : "123456", sNewPassword : '0a69d8c89e305fe071e2057fae7d04b0'}},

      //     {info:'测试 BandGciID',port:13241,path:'/?sMethod=BandGciID&sBusiness=UserRegister'
        //   , post_data:{ lUserId : 60419, sGciID: "guangcai" ,sGciPwd: "111", sGciPhone : "15992615255"}}
          //  {info:'测试 Attention',port:30053,path:'/?sMethod=Attention'
          //   , post_data:{ lUserId : 50022, lPlatformId: 100006 }}

           // {info:'测试 PlatformFans',port:23241,path:'/?sMethod=PlatformFans&sBusiness=UserRegister'
           //  , post_data:{ lPlatformId: 111000 }},

            // {info:'测试 AddData',port:23241,path:'/?sMethod=AddData&sBusiness=UserRegister'
            //  , post_data:{
            //      "sAccount": "13411118979",
            //      "sNickName": '123',
            //      "sPassword": '0a69d8c89e305fe071e2057fae7d04b0',
            //      "iSex": 0,
            //      "sProvince": '广东省',
            //      "sCity": '珠海市',
            //      "sSignature": 'ah,ah,ah',
            //      "sEmail": '123456@163.com',
            //      "sHeadImage": '',
            //      "iPhoneAuth": 1,
            //      "lPlatformID" : 100012,
            //       "sCode" : TymLib.md5( "13411118979ttdsu"  ),
            //       "lPushManId" : "39",
            //       "sBirthDay":'1970-02-02'
            //    }}

             //            {info:'测试 MyShops',port:23241,path:'/?sMethod=MyShops&sBusiness=UserRegister'
             // , post_data:{
             //     "lUserId": 50037
             //   }}

            // {info:'测试 Make',port:23241,path:'/?sMethod=Make&sBusiness=UserData'
            //  , post_data:{
            //       "iCount": 3400,
            //       "sBegin" : "236",
            //       "sTimeStart" : "2018-01-01",
            //       "sTimeEnd" : "2018-02-01",
            //       "sCode" : "better.9394",
            //       "lPlatformId":100012,
            //       "lPushManId" : 109968,
            //       "dPrice" : 6.9

            //    }}
             //   {info:'测试 PeragentLst',port:23241,path:'/?sMethod=PeragentLst&sBusiness=UserData'
             // , post_data:{
             //      "iCount": 200,
             //      "iPage" : 1,
             //      "sPhone" :150,
             //      "sWorkZone" : "市"
             //   }}

             //   {info:'测试 ShopLst',port:23241,path:'/?sMethod=ShopLst&sBusiness=UserData'
             // , post_data:{
             //      "iCount": 30,
             //      "iPage" : 1,
             //      "sPlatformId" : 7600767

             //   }}

             //   {info:'测试 GetUnHotCount',port:23241,path:'/?sMethod=GetUnHotCount&sBusiness=UserData'
             // , post_data:{
             //      "sDay": "2018-01-01"

             //   }}

             //  {info:'测试 GetUnHotInfoByDay',port:23241,path:'/?sMethod=GetUnHotInfoByDay&sBusiness=UserData'
             // , post_data:{
             //      "iPage": "1",
             //      "iCount" : 1,
             //      "iType" :2,
             //      "sDay" : "2018-05-21",
             //      "iHot" : 0

             //   }}

             //  {info:'测试 GetVerifyList',port:23241,path:'/?sMethod=GetVerifyList&sBusiness=UserData'
             // , post_data:{
             //      "iPage": "1",
             //      "iCount" : 4,
             //      "iType" :2,
             //      "iHot" : 0

             //   }}


             //  {info:'测试 Id2Phone',port:23241,path:'/?sMethod=Id2Phone&sBusiness=UserData'
             // , post_data:{
             //      "ids": [1560167]

             //   }}

             //   {info:'测试 UpdateDeivce',port:23241,path:'/?sMethod=UpdateDeivce&sBusiness=UserData'
             // , post_data:{
             //      "sAccount": "13411367357",
             //      "sDeviceId": "123456789"

             //   }}


              {info:'测试 CheckDeivce',port:23241,path:'/?sMethod=CheckDeivce&sBusiness=UserData'
             , post_data:{
                  "sAccount": "13411367357",
                  "sDeviceId": "123456789"

               }}

            // {info:'测试 RegByBind',port:23241,path:'/?sMethod=RegByBind&sBusiness=UserRegister'
            //  , post_data:{
            //      "sAccount": "13411367481",
            //      "sCode" : TymLib.md5( "13411367481ttdsu"  ),
            //      "sAccessToken" : "0a66f9119b824d01bc1f38226d44OX55",
            //      "sOpenId" : "20880083774832647447034362311155",
            //      "src" :2

            //    }}



            // {info:'测试 UpdateAccount',port:13241,path:'/?sMethod=UpdateAccount&sBusiness=UserRegister'
            //  , post_data:{
            //      "sAccount": "13411367357",
            //      "sPassword": 'b3f821e872c29b660f737b843f0a667e',
            //      "sNewAccount": '15916319849',
            //      "sCode": TymLib.md5( "15916319849gci"  )
            //    }}

            // {info:'测试 AddDataNoCode',port:30053,path:'/?sMethod=AddDataNoCode'
            //  , post_data:{
            //      "sAccount": "13411367112",
            //      "sNickName": 'suyp',
            //      "sPassword": '123456',
            //      "iSex": 0,
            //      "sProvince": '广东省',
            //      "sCity": '珠海市',
            //      "sSignature": 'ah,ah,ah',
            //      "sEmail": '123456@163.com',
            //      "sHeadImage": 'ffffffff',
            //      "iPhoneAuth": 1,
            //       "lPlatformID" : 100001,
            //       "lPushManId" : 50006
            //    }}

  //           {info:'测试 Update',port:23241,path:'/?sMethod=UpdateData&sBusiness=UserRegister'
  //            , post_data:{ 
  // "sPrimaryKey": 50037,
  // "sNickName": "  123",
  // "iSex": 0,
  // "sProvince": "广东省change",
  // "sCity": "珠海市change",
  // "sSignature": "change_ah,ah,ah",
  // "sHeadImage" : "headchange",
  // "sEmail" : "emailchange"
  // }}


        //    {info:'测试 PhoneCode',port:30053,path:'/?sMethod=PhoneCode'
      //      , post_data:{ iServerID : 1, sAccount : "13823126565"}},

            // {info:'测试 ForgetPassword',port:13241,path:'/?sMethod=ForgetPassword&sBusiness=UserRegister'
            //  , post_data:{
            //      "sAccount" : "13411367357",
            //      "sCode" : "667470",
            //      "sNewPassword": 'b3f821e872c29b660f737b843f0a667e'
            //    }}

         //   {info:'测试 Attention',port:13241,path:'/?sMethod=Attention&sBusiness=UserRegister'
          //  , post_data:{ lPlatformId : 7600345, lUserId : 60419}}


           //{info:'测试 ThirdUser',port:23241,path:'/?sMethod=ThirdUser&sBusiness=UserRegister'
           // , post_data:{ lPlatformId : 7600345, lUserId : 60419}}
        ]

//当前用例列表
var units = [];
//在这里配置当前用例
Concat(unitsShop);//加入用例

//当前用例序号
var index = 0;
// 请求的结果回调
var CallBackRes = function (res){
    TymLib.Log("info","statusCode:" + res.statusCode);
    res.setEncoding('utf8');
    res.on('data',function(chunk){

        TymLib.Log("info","res:"+ chunk);

        var sDeAes = TymLib.DeAesCipher(chunk);
        TymLib.Log("info","des:"+ sDeAes);
        var entity = JSON.parse(sDeAes);

        if( entity.list != undefined){
            TymLib.Log("info","iRet:" + entity.iRet);
            for(var i  = 0 ; i < entity.list.length ; i ++){
                TymLib.Log("info",entity.list[i]);
            }
        }else{
            TymLib.Log("info",entity);
        }

        index ++;
        if( index < units.length){
            Request();
        }
    });
}

function Request(){
    option.path = units[index].path;
    option.port = units[index].port;
    TymLib.Log("info","\n"   + units[index].info + "\npath:" + option.path  );
    var sAes = TymLib.AesCipher(JSON.stringify(units[index].post_data));

    var sCurTime = TymLib.GetCurSecond();
    var sAppid = "425388c9c928c3d36856e67b6d86cc11";
    var appKey = TymLib.GetAppKeyByAppid(sAppid);   

    var sSid  = TymLib.md5(appKey.sAppKey  + sAes  + sCurTime+  sAppid );

    option.path += "&sAppid=" + sAppid + "&sTimeStamp=" + sCurTime + "&sSign=" + sSid ;
    TymLib.Log("info","path:" + option.path);

    TymLib.Log("info","time:" + TymLib.GetCurMilliSecond());
    option.headers["Content-Length"] = sAes.length;
    var req = https.request(option,CallBackRes);
    req.on('error',function (err){
        TymLib.Log("info","Request error:" + err.code);
    });
    
   // var sAes = "iA9dnxorkrMpqtvNc5LH/0nWxQC3jNXsGHb1IKxgeYP39tepXGYSru8SYkL1OsBG9wlnZLYl9yzdsLO/SXaOMRA6dQWeDN016Da8zHiG8p210bUH8NdQCUIQsosODITugzPVOJiofYhvrmVH0vM4H1wnsqvnNhKtHy9jN+M5sTxA86RQ6lawt8rvhRLc8Lgh/6wt7lfhGz+GtErz8SUXaUzFVSXvrmOnVPL6WzwMTIE4ICu4F1hDUOntxEEXyvAp54mNNQ9dNb8FxvAak3bq3Jl2Sc4po7bbvOAD3+mQTTcp+2Zgtkfncn7fT0dlO6FpDjr5FdRcbw9OdP8G5cc622mVGZE/g9gNaSQIphmSnpNfedIMlGybSp7DikS1ycuOkk+6sumpAOe6XQ9+PK5lAF+PY6fqtk/JgLklBKfzb/UbD135bVpHko2MimEYxWJ9yzkl91iM7ZfavaIujlpWqgoGBKlEuxn6PTtYRtwA7lIoXFG8sG4fBsb927/ZRalrFJ5LkINqcACtm9vkjHaLiPWeQGLsGEolMWy1Mh4tdfo9oKaiTlP97AnTCUzmy++dyq2SGqmh8Bk/5aietln8DqFT4a8zdtukWBYH8zMf9CvRziMtAzNv9X/acPX5iadagiswLuM8VJFvS9RpMfGs+QRdN5Pcj84Qlg3wy1JTYdJ/89pFNh7OFfP14r/k5DnUtOpDrXMMo3MjNNMnKwob1AyA36kK2gtDSMtji12l0O3srhQqVOfOap/nhchSlkSiI65gsDJPVcY7yJRSfgNkt7vjvXIc4Nu87C2s7mwwqganb0O0ZD/fYJjpTYE8xn2mboo3gCRKUQ5pSk4/ssmbgP/zZafEN2UG71T+8L8AXOEzcsKpT6fxNDK+GWgzHWDlB0B0YQYaYyZMPblKefZmGZSRYJBHAUG1CC4SVn4FtbgM6ffWjRUg003ywY3Xw7rlwsJfL5wDrsyY4S1yl0B+cDBjn9dNSjYl2fmxuQEpdXttYOiMch/lxSNofp1JgtVPjx8+yBu0pYOu2ZRmUoJAQ/Kr6E2Itqtd5qRfVf4LDQ75g+gFyI4NMopS8TXqrN5OQknbSvViBbmSefsyuc8yelz/T3bPvJTgnWSBf8N0xPJkJQjshBJWo/OT2q1cLWsYLmHqXeldmryrRz2Gs0b5yO6WFkBMJKbMl7wVGrMqO+0=";
    req.write(sAes + "\n");
    req.end();
}

function Main(){
 // var sA =TymLib.AesCipher("abc");
 //  var sD = TymLib.DeAesCipher(sA);
  // TymLib.Log("info","sA:" + sA + "sD:" + sD);
  //  TymLib.Log("info","\nStart Test...\n");
    Request();
}

function Concat(u){
    for(var i = 0 ; i < u.length ; i ++){
        units.push(u[i]);
    }
}

//开始测试
TymLib.SetLog("test","debug");
TymLib.Log("info","start");
for(var i = 0 ; i < 1 ; i++){
   Main();
}

// urlde = urlencode.decode("=sWeI6AQv7m2GQX8xrcr%2B0kxcsJO0F929aekYu0jn4fIMS43ZyZEEeUDFY69mb8KA6Ukui300EXYEwR6N%2BfltS2zNUFpmmOQg6/QYenAnd4ai4iqAqcvxWflf4JFdRsCxjWqojQfoWhhzH4G4eaquJA%3D%3D");
// aesde = TymLib.DeAesCipher(urlde);
// TymLib.Log("info","aesde:" + aesde);
TymLib.Log("info","end");


