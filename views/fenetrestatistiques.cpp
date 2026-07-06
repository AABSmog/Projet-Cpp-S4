#include "fenetrestatistiques.h"

#include "widgets/graphiquesolde.h"
#include "widgets/camembertdepenses.h"
#include "widgets/indicateursolde.h"

#include "../controllers/comptecontroller.h"
#include "../controllers/statcontroller.h"
#include "../models/comptebancaire.h"
#include "../models/transaction.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

FenetreStatistiques::FenetreStatistiques(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Statistiques");
    setMinimumSize(800, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titre = new QLabel("Statistiques Bancaires");
    titre->setAlignment(Qt::AlignCenter);

    QHBoxLayout* indicateursLayout = new QHBoxLayout;

    IndicateurSolde* solde = new IndicateurSolde("Solde actuel", "0 FCFA");
    solde->setObjectName("statSolde");

    IndicateurSolde* revenus = new IndicateurSolde("Revenus", "0 FCFA");
    revenus->setObjectName("statRevenus");

    IndicateurSolde* depenses = new IndicateurSolde("Depenses", "0 FCFA");
    depenses->setObjectName("statDepenses");

    IndicateurSolde* soldeMoyen = new IndicateurSolde("Solde moyen", "0 FCFA");
    soldeMoyen->setObjectName("statSoldeMoyen");

    indicateursLayout->addWidget(solde);
    indicateursLayout->addWidget(revenus);
    indicateursLayout->addWidget(depenses);
    indicateursLayout->addWidget(soldeMoyen);

    GraphiqueSolde* graphique = new GraphiqueSolde;
    graphique->setMinimumHeight(220);

    CamembertDepenses* camembert = new CamembertDepenses;
    camembert->setObjectName("statCamembert");

    mainLayout->addWidget(titre);
    mainLayout->addLayout(indicateursLayout);
    mainLayout->addWidget(graphique);
    mainLayout->addWidget(camembert);

    actualiser();
}

void FenetreStatistiques::actualiser()
{
    QVector<CompteBancaire> comptes = CompteController::getComptes();
    QVector<Transaction> transactions = CompteController::getToutesTransactions();

    double soldeActuel = StatController::calculerSoldeTotal(comptes);
    double totalDepots = 0.0, totalRetraits = 0.0;
    StatController::calculerStatsTransactions(transactions, &totalDepots, &totalRetraits);
    double moyenne = StatController::calculerSoldeMoyen(comptes);

    if (auto* widgetSolde = findChild<IndicateurSolde*>("statSolde"))
        widgetSolde->setValeur(QString::number(soldeActuel, 'f', 0) + " FCFA");
    if (auto* widgetRevenus = findChild<IndicateurSolde*>("statRevenus"))
        widgetRevenus->setValeur(QString::number(totalDepots, 'f', 0) + " FCFA");
    if (auto* widgetDepenses = findChild<IndicateurSolde*>("statDepenses"))
        widgetDepenses->setValeur(QString::number(totalRetraits, 'f', 0) + " FCFA");
    if (auto* widgetMoyen = findChild<IndicateurSolde*>("statSoldeMoyen"))
        widgetMoyen->setValeur(QString::number(moyenne, 'f', 0) + " FCFA");

    auto* graphique = findChild<GraphiqueSolde*>();
    if (graphique)
        graphique->setDonnees(StatController::construireCourbeSolde(transactions, soldeActuel));

    auto* camembert = findChild<CamembertDepenses*>("statCamembert");
    if (camembert)
        camembert->setDonnees(totalDepots, totalRetraits);
}
