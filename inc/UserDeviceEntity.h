#ifndef USERDEVICEENTITY_H
#define USERDEVICEENTITY_H

#include <string>
#include <vector>
using namespace std;

struct UserDeviceEntity
{
    long userId;
    string deviceId;
};

typedef vector<UserDeviceEntity> UserDeviceLst;



#endif
