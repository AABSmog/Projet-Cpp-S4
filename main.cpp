#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "views/fenetreconnexion.h"
#include "data/datamanager.h"
#include "controllers/comptecontroller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile file(":/styles/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        app.setStyleSheet(styleSheet);
        file.close();
    }

    if (DataManager::instance().initialiser()) {
        CompteController::rechargerComptes();
    }

    FenetreConnexion w;
    w.show();
    return app.exec();
}
