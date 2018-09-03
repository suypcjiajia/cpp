#ifndef CommunityRemarkEntiy_H
#define CommunityRemarkEntiy_H

#include <string>
#include <vector>
using namespace std;

//类型 1-点赞 2-收藏 3-分享 4-评论 5-回复
enum enum_commuityremark_type
{
    commuityremark_type_fine = 1,
    commuityremark_type_collect = 2,
    commuityremark_type_share = 3,
    commuityremark_type_comment = 4,
    commuityremark_type_reply = 5
};

struct CommunityRemarkEntiy
{
    string id ;// '唯一标记（unix秒+6位随机数)',
    string publishId ;// '圈子信息Id',
    long userId ;// '互动者Id',
    string nickName ;// '互动者昵称',
    string headImage ;// '互动者头像',
    string addTime ;// '评论时间',
    string comment ;// '评论内容',
    int type ;//'类型', 1-点赞 2-收藏 3-分享 4-评论 5-回复
    long touserId ;// '回复对像的Id',
    string tonickName ;// '回复对像的昵称',
    string toheadImage ;// '回复对像的头像',
};
typedef vector<CommunityRemarkEntiy> CommunityRemarkList;

#endif // CommunityRemarkEntiy_H
