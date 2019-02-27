#include "tclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TClient w;
    w.setWindowTitle("Thursday");
    w.show();

    return a.exec();
}
