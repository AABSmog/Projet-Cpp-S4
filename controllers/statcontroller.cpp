#include "statcontroller.h"
#include "comptecontroller.h"
#include "../models/transaction.h"
#include "../models/comptebancaire.h"

StatController::StatController() {}

double StatController::montantSigne(const Transaction& transaction)
{
    return CompteController::estUnRetrait(transaction)
        ? -CompteController::getMontant(transaction)
        : CompteController::getMontant(transaction);
}

double StatController::calculerSoldeTotal(const QVector<CompteBancaire>& comptes)
{
    double total = 0.0;
    for (const CompteBancaire& c : comptes)
        total += CompteController::getSolde(c);
    return total;
}

double StatController::calculerSoldeMoyen(const QVector<CompteBancaire>& comptes)
{
    if (comptes.isEmpty()) return 0.0;
    return calculerSoldeTotal(comptes) / comptes.size();
}

void StatController::calculerStatsTransactions(const QVector<Transaction>& transactions,
                                                double* totalDepots, double* totalRetraits)
{
    double depots = 0.0, retraits = 0.0;
    for (const Transaction& t : transactions) {
        if (CompteController::estUnRetrait(t))
            retraits += CompteController::getMontant(t);
        else
            depots += CompteController::getMontant(t);
    }
    if (totalDepots) *totalDepots = depots;
    if (totalRetraits) *totalRetraits = retraits;
}

QVector<double> StatController::construireCourbeSolde(const QVector<Transaction>& transactions,
                                                        double soldeActuel)
{
    QVector<Transaction> tries = CompteController::trierTransactionsParDate(transactions);

    double cumul = soldeActuel;
    for (const Transaction& t : tries)
        cumul -= montantSigne(t);

    QVector<double> valeurs;
    valeurs.append(cumul);
    for (const Transaction& t : tries) {
        cumul += montantSigne(t);
        valeurs.append(cumul);
    }

    return valeurs;
}
