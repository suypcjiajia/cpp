//#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include "TMd5.h"
//#include "OssManage.h"
//#include "dal/MysqlConnect.h"
//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);

//    miIndex = 0;
//    QPixmapCache::setCacheLimit(1);

//    //初始化要操作的数据库
//    MysqlConnect::GetInstance()->Init("mechatdb");

//    //TMultiMysqlDAL::SetMysqlAddr("rm-wz92t4rr5j7cbg964o.mysql.rds.aliyuncs.com");
//    TMultiMysqlDAL::SetMysqlAddr("127.0.0.1");
//    TMultiMysqlDAL::SetMysqlUser("mechat");
//    TMultiMysqlDAL::SetMysqlPwd("Mechat1234");


//    connect(&mQtimer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
//    mQtimer.start(1000*10);


//}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}


//QPixmap MainWindow::getPixmap(QString pixmap_name, int count, int index)
//{
//    QPixmap final_pixmap;
//    if(!pixmap_name.isEmpty() && count > 0)
//    {
//        QPixmap pixmap(pixmap_name);
//        int pixmap_width = pixmap.width()/count;
//        int pixmap_height = pixmap.height();
//        final_pixmap = pixmap.copy(pixmap_width*index, 0, pixmap_width, pixmap_height);
//    }
//}

//void MainWindow::onTimerOut()
//{
//    LongLst lGroupIds;

//    mHeadDal.GetGroupId(lGroupIds, miIndex);

//    if( lGroupIds.size() > 0){
//        miIndex += 10;
//    }else{
//        miIndex = 0;
//    }

//    for( LongLst::iterator it = lGroupIds.begin(); it != lGroupIds.end(); it++){
//        long & lGroupId = *it;
//        LongLst lMemberIds;
//        mHeadDal.GetRealMemberById(lGroupId, lMemberIds );
//        if( lMemberIds.size() == 0){
//            continue;
//        }
//        ListMembers(lGroupId,lMemberIds);
//    }
//}


//void MainWindow::ListMembers(long lGroupId,const LongLst & lMemberIds)
//{

//    StringLst  sHeadNames;
//    int iSucessCount = 0;
//    for( LongLst::const_iterator it = lMemberIds.begin(); it != lMemberIds.end(); it++){

//        if(iSucessCount >= 9){//max 9 pic of user
//            break;
//        }

//        const long & lMemberId = *it;
//        string sHeadUrl;
//        mHeadDal.GetUserHeadUrl(lMemberId,sHeadUrl);

//        string sHeadName = GetHeadName(sHeadUrl);

//        if( sHeadName.empty())
//            continue;

//        string sOSsName = sHeadName;

//        int iPos  = sHeadName.find_last_of(".");
//        if( iPos == string::npos)
//            continue;

//        sHeadName = sHeadName.substr(0,iPos);



//        QString qsLocal = msUserDir.c_str();
//        qsLocal += sHeadName.c_str();
//        QFile qFile(qsLocal );
//        if( qFile.exists() ){
//            sHeadNames.push_back(sHeadName);
//            iSucessCount ++;
//        }else{
//            //download from oss
//            if( OssManage::GetInstance()->GetObjectToLocalFile("tymuserhead",sOSsName,msUserDir  + sHeadName)){
//                sHeadNames.push_back(sHeadName);
//                iSucessCount ++;
//            }
//        }
//    }
//    Jigsaw(lGroupId,sHeadNames);

//}

//string MainWindow::GetHeadName(const string & sHeadUrl)
//{
//    if( sHeadUrl.find_first_of("http://") == string::npos )
//        return "";
//    int pos = sHeadUrl.find_last_of("\\");
//    if( pos == string::npos){
//        pos = sHeadUrl.find_last_of("/");
//    }

//    if( pos!= string::npos){
//        return sHeadUrl.substr(pos + 1);
//    }else{
//        return "";
//    }
//}

//void MainWindow::Save(long lGroupId,QPixmap & pixMap, StringLst & sHeadNames)
//{
//    string s;
//    StringLst::iterator it = sHeadNames.begin();
//    for( ; it != sHeadNames.end() ; it++){
//        s += *it;
//    }
//    string sGroupHeadName = Md5ToLower(s);
//    sGroupHeadName += ".jpg";

//    QString qsLocal = msGroupDir.c_str();
//    qsLocal += sGroupHeadName.c_str();
//    QFile qfile(qsLocal);
//    if( !qfile.exists() ){
//        pixMap.save(qsLocal);
//    }

//    long lObjectLen = 0;
//    string sObjectType;
//    OssManage::GetInstance()->HeadObject("tymgroupchat",sGroupHeadName,sObjectType,lObjectLen);
//    if( lObjectLen <= 1000 ){
//        if(OssManage::GetInstance()->PutObjectFromFile("tymgroupchat", sGroupHeadName, msGroupDir + sGroupHeadName))
//            mHeadDal.UpGroupHead(lGroupId,"http://tymgroupchat." + msEndPoint  + "/" + sGroupHeadName);
//    }
//    ui->label->resize(480,480);
//    ui->label->setPixmap(pixMap);


//}


//QString MainWindow::String2QStr(const string & f)
//{
//    QString qs = f.c_str();
//    return qs;
//}

//QString MainWindow::String2QStr(const string & f, const string & s)
//{
//    QString qs = f.c_str();
//    qs += s.c_str();
//    return qs;

//}

////群成员头像拼图，拼成群头像
//void MainWindow::Jigsaw(long lGroupId,StringLst & sHeadNames)
//{

//    int size = sHeadNames.size();

//    if( size == 0)
//        return;

//    int iGroupHeadWidth = 480,iGroupHeadHeight = 480;
//    int iDistance = 20;


//    QPixmap pixGroupHead(QSize(iGroupHeadWidth,iGroupHeadHeight));
//    QPainter painer(&pixGroupHead);

//    painer.fillRect(0,0,iGroupHeadWidth,iGroupHeadHeight,QColor(128,128,128));


//    if( size == 1){

//        QPixmap pixUser;
//        int x1 = iGroupHeadWidth/4;
//        int y1 = iGroupHeadHeight/4;
//        int d = iGroupHeadWidth/2;
//        if( pixUser.load(String2QStr(msUserDir + sHeadNames[0]))){

//            painer.drawPixmap(QRect(x1,y1,d,d), pixUser);

//            Save(lGroupId,pixGroupHead,sHeadNames);

//        }

//    }else if( size == 2 ){

//        QPixmap pixUser;
//        int x1 = 0;
//        int y1 = iGroupHeadHeight/4;
//        int d = iGroupHeadWidth/2;
//        bool bLoad = true;
//        for( int i  =0 ; i < 2 ; i++){
//            bLoad = pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
//            if( bLoad){
//                painer.drawPixmap(QRect(x1 + d*i, y1 , d,d ), pixUser);
//            }else
//                break;
//        }
//        if( bLoad)
//            Save(lGroupId,pixGroupHead,sHeadNames);

//    }else if( size == 3){
//        QPixmap pixUser;
//        int x1 = iGroupHeadWidth/4;
//        int y1 = iGroupHeadHeight/4;
//        int d = iGroupHeadWidth/2;
//        if(pixUser.load(String2QStr(msUserDir + sHeadNames[0]))){
//            painer.drawPixmap(QRect(x1 , 0, d,d), pixUser);

//            if( pixUser.load(String2QStr(msUserDir + sHeadNames[1]))){
//                painer.drawPixmap(QRect(0, d, d,d), pixUser);
//                if( pixUser.load(String2QStr(msUserDir + sHeadNames[2]))){
//                    painer.drawPixmap(QRect(d, d, d,d), pixUser);

//                    Save(lGroupId,pixGroupHead,sHeadNames);
//                }
//            }
//        }

//    }else if( size == 4 || size == 5){
//        QPixmap pixUser;
//        int x1 = 0;
//        int y1 = 0;
//        int d = iGroupHeadWidth/2;
//        for( int i = 0; i < 4 ; i++){
//            pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
//            painer.drawPixmap(QRect(x1 + d* (i%2) , y1 + d * (i/2), d,d), pixUser);
//        }

//        Save(lGroupId,pixGroupHead,sHeadNames);

//    }else if ( size == 6 || size == 7 || size == 8){
//        QPixmap pixUser;
//        int x1 = iGroupHeadWidth/8;
//        int d = iGroupHeadHeight/4;
//        int y1 = x1 + 0.5*d;
//        for(int i = 0; i < 6 ; i++){
//            pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
//            painer.drawPixmap(QRect(x1 + d*(i%3) ,y1 + d*(i/3), d,d), pixUser);
//        }

//        Save(lGroupId,pixGroupHead,sHeadNames);

//    }else if( size >= 9){

//        QPixmap pixUser;
//        int x1 = 0;
//        int y1 = 0;
//        int d = iGroupHeadWidth/3;

//        for( int i  = 0 ; i < 9 ;  i ++){
//            pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
//            painer.drawPixmap(QRect(x1 + d*(i%3) ,y1 + d*(i/3), d,d), pixUser);

//        }

//        Save(lGroupId,pixGroupHead,sHeadNames);

//    }

//}






#include "TConvert.h"
#include "log/Logger.h"
#include "TypeDef.h"
#include "com/PostsAes.h"
#include "mainwindow.h"
#include "TMd5.h"
#include "OssManage.h"
#include "dal/MysqlConnect.h"
MainWindow::MainWindow()
{

    miIndex = 0;
    QPixmapCache::setCacheLimit(1);

    //初始化要操作的数据库
    //MysqlConnect::GetInstance()->Init("mechatdb");

    //TMultiMysqlDAL::SetMysqlAddr("rm-wz92t4rr5j7cbg964o.mysql.rds.aliyuncs.com");
    //TMultiMysqlDAL::SetMysqlAddr("127.0.0.1");
    //TMultiMysqlDAL::SetMysqlUser("mechat");
    //TMultiMysqlDAL::SetMysqlPwd("Mechat1234");



}

MainWindow::~MainWindow()
{
}


QPixmap MainWindow::getPixmap(QString pixmap_name, int count, int index)
{
    QPixmap final_pixmap;
    if(!pixmap_name.isEmpty() && count > 0)
    {
        QPixmap pixmap(pixmap_name);
        int pixmap_width = pixmap.width()/count;
        int pixmap_height = pixmap.height();
        final_pixmap = pixmap.copy(pixmap_width*index, 0, pixmap_width, pixmap_height);
    }
}

void MainWindow::onTimerOut()
{
    LongLst lGroupIds;int iCount = 10;

    //mHeadDal.GetGroupId(lGroupIds, miIndex);
    GetGroupIds(miIndex, iCount, lGroupIds);

    if( lGroupIds.size() > 0){
        miIndex += iCount;
    }else{
        miIndex = 0;
    }

    for( LongLst::iterator it = lGroupIds.begin(); it != lGroupIds.end(); it++){
        long & lGroupId = *it;
        //LongLst lMemberIds;
        //mHeadDal.GetRealMemberById(lGroupId, lMemberIds );
        MemberInfos lMemberIds;
        GetGroupMembers(lGroupId,lMemberIds);
        if( lMemberIds.size() == 0){
            continue;
        }
        ListMembers(lGroupId,lMemberIds);
    }
}


void MainWindow::ListMembers(long lGroupId,const MemberInfos & lMemberIds)
{

    StringLst  sHeadNames;
    int iSucessCount = 0;
    for( MemberInfos::const_iterator it = lMemberIds.begin(); it != lMemberIds.end(); it++){

        if(iSucessCount >= 9){//max 9 pic of user
            break;
        }

        const MemberInfo & member = *it;
        const long & lMemberId = member.lMemberId;

        string sHeadUrl = member.sHeadImage;
       // mHeadDal.GetUserHeadUrl(lMemberId,sHeadUrl);

        string sHeadName = GetHeadName(sHeadUrl);

        if( sHeadName.empty())
            continue;

        string sOSsName = sHeadName;

        int iPos  = sHeadName.find_last_of(".");
        if( iPos == string::npos)
            continue;

        sHeadName = sHeadName.substr(0,iPos);



        QString qsLocal = msUserDir.c_str();
        qsLocal += sHeadName.c_str();
        QFile qFile(qsLocal );
        if( qFile.exists() ){
            sHeadNames.push_back(sHeadName);
            iSucessCount ++;
        }else{
            //download from oss
            if( OssManage::GetInstance()->GetObjectToLocalFile("tymuserhead",sOSsName,msUserDir  + sHeadName)){
                sHeadNames.push_back(sHeadName);
                iSucessCount ++;
            }
        }
    }
    Jigsaw(lGroupId,sHeadNames);

}

string MainWindow::GetHeadName(const string & sHeadUrl)
{
    if( sHeadUrl.find_first_of("http://") == string::npos )
        return "";
    int pos = sHeadUrl.find_last_of("\\");
    if( pos == string::npos){
        pos = sHeadUrl.find_last_of("/");
    }

    if( pos!= string::npos){
        return sHeadUrl.substr(pos + 1);
    }else{
        return "";
    }
}

void MainWindow::Save(long lGroupId,QPixmap & pixMap, StringLst & sHeadNames)
{
    string s;
    StringLst::iterator it = sHeadNames.begin();
    for( ; it != sHeadNames.end() ; it++){
        s += *it;
    }
    string sGroupHeadName = Md5ToLower(s);
    sGroupHeadName += ".jpg";

    QString qsLocal = msGroupDir.c_str();
    qsLocal += sGroupHeadName.c_str();
    QFile qfile(qsLocal);
    if( !qfile.exists() ){
        pixMap.save(qsLocal);
    }

    long lObjectLen = 0;
    string sObjectType;
    OssManage::GetInstance()->HeadObject("tymgroupchat",sGroupHeadName,sObjectType,lObjectLen);
    if( lObjectLen <= 1000 ){
        if(OssManage::GetInstance()->PutObjectFromFile("tymgroupchat", sGroupHeadName, msGroupDir + sGroupHeadName)){
            //mHeadDal.UpGroupHead(lGroupId,"http://tymgroupchat." + msEndPoint  + "/" + sGroupHeadName);
            UpGroupHead(lGroupId, "http://tymgroupchat." + msEndPoint  + "/" + sGroupHeadName);
        }
    }else{
        //mHeadDal.UpGroupHead(lGroupId,"http://tymgroupchat." + msEndPoint  + "/" + sGroupHeadName);
        UpGroupHead(lGroupId, "http://tymgroupchat." + msEndPoint  + "/" + sGroupHeadName);
    }

}


QString MainWindow::String2QStr(const string & f)
{
    QString qs = f.c_str();
    return qs;
}

QString MainWindow::String2QStr(const string & f, const string & s)
{
    QString qs = f.c_str();
    qs += s.c_str();
    return qs;

}

//群成员头像拼图，拼成群头像
void MainWindow::Jigsaw(long lGroupId,StringLst & sHeadNames)
{

    int size = sHeadNames.size();

    if( size == 0)
        return;

    int iGroupHeadWidth = 480,iGroupHeadHeight = 480;
    int iDistance = 20;


    QPixmap pixGroupHead(QSize(iGroupHeadWidth,iGroupHeadHeight));
    QPainter painer(&pixGroupHead);

    painer.fillRect(0,0,iGroupHeadWidth,iGroupHeadHeight,QColor(128,128,128));


    if( size == 1){

        QPixmap pixUser;
        int x1 = iGroupHeadWidth/4;
        int y1 = iGroupHeadHeight/4;
        int d = iGroupHeadWidth/2;
        if( pixUser.load(String2QStr(msUserDir + sHeadNames[0]))){

            painer.drawPixmap(QRect(x1,y1,d,d), pixUser);

            Save(lGroupId,pixGroupHead,sHeadNames);

        }

    }else if( size == 2 ){

        QPixmap pixUser;
        int x1 = 0;
        int y1 = iGroupHeadHeight/4;
        int d = iGroupHeadWidth/2;
        bool bLoad = true;
        for( int i  =0 ; i < 2 ; i++){
            bLoad = pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
            if( bLoad){
                painer.drawPixmap(QRect(x1 + d*i, y1 , d,d ), pixUser);
            }else
                break;
        }
        if( bLoad)
            Save(lGroupId,pixGroupHead,sHeadNames);

    }else if( size == 3){
        QPixmap pixUser;
        int x1 = iGroupHeadWidth/4;
        int y1 = iGroupHeadHeight/4;
        int d = iGroupHeadWidth/2;
        if(pixUser.load(String2QStr(msUserDir + sHeadNames[0]))){
            painer.drawPixmap(QRect(x1 , 0, d,d), pixUser);

            if( pixUser.load(String2QStr(msUserDir + sHeadNames[1]))){
                painer.drawPixmap(QRect(0, d, d,d), pixUser);
                if( pixUser.load(String2QStr(msUserDir + sHeadNames[2]))){
                    painer.drawPixmap(QRect(d, d, d,d), pixUser);

                    Save(lGroupId,pixGroupHead,sHeadNames);
                }
            }
        }

    }else if( size == 4 || size == 5){
        QPixmap pixUser;
        int x1 = 0;
        int y1 = 0;
        int d = iGroupHeadWidth/2;
        for( int i = 0; i < 4 ; i++){
            pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
            painer.drawPixmap(QRect(x1 + d* (i%2) , y1 + d * (i/2), d,d), pixUser);
        }

        Save(lGroupId,pixGroupHead,sHeadNames);

    }else if ( size == 6 || size == 7 || size == 8){
        QPixmap pixUser;
        int x1 = iGroupHeadWidth/8;
        int d = iGroupHeadHeight/4;
        int y1 = x1 + 0.5*d;
        for(int i = 0; i < 6 ; i++){
            pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
            painer.drawPixmap(QRect(x1 + d*(i%3) ,y1 + d*(i/3), d,d), pixUser);
        }

        Save(lGroupId,pixGroupHead,sHeadNames);

    }else if( size >= 9){

        QPixmap pixUser;
        int x1 = 0;
        int y1 = 0;
        int d = iGroupHeadWidth/3;

        for( int i  = 0 ; i < 9 ;  i ++){
            pixUser.load(String2QStr(msUserDir + sHeadNames[i]));
            painer.drawPixmap(QRect(x1 + d*(i%3) ,y1 + d*(i/3), d,d), pixUser);

        }

        Save(lGroupId,pixGroupHead,sHeadNames);

    }

}


int MainWindow::GetGroupIds(int iIndex, int iCount, longs & lGroupIds)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["iIndex"]=iIndex;
    jsonValue["iCount"]=iCount;

    //URL
    string sUrl = "http://" + msIMIp + ":23241/?sBusiness=GroupData&sMethod=GetGroupIds";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return -1;
    }
    int iRet = tConvert.Json2Int(jsonReturn["iRet"]);
    for( int i = 0; i < jsonReturn["lGroupIds"].size(); i++){
       lGroupIds.push_back(  tConvert.Json2Long(jsonReturn["lGroupIds"][i]));
    }
    return iRet;
}


int MainWindow::GetGroupMembers(long lGroupId , MemberInfos & members)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["lGroupId"]= tConvert.LongToStr( lGroupId );

    //URL
    string sUrl = "http://" + msIMIp + ":23241/?sBusiness=GroupMember&sMethod=MemberById";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return -1;
    }
    int iRet = tConvert.Json2Int(jsonReturn["iRet"]);
    for( int i = 0; i < jsonReturn["list"].size(); i++){
        MemberInfo member;
        member.lMemberId = tConvert.Json2Long(jsonReturn["list"][i]["lMemberId"]);
        member.sHeadImage = tConvert.Json2String(jsonReturn["list"][i]["sHeadImage"]);
       members.push_back( member );
    }
    return iRet;
}


int MainWindow::UpGroupHead(long lGroupId ,string sHeadUrl)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["lGroupId"]= tConvert.LongToStr( lGroupId );
    jsonValue["sGroupHead"]= sHeadUrl;

    //URL
    string sUrl = "http://" + msIMIp + ":23241/?sBusiness=GroupData&sMethod=SetHead";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return -1;
    }
    int iRet = tConvert.Json2Int(jsonReturn["iRet"]);

    return iRet;
}
