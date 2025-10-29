#include <QApplication>
#include <QPushButton>
#include "Mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Mainwindow w;
    w.show();
    return QApplication::exec();
}