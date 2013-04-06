#include "mapreader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapReader w;
    w.show();
    
    return a.exec();
}
