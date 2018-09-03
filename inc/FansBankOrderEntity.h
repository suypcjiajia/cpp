#ifndef FANSBANKSENTITY_H
#define FANSBANKSENTITY_H

#include <string>
#include <vector>
using namespace std;

struct FansBankOrderEntity
{
    FansBankOrderEntity(){
        comeFrom = 0;
    }

    string sOrderNo;
    long lAddTime;
    long lEndTime;
    int iState;
    int iType;
    long lIdSell;
    string sNameSell;
    long lIdBuy;
    string sNameBuy;
    double dPrice;
    int iLots;
    double dTurnOver;
    int iLotsOver;
    int comeFrom;

};

typedef vector<FansBankOrderEntity> FansBankOrderLst;

enum enum_order_state{
    enum_order_salling = 0,
    enum_order_close = 1,
    enum_order_ok = 2,
    enum_order_my = 3
};



#endif // FANSBANKSENTITY_H
