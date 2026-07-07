#include "FenetreStatsIndividuelles.h"
#include "widgets/indicateursolde.h"
#include "widgets/graphiquesolde.h"
#include "widgets/camembertdepenses.h"
#include "../controllers/comptecontroller.h"
#include "../controllers/statcontroller.h"
#include "../models/comptebancaire.h"
#include "../models/transaction.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>

FenetreStatsIndividuelles::FenetreStatsIndividuelles(QWidget *parent)
    : QWidget(parent)
{
    auto* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* content = new QWidget;
    auto* layout = new QVBoxLayout(content);

    auto* titre = new QLabel("Mon Compte");
    titre->setAlignment(Qt::AlignCenter);

    cmbCompte = new QComboBox;

    auto* indicateursLayout = new QHBoxLayout;
    carteSolde = new IndicateurSolde("Solde actuel", "0 FCFA");
    carteDepots = new IndicateurSolde("Total depots", "0 FCFA");
    carteRetraits = new IndicateurSolde("Total retraits", "0 FCFA");
    carteNbTransactions = new IndicateurSolde("Transactions", "0");

    indicateursLayout->addWidget(carteSolde);
    indicateursLayout->addWidget(carteDepots);
    indicateursLayout->addWidget(carteRetraits);
    indicateursLayout->addWidget(carteNbTransactions);

    graphique = new GraphiqueSolde;
    graphique->setMinimumHeight(200);

    camembert = new CamembertDepenses;

    layout->addWidget(titre);
    layout->addWidget(cmbCompte);
    layout->addLayout(indicateursLayout);
    layout->addWidget(graphique);
    layout->addWidget(camembert);
    layout->addStretch();

    scrollArea->setWidget(content);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(scrollArea);

    connect(cmbCompte, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FenetreStatsIndividuelles::afficherCompte);

    actualiser();
}

void FenetreStatsIndividuelles::actualiser()
{
    const bool estAdmin = CompteController::estAdmin();
    int clientId = CompteController::getClientConnecteId();
    const QVector<CompteBancaire> comptes = CompteController::getComptes();

    cmbCompte->blockSignals(true);
    cmbCompte->clear();

    CompteBancaire* compteAffiche = nullptr;

    if (estAdmin) {
        cmbCompte->show();
        for (const auto& c : comptes) {
            cmbCompte->addItem(
                QString("%1 | %2 | %3 FCFA")
                    .arg(CompteController::getIban(c),
                         CompteController::getTypeString(c))
                    .arg(CompteController::getSolde(c), 0, 'f', 0),
                CompteController::getIban(c));
        }
        if (cmbCompte->count() > 0) {
            compteAffiche = CompteController::chercherCompte(cmbCompte->itemData(0).toString());
        }
    } else {
        cmbCompte->hide();
        for (const auto& c : comptes) {
            if (CompteController::getClientId(c) == clientId) {
                compteAffiche = CompteController::chercherCompte(CompteController::getIban(c));
                break;
            }
        }
    }

    cmbCompte->blockSignals(false);

    if (compteAffiche) {
        afficherInfosCompte(*compteAffiche);
    } else if (!comptes.isEmpty()) {
        afficherInfosCompte(comptes.first());
    }
}

void FenetreStatsIndividuelles::actualiserPourCompte(const QString& iban)
{
    if (iban.isEmpty()) { actualiser(); return; }
    CompteBancaire* compte = CompteController::chercherCompte(iban);
    if (compte) afficherInfosCompte(*compte);
}

void FenetreStatsIndividuelles::afficherCompte(int index)
{
    if (index < 0) return;

    const QString iban = cmbCompte->itemData(index).toString();
    CompteBancaire* compte = CompteController::chercherCompte(iban);
    if (compte) {
        afficherInfosCompte(*compte);
    }
}

void FenetreStatsIndividuelles::afficherInfosCompte(const CompteBancaire& compte)
{
    carteSolde->setValeur(QString::number(CompteController::getSolde(compte), 'f', 0) + " FCFA");

    double totalDepots = 0.0, totalRetraits = 0.0;
    const auto historique = CompteController::getHistorique(compte, 1000);
    for (const auto& t : historique) {
        if (CompteController::estUnRetrait(t)) {
            totalRetraits += CompteController::getMontant(t);
        } else {
            totalDepots += CompteController::getMontant(t);
        }
    }

    carteDepots->setValeur(QString::number(totalDepots, 'f', 0) + " FCFA");
    carteRetraits->setValeur(QString::number(totalRetraits, 'f', 0) + " FCFA");
    carteNbTransactions->setValeur(QString::number(historique.size()));

    graphique->setDonnees(CompteController::getSoldesMensuels(compte));
    camembert->setDonnees(totalDepots, totalRetraits);
}
