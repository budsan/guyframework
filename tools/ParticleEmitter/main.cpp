#include <QtGui/QApplication>
#include "particleeditor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParticleEditor w;
    w.show();

    return a.exec();
}
