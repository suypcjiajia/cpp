#funtion: to rebuild project
#history: add by yuyugong on 2016-6-13
#sh ReBuild

project=`pwd |cut -d / -f 5`

rm -rf ../build-*
find . -name "*~"  | xargs rm -f
find . -name "*.autosave"  | xargs rm -f
rm -f ../../bin/$project

qmake -o ./ -r -spec linux-g++ CONFIG+=release

make clean
make
make clean



