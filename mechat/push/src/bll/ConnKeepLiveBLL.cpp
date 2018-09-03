#include "ConnKeepLiveBLL.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TConvert.h"


/*
 * websocket连接心跳探测
策列：对于超时的连接，关闭
 */


using namespace std;
ConnKeepLiveBLL* ConnKeepLiveBLL::mInstance = NULL;
ConnKeepLiveBLL* ConnKeepLiveBLL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new ConnKeepLiveBLL;
    }
    return mInstance;
}
//刷新链接
int ConnKeepLiveBLL::Replace(const connection_hdl & hdl, server * sevice)
{
    ScopeLock lock( this->mMutex );
    TConvert tConvert;
    ConnKeepLiveEntity entity;
    entity.service = sevice;
    entity.lLastTime = tConvert.GetCurrentTime();//记录下活跃时间
    entity.hdl = hdl;


    mConnKeepLiveHash[hdl.lock().get()] = entity;

    return 0;
}
//删除链接
int ConnKeepLiveBLL::del( const connection_hdl & hdl )
{
    ScopeLock lock( this->mMutex );
    ConnKeepLiveHashMap::iterator iter= this->mConnKeepLiveHash.find( hdl.lock().get() );
    if( iter != mConnKeepLiveHash.end() ) {
        mConnKeepLiveHash.erase(iter);
    }
    return 0;
}

void ConnKeepLiveBLL::Start()
{
    pthread_t tid1;
    pthread_create( &tid1, NULL, ConnKeepLiveBLL::PingThread, this );
}

void* ConnKeepLiveBLL::PingThread(void*ptr)
{
    pthread_detach(pthread_self());

    ConnKeepLiveBLL * myself =( ConnKeepLiveBLL *) ptr;
    myself->Ping();

    pthread_exit(NULL);
}

void ConnKeepLiveBLL::Ping()
{
    TConvert tConvert;
    while(true)
    {
        ConnKeepLiveEntity entity;
        if( GetOneHdl(entity)){
            long lTime = tConvert.GetCurrentTime() - entity.lLastTime;
            if( lTime > 60*10*1000000){
                try{
                    del(entity.hdl);
                    entity.service->close(entity.hdl,0,"");
                }catch(...){

                }
            }else if( lTime < 60*5*1000000){//小于5分钟，不发ping
            }
             else{
                websocketpp::lib::error_code err;
                //appendlog(TTDLogger::LOG_DEBUG,"ConnKeepLiveBLL::Ping hdl:%ld",entity.hdl.lock().get());
                entity.service->ping(entity.hdl,"123",err);
            }
            usleep(1000);

        }else{
            sleep(1);
        }





    }

}

bool ConnKeepLiveBLL::GetOneHdl( ConnKeepLiveEntity & entity)
{
    ScopeLock lock( this->mMutex );

    static long lIndex = 0;

    if(mConnKeepLiveHash.size() ==0 )
        return false;


    if( lIndex >= mConnKeepLiveHash.size()){
        lIndex = 0;
    }

    ConnKeepLiveHashMap::iterator iter = mConnKeepLiveHash.begin();
    for(int i = 0; i < lIndex ; i++){
        iter ++;
    }

    entity = iter->second;
    lIndex++;

    return true;

}

