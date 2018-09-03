#ifndef ORDERNUMDAL_H
#define ORDERNUMDAL_H
/*
 *红包订单号数据
 */
#include <string>
#include <vector>
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class OrderNumDAL
{

public:
    //创建表
    int CreateTable();

    int AddData( const string & sOrderNum, const string & sTransferID);

    int GetTransferID(const string & sOrderNum, string & sTransferID);
private:
    const string msTableName = "ordernum";

};

#endif // ORDERNUMDAL_H
