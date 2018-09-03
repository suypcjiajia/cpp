#ifndef SALLINGENTITY_H
#define SALLINGENTITY_H

#include <string>
#include <vector>
using namespace std;

struct SallingEntity
{
    SallingEntity(){        

    }

    string sOrderNo;
    long lAddTime;
    long lEndTime;
    int iState;
    int iType;
    long lIdSell;
    double dPrice;
    int iLots;
    string sName;

};

typedef vector<SallingEntity> SallingLst;


#endif // SALLINGENTITY_H
