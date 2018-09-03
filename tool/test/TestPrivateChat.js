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


         // {info:'测试 Send',port:23241,path:'/?sMethod=Send&sBusiness=PrivateChat'
        // , post_data:{ lUserId : 50037, lToUserId : 52599, iType : 1, sContent : "新华社德国汉堡7月7日电（记者霍小光　蒋国鹏　刘畅）国家主席习近平7日在汉堡主持金砖国家领导人非正式会晤，发表引导性讲话和总结讲话。南非总统祖马、巴西总统特梅尔、俄罗斯总统普京、印度总理莫迪出席。5国领导人围绕世界政治经济形势和二十国集团重点议题深入交换意见，就金砖国家加强团结协作、合力构建开放型世界经济、完善全球经济治理、促进可持续发展达成重要共识。　　习近平在讲话中指出，当前，世界经济出现一些积极迹象，金砖国家发展前景普遍向好，令人振奋；同时，也出现一些需要金砖国家密切关注、妥善应对的挑战。这次二十国集团领导人峰会正值世界政治经济形势面临深刻调整的重要时刻。金砖国家是二十国集团重要成员，要支持主席国德国办好汉堡峰会，发出二十国集团致力于加强伙伴关系、建设开放型世界经济、稳定和促进全球经济增长的积极信号。　　习近平强调，金砖国家要发扬开放包容、合作共赢的伙伴精神，加强团结合作，维护共同利益，谋求联动发展。　　第一，我们要毫不动摇构建开放型世界经济，维护多边贸易体制，推动经济全球化向开放、包容、普惠、平衡、共赢方向发展，让全体人民分享经济增长和经济全球化的成果。　　第二，我们要毫不动摇奉行多边主义，共同推动各方以政治和和平方式解决地区冲突和争端，努力实现各国和平共处、合作共赢，倡导国际社会充分利用联合国等多边机制，共商国际规则，共建伙伴关系，共同应对各种全球性挑战。　　第三，我们要毫不动摇加强全球经济治理，共同巩固二十国集团作为国际经济合作主要论坛的地位，推动主要经济体加强宏观经济政策协调，落实好二十国集团领导人杭州峰会及历届峰会共识，为新兴市场国家和发展中国家创造良好发展环境。"}},

          // {info:'测试 Send',port:23241,path:'/?sMethod=Send&sBusiness=PrivateChat'
          // , post_data:{ sOrderNo : '5003712345678273157', sPassWord : '123456' ,
          // lUserId : 91853, lToUserId : 50397, iCurrency: 3, iCoinType : 1,iType : 5, sContent : '0.18'}}

      //  {info:'测试 MyChat',port:23241,path:'/?sMethod=MyChat&sBusiness=PrivateChat'
      //   , post_data:{ lUserId : 50037,lAddTime : 0, iCount : 100}}


 //{info:'测试 UpLoadPayRecord',port:23241,path:'/?sMethod=UpLoadPayRecord&sBusiness=PrivateChat'
  //        , post_data:{ sPayPhone : "13411367357",lRevPhone : "13411367357", sPayTime : "2017-06-13 :08:00:00", dMoney : 125865.00, 
  //        dCoin: 89652.00,sPayType : "现金", sOrder : "856485956", sState :"已支付", sRevType: "个人(细龙)"}}

       //   {info:'测试 UnPackLkMoney',port:23241,path:'/?sMethod=UnPackLkMoney&sBusiness=PrivateChat'
        // , post_data:{ sKey : '5003712345678273157'}}



         {info:'测试 QueryLkByOrder',port:23241,path:'/?sMethod=QueryLkByOrder&sBusiness=PrivateChat'
        , post_data:{ sKey : '4712041515999656691704', iType : 1}}


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


