#include "fenetretableaubord.h"
#include "widgets/indicateursolde.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>

FenetreTableauBord::FenetreTableauBord(QWidget *parent)
    : QWidget(parent)
{
    auto* principal = new QVBoxLayout(this);

    auto* titre = new QLabel("Tableau de Bord");

    auto* cartes = new QHBoxLayout;

    cartes->addWidget(
        new IndicateurSolde(
            "Solde",
            "150 000 FCFA")
        );

    cartes->addWidget(
        new IndicateurSolde(
            "Comptes",
            "3")
        );

    cartes->addWidget(
        new IndicateurSolde(
            "Prêts",
            "80 000 FCFA")
        );

    tableHistorique = new QTableWidget;

    principal->addWidget(titre);
    principal->addLayout(cartes);
    principal->addWidget(tableHistorique);
}