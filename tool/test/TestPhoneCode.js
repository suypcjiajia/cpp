var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
//https请求参数
var option = {

   hostname: '127.0.0.1',
   //hostname: 'sms.ttdomo.com' ,//负载
    port: 0,
    path: '',
    method: 'POST',
    passphrase: 'tymtym',
    pfx: fs.readFileSync("../../certs/client.pfx"),
    rejectUnauthorized: false,
    headers: {
     "Content-Length": 0
 }
}

//测试用例列表
var unitsShop =
        [
             {info:'测试  CodeTTD',port:23240,path:'/?sMethod=CodeTTD&sBusiness=phonesms'
           , post_data:{ sPhone:'13411367357',sType: '18',sMsg : '123,345,789'}}

           // {info:'测试  CodeTTDv2',port:23240,path:'/?sMethod=CodeTTDv2&sBusiness=phonesms'
          //  , post_data:{ sPhone:'13411367357',sType: '1',sCode: "9580"}},



          // {info:'测试  SendShopOpen',port:23240,path:'/?sMethod=SendShopOpen&sBusiness=phonesms'
          // , post_data:{ sPhone:'15992615255',sChannel : '1',sMsg : 'a,b'}}



           // {info:'测试  UseCode',port:23240,path:'/?sMethod=UseCode&sBusiness=phonesms'
            //, post_data:{ sPhone:'13411367357',sCode : '143885'}},

             // {info:'测试  SendConsumer',port:13240,path:'/?sMethod=SendConsumer'
             // , post_data:{ sPhone:'13411367357',sChannel : '1', sMsg : 'zhuhai店,0.01现金,123456789'}}



        ];

//当前用例列表
var units = [];
//在这里配置当前用例
Concat(unitsShop);//加入商家用例

//当前用例序号
var index = 0;
// 请求的结果回调
var CallBackRes = function (res){
    TymLib.Log("info","statusCode:" + res.statusCode);
    res.setEncoding('utf8');
    res.on('data',function(chunk){

        TymLib.Log("info","res:"+ chunk);
        var sDeAes = TymLib.DeAesCipher(chunk);
        TymLib.Log("info","de_res:"+ sDeAes);
        var entity = JSON.parse(sDeAes);

        TymLib.Log("info",TymLib.GetCurMilliSecond());
        if( entity.list != undefined){
            //TymLib.Log("info","iRet:" + entity.iRet);
            for(var i  = 0 ; i < entity.list.length ; i ++){
               // TymLib.Log("info",entity.list[i]);
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

   var sAes = JSON.stringify(units[index].post_data);
   sAes = TymLib.AesCipher(sAes);
    TymLib.Log("info","postdata:" + sAes);

    var sCurTime = TymLib.GetCurSecond();
    var sAppid = "425388c9c928c3d36856e67b6d86cc11";
    var sSid  = TymLib.md5("53b7a63d019de6ba32ed2e7fe38dfa72" + sAes + sCurTime + sAppid);

    option.path += "&sAppid=" + sAppid  + "&sSign=" + sSid + "&sTimeStamp=" + sCurTime;
    TymLib.Log("info","path:" + option.path);

    TymLib.Log("info","time:" + TymLib.GetCurMilliSecond());



   //sAes = urlencode.encode(sAes);
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
    // var sAes = TymLib.AesCipher("12345678901234567890a");
    // TymLib.Log("info","postdata:" + sAes);

TymLib.Log("info","start");
for(var i = 0 ; i < 1 ; i++){
   Main();
}
TymLib.Log("info","end");




