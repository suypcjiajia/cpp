#ifndef TDAL_H
#define TDAL_H

#include <vector>
#include <string>
#include "TMutex.hpp"
using namespace std;
class  TDAL
{
public:  static TDAL* GetInstance();
private: static TDAL* mInstance;
private: TMutex mMutex;//线程锁
public:
    //初始化
    int Init();
};

#endif // TDAL_H
