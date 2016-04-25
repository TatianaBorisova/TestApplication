#include <QApplication>
#include <QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    // For correct encoding
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    w.showTestView(TestStartView);
    w.startServerSearch();

    return a.exec();
}
