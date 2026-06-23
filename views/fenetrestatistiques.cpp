#include "fenetrestatistiques.h"

#include "widgets/graphiquesolde.h"
#include "widgets/camembertdepenses.h"
#include "widgets/indicateursolde.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

FenetreStatistiques::FenetreStatistiques(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Statistiques");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titre = new QLabel("Statistiques Bancaires");
    titre->setAlignment(Qt::AlignCenter);

    // Cartes indicateurs
    QHBoxLayout* indicateursLayout = new QHBoxLayout;

    IndicateurSolde* solde =
        new IndicateurSolde(
            "Solde actuel",
            "150 000 FCFA"
            );

    IndicateurSolde* revenus =
        new IndicateurSolde(
            "Revenus",
            "80 000 FCFA"
            );

    IndicateurSolde* depenses =
        new IndicateurSolde(
            "Depenses",
            "35 000 FCFA"
            );

    indicateursLayout->addWidget(solde);
    indicateursLayout->addWidget(revenus);
    indicateursLayout->addWidget(depenses);

    // Graphique
    GraphiqueSolde* graphique =
        new GraphiqueSolde;

    // Camembert
    CamembertDepenses* camembert =
        new CamembertDepenses;

    mainLayout->addWidget(titre);
    mainLayout->addLayout(indicateursLayout);
    mainLayout->addWidget(graphique);
    mainLayout->addWidget(camembert);
}