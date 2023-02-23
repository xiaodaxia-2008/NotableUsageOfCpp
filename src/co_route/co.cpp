#include "CoObj.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    CoObj co;
    app.exec();
    return 0;
}