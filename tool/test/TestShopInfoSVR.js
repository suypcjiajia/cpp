
var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
var urlencode = require('urlencode');
//https请求参数
var option = {

   hostname: '171.17.1.126',
  // hostname: 'shop.ttdomo.com', //负载IP 
    port: 0,
    path: '',
    method: 'POST',
    passphrase: 'tymtym',
    pfx: fs.readFileSync("../../certs/client.pfx"),
    rejectUnauthorized: false,
    headers: {
     "Content-Length": 0,
     "Content-type": 'application/x-www-form-urlencoded'
 }
}

//测试用例列表
var unitsShop =
        [


          //    {info:'测试 商家 CreateShopApp',port:3314,path:'/?sMethod=CreateShopApp&sBusiness=ShopInfo'
          //     , post_data:{
          //       "lLoginId" : 112132 ,
          //         "lUserId": 112132 ,
          //         "sPartnerPhone" : "131",
          //         "iCoinType" : 1,
          //         "sInerPhoto": '',
          // //        "sPlatformId": "1601937",
          //   //      "sAccount": "tym177_1",
          //    //     "sPassWord": 792619,
          //         "iOffLineDisCount": 95,
          //         "sShopName": "阿曾甜品1",
          //         "sCompanyName": "阿曾甜品1",
          //         "sShopPhone": "13138103805",
          //         "lServiceUserId": 5,
          //         "sSevvicePhone": "131",
          //         "sSevicePhone" : "131",
          //         "sCity": "深圳南山区",
          //         "sAddress": "广东省",
          //         "dGpsLat": "116.557474",
          //         "dGpsLon": 23.275771,
          //         "sShopType": "美食",
          //         "sBusiness": "美食",
          //         "sHours": "",
          //         "iAwardingPoints": "7",
          //         "iAvgConsume": "90",
          //         "sPosNum": "",
          //         "iProrate": "1",
          //         "iShowNearby": "6",
          //         "sLicensePhoto": [""],
          //         "sIdPhoto": [""],
          //         "sShopFrontPhoto": [],
          //         "sOpenManType" : 1,
                  
          //         "sOtherCerPhoto": [""],
          //       }}

        //   {info:'测试 商家 获取',port:3314,path:'/?sMethod=OpenNum&sBusiness=ShopInfo'
       // , post_data:{ sOpenManType : 1,lUserId: 50037}}


            // {info:'测试 商家 UpdateData',port:2314,path:'/?sMethod=UpdateData&sBusiness=ShopInfo'
            //  , post_data:{
            //      "sPrimaryKey": "100002",
            //      "iOffLineDisCount": 98,
            //      "sShopName": "sShopName测试",
            //      "sCompanyName": "sCompanyName测试",
            //      "sShopPhone": '13411367357',
            //      "lPerAgentId": 5000199,
            //      "lServiceUserId": 50018,
            //      "sSevvicePhone": "sSevvicePhonechange",
            //      "sCtiy": "sCtiychange",
            //      "sAddress": "深圳市龙华区清湖",
            //      "dGpsLat": 113.888,
            //      "dGpsLon": 22.8888,
            //      "sShopType": "sShopTypechange",
            //      "sBusiness": "sBusinesschange",
            //      "sHours": "sHourschange",
            //      "iAwardingPoints": "11",
            //      "iAvgConsume": "18",
            //      "sPosNum": "sPosNumchange",
            //      "iProrate": "9",
            //      "iShowNearby": "888",
            //      "sLicensePhoto": "sLicensePhotochange",
            //      "sIdPhoto": "csIdPhotohange",
            //      "sShopFrontPhoto": "sShopFrontPhotochange",
            //      "sShopDetailPhoto": "sShopDetailPhotochange",
            //      "iAuth" : 3,
            //      "iVerify" : 1,
            //      "sTradeMark" : "http://xxxxxx/xx.jpg" 
            //    }},


      // {info:'测试 商家 获取',port:3314,path:'/?sMethod=GetEntity&sBusiness=ShopInfo'
      // , post_data:{ lPlatformId : 7600767}}
         //  {info:'测试 商家 获取',port:2314,path:'/?sMethod=ShopsByPerAgent&sBusiness=ShopInfo'
        //   , post_data:{ lUserId : 111359}},

            // {info:'测试 商家 AddBank',port:3314,path:'/?sMethod=AddBank&sBusiness=ShopInfo'
            //  , post_data:{ lPlatformId : 100013, sName : '百果园食品有限公司', sAccount : '44444444444444444', 
            // sBranch :'珠海工商银行拱北支行',sBank : '工商银行'}},

          //  {info:'测试 商家 GetBank',port:3314,path:'/?sMethod=GetBank&sBusiness=ShopInfo'
          // , post_data:{ lPlatformId : 100013}}

         //  {info:'测试 商家 Setting',port:3314,path:'/?sMethod=Setting&sBusiness=ShopInfo'
         //  , post_data:{ lPlatformId : 100012,  iDelivery : 1,iPark : 1 , iWifi : 1 , sStartTime : '2017-01-02',iRecommend : 1}},

         // {info:'测试 商家 RecommendShops',port:3314,path:'/?sMethod=RecommendShops&sBusiness=ShopInfo'
         //   , post_data:
         //   {bStar:false, dGpsLat:22.224243000, sShopType:'', sShopName:'', iCount:10, sCity:'珠海香洲区', sArea:'香洲区', sTown:'珠海', iPage:1, dGpsLon:113.536414000, iEnumDiscount:-1, iAuth:-1}
         // }

           //{info:'测试 商家 GetNearBy',port:3314,path:'/?sMethod=GetNearBy&sBusiness=ShopInfo'
         // , post_data:{ iPage : 1,iCount : 10,dGpsLat : 22.662129,dGpsLon : 114.044564  , sCity :'龙华', bStar : true,sTown:'',sArea:'龙华'}}

        //  {info:'测试 商家 GetQuikMark',port:2314,path:'/?sMethod=GetQuikMark&sBusiness=ShopInfo'
         // , post_data:{ lUserId : 50037}}

          //{info:'测试 商家 ShopPerfect',port:2314,path:'/?sMethod=ShopPerfect&sBusiness=ShopInfo'
          //, post_data:{ lUserId : 50037}}

          {info:'测试 商家 ModifyState',port:3314,path:'/?sMethod=ModifyState&sBusiness=ShopInfo'
         , post_data:{ lPlatformId : 7600767, iFansTrad:0}}



        // {info:'测试 商家 GetClass',port:3314,path:'/?sMethod=GetClass&sBusiness=ShopInfo'
         //, post_data:{ sCity : "", sProvince : "吉林省"}}

         // {info:'测试 商家 Products',port:2314,path:'/?sMethod=Products&sBusiness=ShopInfo'
        //  , post_data:{ iPage : 1,sSearch : '123' }}

       // {info:'测试 商家 GetObjects',port:2314,path:'/?sMethod=GetObjects&sBusiness=ShopInfo'
        //  , post_data:{ lUserId : 1000 }}

        //  {info:'测试 商家 GetShopsByObjest',port:2314,path:'/?sMethod=GetShopsByObjest&sBusiness=ShopInfo'
        //  , post_data:{ iPage : 1,iCount : 10,dGpsLat : 22.662129,dGpsLon : 114.044564  , sCity :'龙华' ,Id : 3}}

        //  {info:'测试 商家 GetLotsPlatfrom',port:3314,path:'/?sMethod=GetLotsPlatfrom&sBusiness=ShopInfo'
        //   , post_data:{ lPlatformIds : [100012,100013],dGpsLat : 22.2700 ,dGpsLon : 113.5500}}

         //  {info:'测试 商家 ModifyPhone',port:3314,path:'/?sMethod=ModifyPhone&sBusiness=ShopInfo'
         //  , post_data:{ lPlatformId : 100012, sShopPhone : '13411367357'}}

       //   {info:'测试 商家 GetSetting',port:3314,path:'/?sMethod=GetSetting&sBusiness=ShopInfo',
        //    post_data:{ lPlatformId : 100013}}
//
    //              {info:'测试 商家 Doc',port:3314,path:'/?sMethod=Doc&sBusiness=ShopInfo',
   //         post_data:{ lPlatformId : 100013}},


//              {info:'测试 商家ModifyPwdexport NODE_PATH=/home/meipay_cpp/src/https/com/lib/ ',port:1314,path:'/?sMethod=ModifyPwd&sBusiness=ShopInfo'
//              , post_data:{ 
//   "sPrimaryKey": 7600021,
//   "sOldPwd": "a60c5d01716b2069ec989311ac62b08f",
//   "sNewPwd": "b2ccbed45f316969e5771da536f4495a"
// }},

           // {info:'测试 商家 获取',port:2314,path:'/?sMethod=GetEntity&sBusiness=ShopInfo'
           //  , post_data:{ lPlatformId : '7600422'}}

         // {info:'测试 商家 WriteCommtent',port:2314,path:'/?sMethod=WriteCommtent&sBusiness=ShopInfo'
         //    , post_data:{ lPlatformId : '100004', iScore : 80, lUserId : 50037 , sHeadImage : 'http:xxxxx.jpg' , sNickName: 'suyp',
         //   sContent: 'good shop ,very good', sPics : ["http://xxxx.jpg"]}}

             // {info:'测试 商家 获取',port:2314,path:'/?sMethod=GetCommtent&sBusiness=ShopInfo'
             // , post_data:{ lPlatformId : '7611842', lAddTime : 0, iCount : 20 }}

           //   {info:'测试 商家 Login',port:2314,path:'/?sMethod=Login&sBusiness=ShopInfo'
          //    , post_data:{ sPrimaryKey : '7600767', sPassWord : '9c8c91f4d5509b97b70ac56d987fd869'}},
             // {info:'测试 商家 HotShops',port:2314,path:'/?sMethod=HotShops&sBusiness=ShopInfo'
             // , post_data:{ dGpsLat : 22.2586, dGpsLon : 112.5682 , sCity : '珠海'}}


         //     {info:'测试 商家 CityShopCount',port:3314,path:'/?sMethod=CityShopCount&sBusiness=ShopInfo'
         //    , post_data:{ sEndTime : "2017-05-06"}}


             //{info:'测试 商家 CashBackShops',port:2314,path:'/?sMethod=CashBackShops&sBusiness=ShopInfo'
             //, post_data:{ iPage: 2, iCount : 5}}

        //   {info:'测试 商家 获取',port:3314,path:'/?sMethod=GetMyShop&sBusiness=ShopInfo'
        //     , post_data:{ lUserId : '50037'}}
         //    {info:'测试 商家 获取',port:3314,path:'/?sMethod=GetState&sBusiness=ShopInfo'
          //   , post_data:{ lPlatformId:100012}}

       // {info:'测试 商家 获取',port:1314,path:'/?sMethod=ApplyDiscount&sBusiness=ShopInfo'
       //     , post_data:{ lPlatformId : 100012, iDisco
      //  unt : 50}},
       //   {info:'测试 商家 获取',port:3314,path:'/?sMethod=GetData&sBusiness=ShopInfo'
      //   , post_data:{ iIndex : 20, iCount : 1, iApplyDiscount: 1 }}

  //  {info:'测试 商家 GetPage',port:3314,path:'/?sMethod=GetPage&sBusiness=ShopInfo'
   //      , post_data:{ iPage : 1, iCount : 2,sShopName:"辽NTA865",dGpsLon : 113.557143, dGpsLat : 22.247054  }}

      //  {info:'测试 商家 GetAgentInfoByPlatformId',port:2314,path:'/?sMethod=GetAgentInfoByPlatformId&sBusiness=ShopInfo'
     //    , post_data:{ lPlatformId: 7601801 }}


   // {info:'测试 商家 ShowProduct',port:2314,path:'/?sMethod=ShowProduct&sBusiness=ShopInfo'
    //      , post_data:{ iPage : 1, iCount : 5,lPlatformId : 100012 }}

        //    {info:'测试 商家 获取',port:1314,path:'/?sMethod=ChangeDiscount&sBusiness=ShopInfo'
        //   , post_data:{ lPlatformId : 100012 , iPass : 2 }}

        //{info:'测试 商家 AddAdv',port:2314,path:'/?sMethod=AddAdv&sBusiness=ShopInfo'
       //      , post_data:{ lPlatformId : '100012', sAdvLink : 'http://xxx.jpg'}}

      //  {info:'测试 商家 GetAdv',port:2314,path:'/?sMethod=GetAdv&sBusiness=ShopInfo'
       // , post_data:{ iPage  : 1, iCount : 2,sShopName : '熊猫'}}

      // {info:'测试 商家 DelAdv',port:2314,path:'/?sMethod=DelAdv&sBusiness=ShopInfo'
      //  , post_data:{ id: 2}}
     // {info:'测试 商家 AdvVerify',port:2314,path:'/?sMethod=AdvVerify&sBusiness=ShopInfo'
       // , post_data:{ id: 1, iVerify : 1}}

     //   {info:'测试 商家 AddProduct',port:2314,path:'/?sMethod=AddProduct&sBusiness=ShopInfo'
      // , post_data:{ lPlatformId: 7604155, sPicLink : 'http://xxxxxx.jpg',sName :'book%note1', dPrice : 100.22}}



       // {info:'测试 商家 GetProduct',port:2314,path:'/?sMethod=GetProduct&sBusiness=ShopInfo'
      //  , post_data:{ iPage: 5, iCount : 5}}


    //  {info:'测试 商家 Check',port:2314,path:'/?sMethod=Check&sBusiness=ShopInfo'
    //    , post_data:{ sPassWord : 'b3f821e872c29b660f737b843f0a667e', lPlatformId : 100088}}

   //  {info:'测试 商家 ModifyCheck',port:2314,path:'/?sMethod=ModifyCheck&sBusiness=ShopInfo'
    //    , post_data:{ sPassWord : 'b3f821e872c29b660f737b843f0a667e', sNewPassWord : 'b3f821e872c29b660f737b843f0a667e', lPlatformId : 100018}}

        //{info:'测试 商家 UpProduct',port:2314,path:'/?sMethod=UpProduct&sBusiness=ShopInfo'
       // , post_data:{ id: 64, sPicLink : "http://bbbbb.jpg"  ,sName : "chang" , dPrice : '99999999.99'}}

        //{info:'测试 商家 ProductVerify',port:2314,path:'/?sMethod=ProductVerify&sBusiness=ShopInfo'
        //, post_data:{ id: 55, iVerify : 1}}

       // {info:'测试 商家 DelProduct',port:2314,path:'/?sMethod=DelProduct&sBusiness=ShopInfo'
      //  , post_data:{ id: 55}}

      //  {info:'测试 商家 OpenbyPerAgent',port:3314,path:'/?sMethod=OpenbyPerAgent&sBusiness=ShopInfo'
      //   , post_data:{ sAccount: "13411367357"}}

        //  {info:'测试 商家 GetEntity',port:2314,path:'/?sMethod=GetEntity&sBusiness=ZoneAgent'
       //  , post_data:{ sCond: "珠海市",iType :4}}

       //     {info:'测试 商家 AddData',port:3314,path:'/?sMethod=AddData&sBusiness=ZoneAgent'
       //  , post_data:{ sAccount: "133333333",lUserId :50037,sPassWord:'5555', sName:'ddd',sUnit:"yyy", sID:"ttt",sCity:"cc"}}


       //   {info:'测试 商家 UpdateData',port:3314,path:'/?sMethod=UpdateData&sBusiness=ZoneAgent'
       //  , post_data:{  sPrimaryKey : "133333333",sPhone: "1234567"}}

       //  {info:'测试 商家 DeleteData',port:3314,path:'/?sMethod=DeleteData&sBusiness=ZoneAgent'
       //  , post_data:{  sPrimaryKey : "133333333"}}

        // {info:'测试 商家 GetEntity',port:3314,path:'/?sMethod=GetEntity&sBusiness=ZoneAgent'
       //  , post_data:{  sCond : "龙华", iType: 5}}

         //    {info:'测试 商家 GetData',port:3314,path:'/?sMethod=GetData&sBusiness=ZoneAgent'
       //  , post_data:{  iIndex : 0, iCount: 5, sAccount: "zybozhou"}}

        //      {info:'测试 商家 ModifyPwd',port:3314,path:'/?sMethod=ModifyPwd&sBusiness=ZoneAgent'
        // , post_data:{  sPrimaryKey: "133333333", sOldPwd: "5555", sNewPwd: "456"}}

       //      {info:'测试 商家 Login',port:3314,path:'/?sMethod=Login&sBusiness=ZoneAgent'
      //   , post_data:{  sAccount: "133333333",  sPassWord: "4567"}}

        //  {info:'测试 商家 ResetPwd',port:3314,path:'/?sMethod=ResetPwd&sBusiness=ZoneAgent'
       //  , post_data:{  sPrimaryKey: "133333333",  sNewPwd: "4567"}}

     //       {info:'测试 商家 IsOpen',port:3314,path:'/?sMethod=IsOpen&sBusiness=ShopInfo'
     //    , post_data:{  userId : 50037 }}



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

        //TymLib.Log("info","res:"+ chunk);
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
            //TymLib.Log("info",entity);
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
    //TymLib.Log("info","postdata:" + sAes);
    
    
    TymLib.Log("info","postdata:" + sAes);

    var sCurTime = TymLib.GetCurSecond();
    var sAppid = "425388c9c928c3d36856e67b6d86cc11";
    var sSid  = TymLib.md5("53b7a63d019de6ba32ed2e7fe38dfa72" + sAes + sCurTime + sAppid);

    option.path += "&sAppid=" + sAppid  + "&sSign=" + sSid + "&sTimeStamp=" + sCurTime;

    sAes = urlencode(sAes);
    sAes = "sCondition=" + sAes;

    TymLib.Log("info","path:" + option.path);

    TymLib.Log("info","time:" + TymLib.GetCurMilliSecond());

   //sAes = urlencode.encode(sAes);
    option.headers["Content-Length"] = sAes.length;
    var req = https.request(option,CallBackRes);
    req.on('error',function (err){
        TymLib.Log("info","Request error:" + err.code);
    });

    req.write(sAes );
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

       // var sDeAes = TymLib.DeAesCipher('tyQwg6xrA1Iwms5pdsCXQA==');
       // TymLib.Log("info","de_res:["+ sDeAes + "]");
for(var i = 0 ; i < 1 ; i++){
   Main();
}
TymLib.Log("info","end");




