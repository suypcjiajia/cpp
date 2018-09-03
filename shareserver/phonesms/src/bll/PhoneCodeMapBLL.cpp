#include "PhoneCodeMapBLL.h"

#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"

using namespace std;
PhoneCodeMapBLL* PhoneCodeMapBLL::mInstance = NULL;
PhoneCodeMapBLL* PhoneCodeMapBLL::GetInstance()
{
        if (mInstance == NULL){
                mInstance = new PhoneCodeMapBLL;
        }
        return mInstance;
}
PhoneCodeMapBLL::PhoneCodeMapBLL()
{
    this->mPhoneCodes.clear();
    pthread_t tid;
    pthread_create(&tid,NULL,ThreadFun,this);

}

void * PhoneCodeMapBLL::ThreadFun(void *p)
{
    PhoneCodeMapBLL * myself =  (PhoneCodeMapBLL*) p;
    myself->RealRun();
}

//线程运行逻辑
void PhoneCodeMapBLL::RealRun()
{

    while(true){
        sleep(60*10);
        ClearTimeOut();

    }
}

void PhoneCodeMapBLL::ClearTimeOut()
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();

    appendlog(TTDLogger::LOG_DEBUG,"PhoneCodeMapBLL::RealRun  mPhoneCodes's size=%d",this->mPhoneCodes.size());

    ScopeLock lock( this->mMutex );
    MapPhoneCodeState::iterator iter = this->mPhoneCodes.begin();
    for ( ; iter != this->mPhoneCodes.end(); ) {

        PhoneCodeState state = iter->second;
        //10分钟 = 10*60*1000*1000微秒
        if (lCurrentTime > state.lAddTime +10*60*1000*1000 ) {
            this->mPhoneCodes.erase(iter++);
        }
        else {
            ++iter;
        }
    }
}

//设置
int PhoneCodeMapBLL::SetPhoneCodeList(const string& psPhone, const string& psCode)
{
    ScopeLock lock( this->mMutex );

    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();


    PhoneCodeState CodeState;
    CodeState.iState = 0;
    CodeState.lAddTime = lCurrentTime;
    this->mPhoneCodes[psPhone + psCode] = CodeState;

    return 0;
}
//是否存在
bool PhoneCodeMapBLL::IsdExist(const string& psPhone, const string& psCode)
{
    ScopeLock lock( this->mMutex );



    MapPhoneCodeState::iterator iter= this->mPhoneCodes.find(psPhone + psCode);
    if( iter != this->mPhoneCodes.end() ) {
        return true;
    }

    return false;
}
//Set beused
int PhoneCodeMapBLL::SetUsed( const string& psPhone, const string& psCode )
{
    ScopeLock lock( this->mMutex );


    MapPhoneCodeState::iterator iter= this->mPhoneCodes.find(psPhone + psCode);
    if( iter != this->mPhoneCodes.end() ) {
        PhoneCodeState & CodeState = iter->second;
        if( CodeState.iState != 0){
            return -2;
        }
        CodeState.iState = 1;
        return 0;
    }

    return -1;
}

