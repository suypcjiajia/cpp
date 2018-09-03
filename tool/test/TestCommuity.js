var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
var urlencode = require('urlencode');
//https请求参数
var option = {
  // hostname: '120.25.129.101',
  hostname: '192.168.168.69',
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

        // {info:'测试 SendPublish',port:23241,path:'/?sMethod=SendPublish&sBusiness=Community'
        // , post_data:{ userId : 50037, content : "5689", pic: [],authority : 0}}

         //  {info:'测试 GetPublish',port:23241,path:'/?sMethod=GetPublish&sBusiness=Community'
       //  , post_data:{ id:"1513565164374759"}}

         {info:'测试 GetShouYe',port:23241,path:'/?sMethod=GetShouYe&sBusiness=Community'
         , post_data:{ userId:50037, page:1}}

      //   {info:'测试 Hudong',port:23241,path:'/?sMethod=Hudong&sBusiness=Community'
       //  , post_data:{ userId:50038, type:2, id:"1513565164374759"}}

        // {info:'测试 SendComment',port:23241,path:'/?sMethod=SendComment&sBusiness=Community'
        // , post_data:{ userId:50038, comment:"hello", id:"1513565164374759"}}

        //{info:'测试 ReplyComment',port:23241,path:'/?sMethod=ReplyComment&sBusiness=Community'
        // , post_data:{ userId: 50039, touserId: 50038 , id:"1513565164374759", comment:"yes,hello"}},

        // {info:'测试 GetComment',port:23241,path:'/?sMethod=GetComment&sBusiness=CommunityBLL'
        // , post_data:{ page:1, id:"1513565164374759"}}

      //  {info:'测试 ToMeHudong',port:23241,path:'/?sMethod=ToMeHudong&sBusiness=CommunityBLL'
       //  , post_data:{ userId:50037, page:1}}

         //{info:'测试 ClearHudong',port:23241,path:'/?sMethod=ClearHudong&sBusiness=CommunityBLL'
        // , post_data:{ userId:50037}}

       // {info:'测试 GetMyPuslish',port:23241,path:'/?sMethod=GetMyPuslish&sBusiness=CommunityBLL'
        // , post_data:{ userId:50037, page : 1, condition: ""}}

        // {info:'测试 GetCollection',port:23241,path:'/?sMethod=GetCollection&sBusiness=CommunityBLL'
       //  , post_data:{ userId:50038, page : 1}}

       //  {info:'测试 MyInfo',port:23241,path:'/?sMethod=MyInfo&sBusiness=CommunityBLL'
        // , post_data:{ userId:50038}}



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


