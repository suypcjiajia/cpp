#重启java服务 write by suyp

processname=$1

if [ "$processname" = "help" ];then
 	echo "bash java_restart.sh agent
bash java_restart.sh allianceshops"
	exit 0
elif [ "$processname" = "" ];then
	echo "invalid param"
	exit 0
fi

line=`ps -ef | grep java | grep jar | grep "$processname"`

pid=`echo $line | cut -d' ' -f2`


if [ ! -n "$pid" ];then
	echo "cannot find process"
	exit 0
fi

kill -9 $pid
sleep 2
cd /project/"$processname"
nohup java -jar "$processname"-0.0.1-SNAPSHOT.jar &
