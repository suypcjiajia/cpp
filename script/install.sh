#!/bin/bash
#author: suyp
#bash install.sh

if [ ! -d "/tiyoume/lib/" ] ;then
  mkdir -p /tiyoume/lib/
fi

#改权限
#绝对路径的文件列表，空格或回车符分隔
filelist="/tiyoume/Demon /tiyoume/ShopInfo/ShopInfo /tiyoume/Monitor_Demon.sh /tiyoume/WalletAccount/WalletAccount"
for file in $filelist
do
  if [ -f "$file" ] ;then
    if [ ! -x "$file" ]; then #没权限，加执行权限
      chmod 755 $file
    fi
  else
    echo "not exist $file"
    continue #不存在文件
  fi
done
chmod 755 /tiyoume/lib/*
#配置运行库的目录环境
echo "/tiyoume/lib" > /etc/ld.so.conf.d/tiyoume.conf
ldconfig
echo "ok"


