var https=require('http');
var fs=require('fs');
var TymLib = require("TymLib.js");
var query = require("querystring");    //解析POST请求
var urlencode = require('urlencode');
//https请求参数
var option = {
   //hostname: '120.25.129.101',
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

         // {info:'测试 Order',port:2314,path:'/?sMethod=Order&sBusiness=Coupon'
         // , post_data:{"userId":"50037","idProduct":'76096701505291294',"idPlatform":100004 ,num: 1, orderNo :'12345689571122179',
         //              payType:0,password:"e10adc3949ba59abbe56e057f20f883e"}}

         // {info:'测试 GetMyCoupon',port:2314,path:'/?sMethod=GetMyCoupon&sBusiness=Coupon'
         // , post_data:{"userId":"50037","page":1,"count":10 , state :'1,2'}}

       //   {info:'测试 Loop',port:2314,path:'/?sMethod=Loop&sBusiness=Coupon'
        //  , post_data:{"town":"深圳","area":"龙华新区"}}

        //  {info:'测试 TypeInfo',port:2314,path:'/?sMethod=TypeInfo&sBusiness=Coupon'
        //  , post_data:{"town":"深圳","area":"龙华新区"}}

        //  {info:'测试 GetPageByType',port:2314,path:'/?sMethod=GetPageByType&sBusiness=Coupon'
        //  , post_data:{"town":"深圳","area":"龙华新区", "gpsLat" : 22.12356 , "gpsLon" : 122.25689, "page" : 1, "sort" :2, "type":"健康养生"}}


         // {info:'测试 Detail',port:2314,path:'/?sMethod=Detail&sBusiness=Coupon'
         // , post_data:{"userId":"161323",orderNo : "918531506567447093"}}

          // {info:'测试 ',port:2314,path:'/zoneAgent/isOpen?zone=baoanqu'
          // , post_data:{}, type:1}


         // {info:'测试 ProductTimeOut',port:2314,path:'/?sMethod=ProductTimeOut&sBusiness=Coupon'
         // , post_data:{"idProduct": 4}}


        // {info:'测试 IsTimeOut',port:2314,path:'/?sMethod=IsTimeOut&sBusiness=Coupon'
         // , post_data:{orderNo : "1234568957112"}} 

       //   {info:'测试 ReqBack',port:2314,path:'/?sMethod=ReqBack&sBusiness=Coupon'
       //   , post_data:{orderNo : "1234568", code : ["1111"], userId: 50037 ,reason : "i donot like it", password:"e10adc3949ba59abbe56e057f20f883exxxxxx"}}

           // {info:'测试 WriteCommtent',port:2314,path:'/?sMethod=WriteCommtent&sBusiness=Coupon'
           // , post_data:{ idPlatform : '100004', score : 5.0, userId : 50037 , headImage : 'http://member.oss-cn-shenzhen.aliyuncs.com/%E8%81%94%E7%9B%9F%E5%95%86%E5%AE%B6%E5%B1%95%E7%A4%BA%E4%BA%A7%E5%93%81%E5%9B%BE1.png' , nickName: 'suyp',
           //  content: 'good shop ,very good', pics : ["http://member.oss-cn-shenzhen.aliyuncs.com/%E8%81%94%E7%9B%9F%E5%95%86%E5%AE%B6%E5%B1%95%E7%A4%BA%E4%BA%A7%E5%93%81%E5%9B%BE1.png"] ,idProduct: 4}}


         //   {info:'测试 ProductDetail',port:2314,path:'/?sMethod=ProductDetail&sBusiness=Coupon'
        //   , post_data:{id: 9223372036854775807}}


        //  {info:'测试 Agree',port:2314,path:'/?sMethod=Agree&sBusiness=Coupon'
        //  , post_data:{orderNo: "456987"}}


          //{info:'测试 RejBack',port:2314,path:'/?sMethod=RejBack&sBusiness=Coupon'
          //, post_data:{orderNo: "456987" , rejReason : "no reason"}}


         //  {info:'测试 Check',port:2314,path:'/?sMethod=Check&sBusiness=Coupon'
         // , post_data:{idPlatform: 7610641, code: "20939267708663"}}


           {info:'测试 GetCouponByPlatfrom',port:2314,path:'/?sMethod=GetCouponByPlatfrom&sBusiness=Coupon'
           , post_data:{"idPlatform":"7609667","page":1,"count":5 , state :'1'}}

          //{info:'测试 CancelBack',port:2314,path:'/?sMethod=CancelBack&sBusiness=Coupon'
          //, post_data:{"userId":"50037", "orderNo" : "1234568957" }}


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
        //var sDeAes = chunk;
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
    var type = units[index].type;
    TymLib.Log("info","\n"   + units[index].info + "\npath:" + option.path  );

    if( type != 1)
    {
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
    }else{
    var  postdata = JSON.stringify(units[index].post_data);

    postdata = urlencode(postdata);

    option.headers["Content-Length"] = postdata.length;
    var req = https.request(option,CallBackRes);
    req.on('error',function (err){
        TymLib.Log("info","Request error:" + err.code);
    });

    TymLib.Log("info","write::" + postdata);
    
    req.write(postdata + "\n");
    req.end();
    }
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


