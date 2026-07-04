#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "views/fenetreconnexion.h"
#include "data/datamanager.h"
#include "models/banque.h"

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

    if (DataManager::instance().initialiser()) {
        Banque::viderComptes();
        const QVector<CompteBancaire> comptes = DataManager::instance().chargerComptes();
        for (const CompteBancaire& compte : comptes) {
            CompteBancaire copie = compte;
            copie.chargerHistorique(DataManager::instance().chargerTransactions(copie.getIBAN()));
            Banque::ajouterCompte(copie);
        }
    }

    FenetreConnexion w;
    w.show();
    return app.exec();
}
