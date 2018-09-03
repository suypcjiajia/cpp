#author: suyp
#desc: kill locked connect for mysql

host=rm-wz9c618cpa59o812l.mysql.rds.aliyuncs.com
user=wallet
db=walletdb
pwd=Wallet1234


while [ true ]
do
  sleep 5
  lockplst=`mysql -h $host -u $user -p$pwd -D$db -e 'show processlist' | grep -i lock | awk '{print $1}'`
  for lockp in $lockplst
  do
     #echo $lockp
     killcmd="mysql -h $host -u $user -p$pwd -D$db -e 'kill $lockp'"
     echo $killcmd > /home/killcmd.sh
     sh /home/killcmd.sh
     sleep 1
  done
done
