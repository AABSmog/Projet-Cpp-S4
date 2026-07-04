#include "fenetrestatistiques.h"

#include "widgets/graphiquesolde.h"
#include "widgets/camembertdepenses.h"
#include "widgets/indicateursolde.h"

#include "../data/datamanager.h"
#include "../models/banque.h"
#include "../models/transaction.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <algorithm>

static double montantSigne(const Transaction& transaction)
{
    return transaction.getType().contains("retrait", Qt::CaseInsensitive)
        ? -transaction.getMontant()
        : transaction.getMontant();
}

static QVector<double> construireCourbeSolde(const QVector<Transaction>& transactions, double soldeActuel)
{
    QVector<Transaction> tries = transactions;
    std::sort(tries.begin(), tries.end(), [](const Transaction& left, const Transaction& right) {
        if (left.getDate() == right.getDate()) {
            return left.getId() < right.getId();
        }
        return left.getDate() < right.getDate();
    });

    double cumul = soldeActuel;
    for (const Transaction& transaction : tries) {
        cumul -= montantSigne(transaction);
    }

    QVector<double> valeurs;
    valeurs.append(cumul);
    for (const Transaction& transaction : tries) {
        cumul += montantSigne(transaction);
        valeurs.append(cumul);
    }

    return valeurs;
}

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

    // Graphique
    GraphiqueSolde* graphique =
        new GraphiqueSolde;
    graphique->setMinimumHeight(220);

    // Camembert
    CamembertDepenses* camembert =
        new CamembertDepenses;
    camembert->setObjectName("statCamembert");

    mainLayout->addWidget(titre);
    mainLayout->addLayout(indicateursLayout);
    mainLayout->addWidget(graphique);
    mainLayout->addWidget(camembert);

    actualiser();
}

void FenetreStatistiques::actualiser()
{
    const QVector<CompteBancaire> comptes = DataManager::instance().chargerComptes();
    const QVector<Transaction> transactions = DataManager::instance().chargerToutesTransactions();

    double soldeActuel = 0.0;
    double revenusTotaux = 0.0;
    double depensesTotales = 0.0;
    for (const CompteBancaire& compte : comptes) {
        soldeActuel += compte.getSolde();
    }

    for (const Transaction& transaction : transactions) {
        if (transaction.getType().contains("retrait", Qt::CaseInsensitive)) {
            depensesTotales += transaction.getMontant();
        } else {
            revenusTotaux += transaction.getMontant();
        }
    }

    double moyenne = 0.0;
    if (!comptes.isEmpty()) {
        for (const CompteBancaire& compte : comptes) {
            moyenne += compte.getSolde();
        }
        moyenne /= comptes.size();
    }

    // Widgets enfants créés dans le constructeur, on rafraîchit via une reconstruction légère.
    if (auto* widgetSolde = findChild<IndicateurSolde*>("statSolde")) {
        widgetSolde->setValeur(QString::number(soldeActuel, 'f', 0) + " FCFA");
    }

    if (auto* widgetRevenus = findChild<IndicateurSolde*>("statRevenus")) {
        widgetRevenus->setValeur(QString::number(revenusTotaux, 'f', 0) + " FCFA");
    }

    if (auto* widgetDepenses = findChild<IndicateurSolde*>("statDepenses")) {
        widgetDepenses->setValeur(QString::number(depensesTotales, 'f', 0) + " FCFA");
    }

    if (auto* widgetMoyen = findChild<IndicateurSolde*>("statSoldeMoyen")) {
        widgetMoyen->setValeur(QString::number(moyenne, 'f', 0) + " FCFA");
    }

    auto* graphique = findChild<GraphiqueSolde*>();
    if (graphique != nullptr) {
        graphique->setDonnees(construireCourbeSolde(transactions, soldeActuel));
    }

    auto* camembert = findChild<CamembertDepenses*>("statCamembert");
    if (camembert != nullptr) {
        camembert->setDonnees(revenusTotaux, depensesTotales);
    }
}