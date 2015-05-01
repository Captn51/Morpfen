#include <QApplication>
#include "morpfen.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Morpfen m;
    m.show();
    return a.exec();
}

