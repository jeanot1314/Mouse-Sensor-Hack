
#include "view_window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    QApplication app(argc, argv);

    view_window win;
    win.show();

    app.exec();

    return app.exec();
}



