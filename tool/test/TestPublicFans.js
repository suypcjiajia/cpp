var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
var urlencode = require('urlencode');
//https请求参数
var option = {
  // hostname: '120.25.129.101',
  hostname: '192.168.168.158',
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


        //  {info:'测试 Tick',port:23241,path:'/?sMethod=Tick&sBusiness=PublicFans'
        //  , post_data:{ sPayPhone : 13726277189, lPlatformId : 7608177}},

         //  {info:'测试 MyPublic',port:23241,path:'/?sMethod=MyPublic&sBusiness=PublicFans'
         // , post_data:{ lUserId : 50037}}


          {info:'测试 Luky',port:23241,path:'/?sMethod=Luky&sBusiness=PublicFans'
          , post_data:{ bRand: false,lUserId : 50037 , Id : 111000 ,dMoney : 1.00, iNum : '2' ,
            sPassWord : "123456" ,sOrderNo : "4116952822722323", iDest : 1 ,bRand : true}}

        //{info:'测试 UnPackLk',port:23241,path:'/?sMethod=UnPackLk&sBusiness=PublicFans'
        //  , post_data:{ lUserId : 50037 ,sKey : "45869528227223"}}

         // {info:'测试 PublicData',port:23241,path:'/?sMethod=PublicData&sBusiness=PublicFans'
        //  , post_data:{ Id : 111000}}

         // {info:'测试 Detail',port:23241,path:'/?sMethod=Detail&sBusiness=PublicFans'
         // , post_data:{ Id : 111000}}


        //  {info:'测试 Chat',port:23241,path:'/?sMethod=Chat&sBusiness=PublicFans'
         // , post_data:{ lUserId: 50009, lToUserId: 50037,Id : 111000, iType: 1 , sContent : "22222"}}



        //  {info:'测试 GetInfo',port:23241,path:'/?sMethod=GetInfo&sBusiness=PublicFans'
        //  , post_data:{ lUserId: 209074, lAddTime: 0, iCount: 100 }}

         //  {info:'测试 ConsumerPage',port:23241,path:'/?sMethod=ConsumerPage&sBusiness=PublicFans'
         // , post_data:{ Id : 100012, iPage :1}}


         // {info:'测试 UnAttention',port:23241,path:'/?sMethod=UnAttention&sBusiness=PublicFans'
         // , post_data:{ lUserId : 50037, Id: 100012}}





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


