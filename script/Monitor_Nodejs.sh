#author: suyp
#desc: Nodejs进程守护

#进程列表
ProcessList="ShopInfoSVR.js ZoneAgentSVR.js"

cd /tiyoume/https #进入运行路径
export NODE_PATH=/tiyoume/https/com/lib/ #配置NODE路径

while [ true ]
do
  sleep 1
  for Process in $ProcessList
  do
    if [ -f "$Process" ] ;then
      if [ ! -x "$Process" ]; then #没权限，加执行权限
        chmod a+x $Process
      fi
    else
      continue #不存在文件
    fi
    pname=`ps -ef | grep "$Process" | grep -v grep`
    if [ ! -n "$pname" ];then
      nodejs $Process & #进程没启动，启动它
    fi
  done
done
