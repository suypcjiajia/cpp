var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
//https请求参数
var option = {
  
   hostname: 'im.ttdomo.com',
  // hostname: '127.0.0.1',
   //hostname :'47.75.132.64', //hong kong 
    port: 0,
    path: '',
    method: 'POST',
    //passphrase: 'tymtym',
   // pfx: fs.readFileSync("../../certs/client.pfx"),
    rejectUnauthorized: false,
        headers: {
     "Content-Length": 0,
     "sToken": "nNl39CEzfR8NZt28yxG6sA==",
     "versions" : "3.9.2",
     "clientType" :"01"

 }
}

//测试用例列表
var unitsShop =
        [
         //  {info:'测试 Pass',port:23241,path:'/?sMethod=Pass&sBusiness=UserQuery'
          //  , post_data:{ lUserId : 50037, sPassword : '0a69d8c89e305fe071e2057fae7d04b0123'}},



             // {info:'测试 BandThirdId',port:23241,path:'/?sMethod=BandThirdId&sBusiness=UserQuery'
             // , post_data:{ sCode :123456 ,sAccount : "13411367357", 
             // sPassword : "0a69d8c89e305fe071e2057fae7d04b0", sWeixinNum : "123456890066666"}},


            //   {info:'测试 MyShops',port:23241,path:'/?sMethod=MyShops&sBusiness=UserQuery'
          //  , post_data:{ lUserId : 93525}}

        //  {info:'测试 UserLogin',port:23241,path:'/?sMethod=UserLogin&sBusiness=UserQuery'
         // , post_data:{ sAccount : "13411118979", sPassword : "0a69d8c89e305fe071e2057fae7d04b0"}}

          // {info:'测试 LoginByCode',port:23241,path:'/?sMethod=LoginByCode&sBusiness=UserQuery'
           // , post_data:{ sAccount : "13411367357", sCode : "656679"}}

            //{info:'测试 GetVips',port:23241,path:'/?sMethod=GetVips&sBusiness=UserData'
            //, post_data:{ iPage : 1, iCount : 3}}

          //  {info:'测试 GetGrade',port:23241,path:'/?sMethod=GetGrade&sBusiness=UserData'
          //  , post_data:{ sAccount : "15946216010"}}

            //{info:'测试 GetRegisters',port:23241,path:'/?sMethod=GetRegisters&sBusiness=UserData'
           // , post_data:{ sDay : "2017-01-01~2017-03-01"}}

            //{info:'测试 Pass',port:23241,path:'/?sMethod=Pass&sBusiness=UserData'
            //, post_data:{ sAccount : "13411367357", sPassword : "123456"}}

           // {info:'测试 Auto',port:23241,path:'/?sMethod=Auto&sBusiness=UserData'
          //  , post_data:{ sAccount : "13411367359", lPushManId : 50037,lPlatformId: 100012}}


           // {info:'测试 GetPushUpChain',port:23241,path:'/?sMethod=GetPushUpChain&sBusiness=UserData'
           // , post_data:{ sAccount : "13411367357",iLevel : 5}}
          //  {info:'测试 SetGrade',port:23241,path:'/?sMethod=SetGrade&sBusiness=UserData'
           // , post_data:{ sAccount : "15967710678", iLevel : 1}}

            // {info:'测试 MyFSLevelPushCount',port:23241,path:'/?sMethod=MyFSLevelPushCount&sBusiness=UserQuery'
           // , post_data:{ lUserId : "50012"}}

            //{info:'测试 MyPushAndLoginedCount',port:23241,path:'/?sMethod=MyPushAndLoginedCount&sBusiness=UserQuery'
            // , post_data:{ lUserId : "517169"}}

           // {info:'测试 Sign',port:23241,path:'/?sMethod=Sign&sBusiness=UserQuery'
            //, post_data:{ lUserId : "93525"}}


          //  {info:'测试 GetFans',port:23241,path:'/?sMethod=GetFans&sBusiness=UserQuery'
         //   , post_data:{ Id : "7616554", iType : 1 , iPage :1 , iCount : 8}}

          //  {info:'测试 GetMakedFans',port:23241,path:'/?sMethod=GetMakedFans&sBusiness=UserQuery'
          //   , post_data:{iPage :1 , iCount : 8, sAccount :"12"}}

          //  {info:'测试 GetVideo',port:23241,path:'/?sMethod=GetVideo&sBusiness=UserQuery'
         //   , post_data:{ userId : "50037"}}

     //       {info:'测试 LoginTime',port:13241,path:'/?sMethod=LoginTime&sBusiness=UserState'
      //      , post_data:{ lUserId : "60419"}}

           {info:'测试 GetUserInfo',port:23241,path:'/?sMethod=GetUserInfo&sBusiness=UserQuery'
            , post_data:{
                "sAccount": "18318035317"
              }}


            // {info:'测试 Perfect',port:23241,path:'/?sMethod=Perfect&sBusiness=UserQuery'
            // , post_data:{
            //     "sAccount": "18818580245"
            // }}

            //  {info:'测试 GetUserInfoByGciID',port:13241,path:'/?sMethod=GetUserInfoByGciID&sBusiness=UserQuery'
            //   , post_data:{
            //       "sGciID": "gciaccd"
            //     }},

              //   {info:'测试 GetUserInfoByID',port:23241,path:'/?sMethod=GetUserInfoByID&sBusiness=UserQuery'
              // , post_data:{
              //     "lUserId": 60419
              //   }}
              //              {info:'测试 GetUserInfoByPhone',port:13241,path:'/?sMethod=GetUserInfoByPhone&sBusiness=UserQuery'
              // , post_data:{
              //     "sAccount": "13411367357"
              //   }},

           //             {info:'测试 MyPushMan',port:13241,path:'/?sMethod=MyPushMan&sBusiness=UserQuery'
           // , post_data:{ lUserId : 60419}},
           //              {info:'测试 MyPushCount',port:13241,path:'/?sMethod=MyPushCount&sBusiness=UserQuery'
           //  , post_data:{  sAccount : "13411367357"}}

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

            // {info:'测试 UpdateData',port:23241,path:'/?sMethod=UpdateData&&sBusiness=UserRegister'
            //  , post_data:{
            //      "lUserId" : 50037,
            //      "sPrimaryKey" : 50037,
            //      "sNickName": 'tttttt',
            //      "sBirthDay" : '1996-1-12',
            //      'sSignature' : '555555777777'
            //    }}




            // {info:'测试 ForgetPassword',port:13241,path:'/?sMethod=ForgetPassword&sBusiness=UserRegister'
            //  , post_data:{
            //      "sAccount" : "13823126565",
            //      "sCode" : "815731",
            //      "sNewPassword": 'b3f821e872c29b660f737b843f0a667e'
            //    }}


            // {info:'测试 GetUserInfoList',port:13241,path:'/?sMethod=GetUserInfoList&sBusiness=UserQuery'
            //  , post_data:{
            //      "lUserIdLst" : [ { lUserId:60421 } , { lUserId:60422 } ]
            //    }}
           //  {info:'测试 IsRegistered',port:13241,path:'/?sMethod=IsRegistered&sBusiness=UserQuery'
         //   , post_data:{ sAccount : "13411367358"}}
        ];

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

        var sDeAes = TymLib.DeAesCipher(chunk);
        TymLib.Log("info","res:"+ sDeAes);
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

    var sCurTime = TymLib.GetCurSecond() ;
    var sAppid = "425388c9c928c3d36856e67b6d86cc11";
    var appKey = TymLib.GetAppKeyByAppid(sAppid);

    

    var sSid  = TymLib.md5(appKey.sAppKey  + sAes  + sCurTime+  sAppid );

    option.path += "&sAppid=" + sAppid + "&sTimeStamp=" + sCurTime + "&sSign=" + sSid ;
    TymLib.Log("info","path:" + option.path);

    option.headers["Content-Length"] = sAes.length;
    var req = https.request(option,CallBackRes);
    req.on('error',function (err){
        TymLib.Log("info","Request error:" + err.code);
    });
    TymLib.Log("info","post aes:" + sAes);
    req.write(sAes + "\n");
    req.end();
}

function Main(){
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

TymLib.Log("info","end");




