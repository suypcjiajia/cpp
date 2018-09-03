#ifndef KVALUEENTITY_H
#define KVALUEENTITY_H

#include <string>
#include <vector>
using namespace std;

struct KValueEntity
{
    KValueEntity(){
         price = 0.0;
         open = 0.0;
         close = 0.0;
         hight = 0.0;
         low = 0.0;
         yclose = 0.0;
         turnOver = 0.0;
         turnRate = 0.0;

         Id = -1;
         iType = -1;


    }

   double price;
   double open;
   double close;
   double hight;
   double low;
   double yclose;
   double turnOver;
   double turnRate;

   long Id;
   int iType;
   string sDay;
   string sName;

};

typedef vector<KValueEntity> KValueLst;

enum enum_kvalue_type{
    enum_kvalue_type_platform = 1,//关注关系
    enum_kvalue_type_push =2 //推广关系
};


#endif // KVALUEENTITY_H
