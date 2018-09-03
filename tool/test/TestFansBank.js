var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
var urlencode = require('urlencode');
//https请求参数
var option = {
  //hostname: 'im.ttdomo.com',
 // hostname: '120.76.26.120',//外网测试
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


       //  {info:'测试 GetBankInfo',port:23241,path:'/?sMethod=GetBankInfo&sBusiness=FansBank'
      //   , post_data:{ lUserId : 976139, iType :2}}


      //     {info:'测试 GetMarketOnSalling',port:23241,path:'/?sMethod=GetMarketOnSalling&sBusiness=FansBank'
       //    , post_data:{ lUserId : 402313, iType : 1}}


        // {info:'测试 GetMarketK',port:23241,path:'/?sMethod=GetMarketK&sBusiness=FansBank'
        // , post_data:{ lUserId: 209392, Id : 206103, iType : 2}}


        //  {info:'测试 GetPageMarket',port:23241,path:'/?sMethod=GetPageMarket&sBusiness=FansBank'
        //  , post_data:{ iPage : 1, iCount :5, iType : 1}}


       //  {info:'测试 Sell',port:23241,path:'/?sMethod=Sell&sBusiness=FansBank'
      //   , post_data:{ Id : 500774, iType : 2, iLots: 5,sPassWord : 'd0970714757783e6cf17b26fb8e2298f'}}


        //{info:'测试 GetMarketValue',port:23241,path:'/?sMethod=GetMarketValue&sBusiness=FansBank'
        //  , post_data:{ Id : 111000, iType : 1}}


     //   {info:'测试 SearchMarket',port:23241,path:'/?sMethod=SearchMarket&sBusiness=FansBank'
       //   , post_data:{ sCondition : '熊猫', iType : 1}}


         // {info:'测试 GetSelling',port:23241,path:'/?sMethod=GetSelling&sBusiness=FansBank'
         // , post_data:{ lUserId: 50037,iPage : 1, iCount : 5, iType : 2}}


       // {info:'测试 SearchSelling',port:23241,path:'/?sMethod=SearchSelling&sBusiness=FansBank'
        //  , post_data:{ iPage : 1, iCount : 5, iType : 1, sCondition : '111'}}



        {info:'测试 Buy',port:23241,path:'/?sMethod=Buy&sBusiness=FansBank'
         , post_data:{ sOrderNo : '44445563362233535',  iType : 2, lBuyId : 50037,lSellId : 39 ,iLots : 1, iPayType : 22,sPassWord : 'fd31c84d7ed615fad84a7811ded732d297ea31e2'}}


           // {info:'测试 InnerCreate',port:23241,path:'/?sMethod=InnerCreate&sBusiness=FansBank'
           //  , post_data:{ sOrderNo : '54445563362233558', lBuyId : 50037, lSellId:50222,iLots : 1, 
           //   dPrice : 5.0,dCoin:0.0}}

         // {info:'测试 InnerBuy',port:23241,path:'/?sMethod=InnerBuy&sBusiness=FansBank'
         //  , post_data:{ sOrderNo : '54445563362233557', lBuyId : 50037,lSellId:50222,iLots : 1,
         //   sPassWord : 'fd31c84d7ed615fad84a7811ded732d297ea31e2',dPrice : 5.0,dCoin:0.0}}

        //   {info:'测试 FansGive',port:23241,path:'/?sMethod=FansGive&sBusiness=FansBank'
         //  , post_data:{ lBuyId : 221549, iLots : 1, lSellId : 181}},


        //   {info:'测试 OrderState',port:23241,path:'/?sMethod=OrderState&sBusiness=FansBank'
       //   , post_data:{ orderNo : '44445563362233586'}}


         // {info:'测试 GetPageOrder',port:23241,path:'/?sMethod=GetPageOrder&sBusiness=FansBank'
         // , post_data:{ iType : 1, iState : 3, lUserId : 206103,iPage : 1, iCount :5 }}


          // {info:'测试 CancelOrder',port:23241,path:'/?sMethod=CancelOrder&sBusiness=FansBank'
          // , post_data:{ iType : 1, Id : 7600767, sPassWord : 'e10adc3949ba59abbe56e057f20f883e'}}


          // {info:'测试 Search',port:23241,path:'/?sMethod=Search&sBusiness=FansBank'
         //  , post_data:{ iPage : 1, iCount : 5, iType : 2, sCondition : '安卓'}}


          // {info:'测试 GetTradeById',port:23241,path:'/?sMethod=GetTradeById&sBusiness=FansBank'
          // , post_data:{ iPage : 1, iCount : 5, iType : 1, Id : 7600767}}

        //  {info:'测试 GetAllTrade',port:23241,path:'/?sMethod=GetAllTrade&sBusiness=FansBank'
         // , post_data:{ iPage : 1, iCount : 5, iType : 1}}


        //  {info:'测试 RedPoin',port:23241,path:'/?sMethod=RedPoin&sBusiness=FansBank'
         // , post_data:{ lLasts :[ { iType : 1,lLastTime : 1504860447968583},{iType : 2,lLastTime : 1504860221203349}]}}


       //  {info:'测试 GetOrderByManage',port:23241,path:'/?sMethod=GetOrderByManage&sBusiness=FansBank'
       //  , post_data:{ type : 1, state : 0,page : 1, count :2 }}

      //   {info:'测试 GetConfig',port:23241,path:'/?sMethod=GetConfig&sBusiness=FansBank'
    //   , post_data:{id:1}}

        // {info:'测试 SetConfig',port:23241,path:'/?sMethod=SetConfig&sBusiness=FansBank'
      //   , post_data:{ autoOPenPeragent: "8"}}






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


