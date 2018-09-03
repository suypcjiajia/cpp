#ifndef AGENTSYNC_H
#define AGENTSYNC_H

#include "com/Request.h"
#include "dal/OfflineOrderDAL.h"
#include "dal/PerAgentDAL.h"
class AgentSync
{
public:
    AgentSync();
    void Start();
    string OrderInfo(const Json::Value & j);

    static void* SyncPerAgent(void*);
    static string SyncPerAgent(const Json::Value & j);
private:
    OfflineOrderDAL mOfflineOrderDAL;

};

#endif // AGENTSYNC_H
