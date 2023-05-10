#include "simpleplagiarismdetector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimplePlagiarismDetector w;
    w.show();
    return a.exec();
}
