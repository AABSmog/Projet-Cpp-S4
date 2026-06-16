#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 1. Open the QSS file
    QFile file(":/styles/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        // 2. Read the stylesheet text
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();

        // 3. Apply it to the entire application
        app.setStyleSheet(styleSheet);
        file.close();
    }

    MainWindow w;
    w.show();
    return app.exec();
}
