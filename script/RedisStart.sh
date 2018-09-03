#!/bin/sh


path=/tiyoume/redis_cluster/

cd $path

cd 7000
../redis-server redis.conf &
sleep 2

cd ../7001
../redis-server redis.conf &
sleep 2

cd ../7002
../redis-server redis.conf &
sleep 2
