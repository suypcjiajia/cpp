#include "unistd.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    while(true){
        w.onTimerOut();
        sleep(60);
    }

    return a.exec();


}
