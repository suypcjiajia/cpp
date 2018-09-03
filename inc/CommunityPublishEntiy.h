#ifndef CommunityPublishEntiy_H
#define CommunityPublishEntiy_H

#include <string>
#include <vector>
using namespace std;

struct CommunityPublishEntiy
{
    string id ;// '唯一标记（unix秒+6位随机数)',
    long userId;// '发布者Id',
    string nickName ;// '发布者昵称',
    string headImage ;// '发布者头像',
    string addTime ;// '发布时间',
    int fineCount ;// '点赞次数',
    int markCount ;// '评论次数',
    int collectCount ;// '收藏次数',
    string content ;// '内容',
    string pic ;// '图片（数组）',
    int authority ;// '权限',0-所有人可见 1-仅自己可见 2-仅好友可见
};
typedef vector<CommunityPublishEntiy> CommunityPublishList;

#endif // CommunityPublishEntiy_H
