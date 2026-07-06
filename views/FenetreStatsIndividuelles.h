#pragma once

#include <QWidget>
#include <QVector>

class CompteBancaire;
class QComboBox;
class IndicateurSolde;
class GraphiqueSolde;
class CamembertDepenses;

class FenetreStatsIndividuelles : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreStatsIndividuelles(QWidget *parent = nullptr);
    void actualiser();

private:
    void afficherCompte(int index);
    void afficherInfosCompte(const CompteBancaire& compte);

    QComboBox* cmbCompte;
    IndicateurSolde* carteSolde;
    IndicateurSolde* carteDepots;
    IndicateurSolde* carteRetraits;
    IndicateurSolde* carteNbTransactions;
    GraphiqueSolde* graphique;
    CamembertDepenses* camembert;
};
