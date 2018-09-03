#funtion: to rebuild project

#sh ReBuild

project=`pwd |cut -d / -f 6`
objdir=/mktmp

rm -f ../../bin/$project
mkdir $objdir
qmake -o $objdir -r -spec linux-g++ CONFIG+=release
cd $objdir
make clean
make



