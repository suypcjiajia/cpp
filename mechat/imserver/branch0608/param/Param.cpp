#include "Param.h"
#include "bll/TBLL.h"
Param::Param()
{

}



//获取配置：每手的粉丝数
int Param::GetFansPerLots()
{
    string sFans = TBLL::GetInstance()->GetFansPerLots();
    return atoi(sFans.c_str());
}
