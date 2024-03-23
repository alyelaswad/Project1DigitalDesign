#include "waveforms.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WaveForms w;
    w.show();
    return a.exec();
}
