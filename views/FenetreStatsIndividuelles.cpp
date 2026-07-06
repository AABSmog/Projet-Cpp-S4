#include "FenetreStatsIndividuelles.h"
#include "widgets/indicateursolde.h"
#include "widgets/graphiquesolde.h"
#include "widgets/camembertdepenses.h"
#include "../models/banque.h"
#include "../models/transaction.h"
#include "../data/datamanager.h"
#include "../models/client.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

static QString typeCompteString(TypeCompte type)
{
    switch (type) {
        case TypeCompte::COURANT: return "Courant";
        case TypeCompte::EPARGNE: return "Epargne";
        case TypeCompte::PROFESSIONNEL: return "Professionnel";
    }
    return "Inconnu";
}

FenetreStatsIndividuelles::FenetreStatsIndividuelles(QWidget *parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

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

    connect(cmbCompte, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FenetreStatsIndividuelles::afficherCompte);

    actualiser();
}

void FenetreStatsIndividuelles::actualiser()
{
    const Client* client = DataManager::instance().clientConnecte();
    const bool estAdmin = client != nullptr && client->getLogin() == "admin";
    int clientId = client ? client->getIdClient() : 0;
    const QVector<CompteBancaire> comptes = Banque::getComptes();

    cmbCompte->blockSignals(true);
    cmbCompte->clear();

    CompteBancaire* compteAffiche = nullptr;

    if (estAdmin) {
        cmbCompte->show();
        for (const auto& c : comptes) {
            cmbCompte->addItem(
                QString("%1 | %2 | %3 FCFA")
                    .arg(c.getIBAN(), typeCompteString(c.getType()))
                    .arg(c.getSolde(), 0, 'f', 0),
                c.getIBAN());
        }
        if (cmbCompte->count() > 0) {
            compteAffiche = Banque::chercherCompte(cmbCompte->itemData(0).toString());
        }
    } else {
        cmbCompte->hide();
        for (const auto& c : comptes) {
            if (c.getClientId() == clientId) {
                compteAffiche = Banque::chercherCompte(c.getIBAN());
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

void FenetreStatsIndividuelles::afficherCompte(int index)
{
    if (index < 0) return;

    const QString iban = cmbCompte->itemData(index).toString();
    CompteBancaire* compte = Banque::chercherCompte(iban);
    if (compte) {
        afficherInfosCompte(*compte);
    }
}

void FenetreStatsIndividuelles::afficherInfosCompte(const CompteBancaire& compte)
{
    carteSolde->setValeur(QString::number(compte.getSolde(), 'f', 0) + " FCFA");

    double totalDepots = 0.0, totalRetraits = 0.0;
    const auto historique = compte.getHistorique(1000);
    for (const auto& t : historique) {
        if (t.getType().contains("retrait", Qt::CaseInsensitive)) {
            totalRetraits += t.getMontant();
        } else {
            totalDepots += t.getMontant();
        }
    }

    carteDepots->setValeur(QString::number(totalDepots, 'f', 0) + " FCFA");
    carteRetraits->setValeur(QString::number(totalRetraits, 'f', 0) + " FCFA");
    carteNbTransactions->setValeur(QString::number(historique.size()));

    graphique->setDonnees(compte.getSoldesMensuels());
    camembert->setDonnees(totalDepots, totalRetraits);
}
