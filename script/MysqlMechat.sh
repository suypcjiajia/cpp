#!/bin/bash　　

HOSTNAME="127.0.0.1"                                           #数据库信息
PORT="3306"
USERNAME="mechat"
PASSWORD="Mechat1234"
DBNAME="mechatdb"                                                       #数据库名称


#也可以写 HOSTNAME="localhost"，端口号 PORT可以不设定

echo "start..."
echo "HOSTNAME:$HOSTNAME PORT:$PORT USERNAME:$USERNAME PASSWORD:$PASSWORD DBNAME:$DBNAME"

 #创建数据库
sql="create database IF NOT EXISTS ${DBNAME}"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD} -e "${sql}"

#创建用户注册表
sql="create table IF NOT EXISTS user_datadal 
(
  sPrimaryKey VARCHAR(50) NOT NULL  comment '用户Id',
  lUserId bigint NOT NULL  comment '用户Id，同sPrimaryKey',
  lAddTime bigint NOT NULL comment '注册时间',
  lUpTime bigint NOT NULL comment '更新时间',
  iState int NOT NULL comment '状态 0-正常  1-封死  2-pass 3-禁止交易 333-逻辑删除',
  sAccount VARCHAR(12) NOT NULL  comment '注册手机号码',
  sPassword VARCHAR(64) NOT NULL  comment '密码',
  iSex int NOT NULL comment '姓别0-女 1-男 2-未知',
  sProvince VARCHAR(50) NOT NULL  comment '省份',
  sCity VARCHAR(50) NOT NULL  comment '城市',
  sSignature VARCHAR(50) NOT NULL  comment '个性签名',
  sEmail VARCHAR(50) NOT NULL  comment '邮箱地址',
  sHeadImage VARCHAR(200) NOT NULL  comment '头像',
  lPushManId bigint  NOT NULL  comment '推广人',
  lPlatformId bigint  NOT NULL  comment '第一次关注平台号',
  lCurrentPlatformId bigint  NOT NULL  comment '当前关注平台号',
  sBirthDay datatime NOT NULL  comment '生日',
  sQqNum VARCHAR(50) NOT NULL  comment '绑定QQ',
  sWeixinNum VARCHAR(50) NOT NULL  comment '绑定微信',
  sAliNum VARCHAR(50) NOT NULL  comment '绑定支付宝',
  iHot int NOT NULL default 0 comment '0-未审核 1-激活 2-初审完成 3-锁定待处理 4-警告待处理',
  UNIQUE KEY (lUserId),
  PRIMARY KEY(sPrimaryKey)
) comment='用户投票记录' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建第三方资料信息表
sql="create table IF NOT EXISTS third_user
(
  addTime datetime NOT NULL comment '添加时间',
  sex int NOT NULL comment '性别 0-女 1-男',
  headImage VARCHAR(200) NOT NULL  comment '头像',
  pushManId bigint  NOT NULL  comment '推广人',
  platformId bigint  NOT NULL  comment '关注平台号',
  nickName VARCHAR(64) NOT NULL  comment '昵称',
  id VARCHAR(128) NOT NULL  comment '微信或支付宝id',
  type int NOT NULL comment 'id类型 1-微信 2-支付宝',
  PRIMARY KEY(id)
) comment='第三方资料信息表' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"

#创建用户价值表
sql="create table IF NOT EXISTS fansbank_u_value 
(
userId BIGINT NOT NULL comment '用户Id', 
value Decimal(10,2) default 0 comment '价值',
type INT NOT NULL comment '类型 1 -平台  2 -个代',
upTime BIGINT NOT NULL comment '用户价值最后一次提高时的时间，unix微秒',
PRIMARY KEY(userId, type)
) ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"

#创建日K线表
sql="create table IF NOT EXISTS fansbank_k_value 
(
id BIGINT NOT NULL comment '天天兑号或平台号',
name VARCHAR(64) NOT NULL comment '昵称或店名',
type INT NOT NULL comment '类型 1 -平台  2 -个代', 
day VARCHAR(12) NOT NULL comment '日期 格式：2017-01-01', 
price Decimal(10,2) Not Null comment '实时现金均价',
open Decimal(10,2) Not Null comment '现金开盘价',
close Decimal(10,2) Not Null comment '现金收盘价', 
hight Decimal(10,2) Not Null comment '现金最高价',
low Decimal(10,2) Not Null comment '现金最低价', 
yclose Decimal(10,2) Not Null comment '昨收',
turnOver Decimal(10,2) Not Null comment '实时交易额',
turnRate Decimal(10,2) Not Null comment '实时换手率',
PRIMARY KEY(id,type,day)
) ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建订单表
sql="create table IF NOT EXISTS fansbank_order 
(
orderNo VARCHAR(64) NOT NULL comment '订单号',
addTime BIGINT NOT NULL comment '创建时间，unix微秒',
endTime BIGINT NOT NULL comment '结束时间，unix微秒',
state INT NOT NULL comment '订单状态 0 -正在卖出中 1 -已取消  2 -已完成交易',
type INT NOT NULL comment '类型 1 -平台  2 -个代',
idBuy BIGINT NOT NULL comment '买入者 天天兑号或平台号',
nameBuy VARCHAR(64) NOT NULL comment '买入者 昵称或店名', 
idSell BIGINT NOT NULL comment '卖出者 天天兑号或平台号',
nameSell VARCHAR(64) NOT NULL comment '卖出者 昵称或店名',
price Decimal(10,2) NOT NULL comment '现金均价(对已完成交易有效)', 
lots INT not NULL comment '手数', 
comeFrom INT not NULL comment '订单来源 0-BC端交易 1-报单大赛交易', 
lotsOver INT not NULL default 0 comment '已经卖出的手数(只对正在卖出的订单已取消的订单有效)',
turnOver  Decimal(10,2) NOT NULL comment '已经交易的现金额(只对正在卖出的订单已取消的订单有效)',
PRIMARY KEY(orderNo)
) ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"

#创建订单粉丝转移表
#orderNo 订单号==订单表的orderNo
#fans 转移的粉丝，逗号分隔
sql="create table IF NOT EXISTS fansbank_fansmove 
(
orderNo VARCHAR(64) NOT NULL comment '订单号==订单表的orderNo',
fans LongText NOT NULL default '' comment '转移的粉丝，逗号分隔',
PRIMARY KEY(orderNo)
) ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"

#创建配置信息表
sql="create table IF NOT EXISTS fansbank_config 
(
syncTimeTrade BIGINT NOT NULL comment '钱包附近消费时间段同步点 unix微秒',
syncPageTrade INT NOT NULL comment '钱包附近消费分页同步点 页码',
deValueDay VARCHAR(12) NOT NULL comment '计算贬值的最新日期 格式2017-08-04',
consumeSyncTime datetime comment '获取消费粉丝时间点 2018-02-11 14:05:21',
userId BIGINT NOT NULL comment '注册用户表同步到用户价值表，同步点'
) 
ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建粉丝交易配置表
sql="create table IF NOT EXISTS fansbank_keyvalue 
(fanskey VARCHAR(48) NOT NULL comment 'key',
fansvalue VARCHAR(128) NOT NULL comment 'value',
PRIMARY KEY(fanskey)
) comment='粉丝交易配置' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建粉丝数量表
sql="create table IF NOT EXISTS fansbank_count 
(id bigint NOT NULL comment '身份id',
type int NOT NULL comment '哪种身份1 -平台  2 -个代',
count int NOT NULL comment '粉丝数量',
PRIMARY KEY(id,type)
) comment='粉丝数量表' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建圈子发布信息
sql="create table IF NOT EXISTS community_publish 
(
  id VARCHAR(64) NOT NULL comment '唯一标记（unix秒+6位随机数)',
  userId bigint NOT NULL comment '发布者Id',
  nickName VARCHAR(64) NOT NULL comment '发布者昵称',
  headImage VARCHAR(256) NOT NULL comment '发布者头像',
  addTime datetime comment '发布时间',
  fineCount int NOT NULL default 0 comment '点赞次数',
  markCount int NOT NULL default 0 comment '评论次数',
  collectCount int NOT NULL default 0 comment '收藏次数',
  content longtext NOT NULL comment '内容',
  pic longtext NOT NULL comment '图片（数组）',
  authority int NOT NULL default 0 comment '权限 0-所有人可见 1-仅自己可见 2-仅好友可见',
  PRIMARY KEY(id)
) comment='圈子信息' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"


#创建圈子互动消息
sql="create table IF NOT EXISTS community_remark 
(
  id VARCHAR(64) NOT NULL comment '唯一标记（unix秒+6位随机数)',
  publishId VARCHAR(64) NOT NULL comment '圈子信息Id',
  userId bigint NOT NULL comment '互动者Id',
  nickName VARCHAR(64) NOT NULL comment '互动者昵称',
  headImage VARCHAR(256) NOT NULL comment '互动者头像',
  addTime datetime comment '评论时间',
  comment longtext NOT NULL comment '评论内容',
  type int NOT NULL  comment '类型 1-点赞 2-收藏 3-分享 4-评论 5-回复',
  touserId bigint NOT NULL comment '回复对像的Id',
  tonickName VARCHAR(64) NOT NULL comment '回复对像的昵称',
  toheadImage VARCHAR(256) NOT NULL comment '回复对像的头像',
  whoClear longtext NOT NULL comment '记录谁清空消息, 逗号分隔',
  PRIMARY KEY(id)
) comment='圈子互动消息' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"



#创建第三方支付信息表
sql="create table IF NOT EXISTS thirdpay 
(
  id VARCHAR(64) NOT NULL comment '唯一标记（unix秒+6位随机数)',
  openid VARCHAR(64) NOT NULL comment '第三方身份标记',
  phone bigint NOT NULL comment '关联手机号',
  platformId bigint NOT NULL comment '平台号',
  pushId bigint NOT NULL comment '推广人',
  addTime datetime comment '时间',
  money Decimal(10,2) Not Null comment '金额',
  thirdType int NOT NULL  comment '第三方类型',
  PRIMARY KEY(id)
) comment='第三方支付信息表' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"



#创建分帐信息表
sql="create table IF NOT EXISTS fenzhang 
(
  orderNo VARCHAR(64) NOT NULL comment '订单',
  addTime datetime comment '时间',
  info longtext  Not Null comment '分帐信息',
  PRIMARY KEY(orderNo)
) comment='分帐信息表' ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${sql}"
