var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
var urlencode = require('urlencode');
//https请求参数
var option = {
   hostname: '120.25.129.101',
  //hostname: '192.168.168.69',
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


        //  {info:'测试 JoinIn',port:23241,path:'/?sMethod=JoinIn&sBusiness=GroupMember'
       //  , post_data:{ lMemberIdLst : [50037,50069], lGroupId : 1, lInviteId : 50062}}
//
      //    {info:'测试 JoinOut',port:23241,path:'/?sMethod=JoinOut&sBusiness=GroupMember'
      //    , post_data:{ lUserId : 50037,  lGroupId : 1}}

       //   {info:'测试 MyGroup',port:23241,path:'/?sMethod=MyGroup&sBusiness=GroupMember'
        //  , post_data:{ lUserId : 50037,  lUpTime : 0}}


         // {info:'测试 MemberById',port:23241,path:'/?sMethod=MemberById&sBusiness=GroupMember'
        //  , post_data:{ lGroupId : 56}}

        // {info:'测试 SetNickName',port:23241,path:'/?sMethod=SetNickName&sBusiness=GroupMember'
        //  , post_data:{ lUserId : 50037 ,lGroupId : 1 , sNickName : 'chang_name'}}

     //   {info:'测试 KickOut',port:23241,path:'/?sMethod=KickOut&sBusiness=GroupMember'
      //    , post_data:{ lUserId : 50034 ,lGroupId : 23 , lMemberIds : [50061,50062]}}


     //   {info:'测试 MemberBylotsId',port:23241,path:'/?sMethod=MemberBylotsId&sBusiness=GroupMember'
       //   , post_data:{ lGroupIdLst : [{lGroupId : 1, lUpTime : 1484210868212164},{lGroupId:2 , lUpTime : 0}]}}


          {info:'测试 ModifyMySetting',port:23241,path:'/?sMethod=ModifyMySetting&sBusiness=GroupMember'
          , post_data:{ lUserId:161323,lGroupId : 102, iMsgMode: 2}}



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
// for(var i = 0 ; i < 1 ; i++){
    Main();
// }

 TymLib.Log("info","end");



