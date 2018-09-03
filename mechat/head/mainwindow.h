#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
//#include <QImage>
//#include <QPainter>
//#include <QTimer>
//#include "HeadDal.h"
//#include <QFile>
//#include <QPixmapCache>

//typedef vector<string> StringLst;

//namespace Ui {
//class MainWindow;
//}

//class MainWindow : public QMainWindow
//{
//    Q_OBJECT

//public:
//    explicit MainWindow(QWidget *parent = 0);
//    ~MainWindow();



//    QPixmap getPixmap(QString pixmap_name, int count, int index);

//private:
//    void ListMembers(long lGroupId,const LongLst & lMemberIds);
//    string GetHeadName(const string & sHeadUrl);

//    //群成员头像拼图，拼成群头像
//    void Jigsaw(long lGroupId,StringLst & sHeadNames);

//    void Save(long lGroupId,QPixmap & pixMap, StringLst & sHeadNames);

//    QString String2QStr(const string & f);
//    QString String2QStr(const string & f, const string & s);

//public slots:
//  void onTimerOut();


//private:
//    Ui::MainWindow *ui;

//    QImage mQimage;
//    QTimer mQtimer;


//    HeadDAL mHeadDal;

//    const string msUserDir = "/Head/User/";
//    const string msGroupDir = "/Head/Group/";
//    const string  msEndPoint =  "oss-cn-shenzhen.aliyuncs.com";
//    int miIndex ;

//};



#include "TypeDef.h"
#include <QImage>
#include <QPainter>
#include <QTimer>
#include "HeadDal.h"
#include <QFile>
#include <QPixmapCache>


struct MemberInfo{
    long lMemberId;
    string sHeadImage;
};
typedef vector<MemberInfo> MemberInfos;

class MainWindow
{


public:
     MainWindow();
    ~MainWindow();

    void onTimerOut();



    QPixmap getPixmap(QString pixmap_name, int count, int index);

private:
    void ListMembers(long lGroupId,const MemberInfos & lMemberIds);
    string GetHeadName(const string & sHeadUrl);

    //群成员头像拼图，拼成群头像
    void Jigsaw(long lGroupId,StringLst & sHeadNames);

    void Save(long lGroupId,QPixmap & pixMap, StringLst & sHeadNames);

    QString String2QStr(const string & f);
    QString String2QStr(const string & f, const string & s);

    int GetGroupIds(int iIndex, int iCount, longs & lGroupIds);

    int GetGroupMembers(long lGroupId , MemberInfos & members);

    int UpGroupHead(long lGroupId ,string sHeadUrl);




private:

    QImage mQimage;


    HeadDAL mHeadDal;

    const string msUserDir = "/Head/User/";
    const string msGroupDir = "/Head/Group/";
    const string  msEndPoint =  "oss-cn-shenzhen.aliyuncs.com";
    const string msIMIp = "120.25.129.101";
    //const string msIMIp = "192.168.168.158";
    int miIndex ;

};

#endif // MAINWINDOW_H
