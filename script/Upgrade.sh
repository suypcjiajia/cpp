#!/bin/bash
#author: suyp
#date: 2017-03-30
#Program:  to upgrade the version of servers
#bash Upgrade -h for help
serverips=""
remotedir=""
remotename=""
function GetServerIps()
{
	case $1 in
		"im")
    	   serverips=("120.76.84.231" "120.76.153.193")
           ;;
        "shop")
			serverips=("120.76.154.15" "120.76.154.26")
			;;
		"push")
			serverips=("120.76.77.26" "120.76.78.130")
			;;
		"sms")
			serverips=("120.76.154.15")
			;;
		*)
			echo "GetServerIp err"
			exit 0
			;;
	esac
}

function GetRemoteDir()
{
	case $1 in
		"im")
    	   remotedir="/tiyoume/imserver/"
           ;;
        "shop")
			remotedir="/tiyoume/MepayHttp/"
			;;
		"push")
			remotedir="/tiyoume/push/"
			;;
		"sms")
			remotedir="/tiyoume/phonesms/"
			;;
		*)
			echo "GetRemoteDir err"
			exit 0
			;;
	esac

}

function GetRemoteName()
{
	case $1 in
		"im")
    	   remotename="imserver"
           ;;
        "shop")
			remotename="MepayHttp"
			;;
		"push")
			remotename="push"
			;;
		"sms")
			remotename="phonesms"
			;;
		*)
			echo "GetRemoteName err"
			exit 0
			;;
	esac

}

function PrintHelp()
{
	echo "input two params , first one is the full filename , the second is one of 'im shop push sms'"
}



localname=$1
serverid=$2

if [ "$1" == "-h" ] ; then
	PrintHelp
	exit 0
fi

if [ "$#" -ne 2 ] ; then
	echo "please input right param, -h for help "
	exit 0
fi

echo "localname: $localname"

if [ -f "$localname" ] ; then
	:
else
	echo "err: not fine file "
	exit 0
fi

chmod a+x "$localname"

GetServerIps $serverid
GetRemoteDir $serverid
GetRemoteName $serverid
remotefile=$remotedir$remotename
echo ${serverips[*]}
echo "remotefile:$remotefile"

rm "./restart.sh"
echo "pid=\`pgrep $remotename\`" >> "./restart.sh"
echo "kill -9 \$pid" >> "./restart.sh"
for dip in ${serverips[*]}
do	
	echo "update ip:$dip start..."
	echo "now rm file:$remotefile"
	sshpass -p Tym123456 ssh $dip "rm $remotefile"
	echo "now scp file:$localname"
	sshpass -p Tym123456 scp $localname $dip:$remotefile
	echo "now scp file restart.sh"
	sshpass -p Tym123456 scp './restart.sh' $dip:/home/
	echo "now ssh restart.sh"
	sshpass -p Tym123456 ssh $dip "bash /home/restart.sh"

	#get ver of the server
	dstver=`sshpass -p Tym123456 ssh $dip "$remotefile -v"`
	#get ver of local
	sourcever=`$localname -v`

	msg="update for $serverid $dip"
	if [ "$dstver" == "$sourcever" ] ; then
		echo "success $msg"
	else
		echo "fail $msg"
	fi
done
