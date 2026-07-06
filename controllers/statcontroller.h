#ifndef STATCONTROLLER_H
#define STATCONTROLLER_H

#include <QVector>

class CompteBancaire;
class Transaction;

class StatController
{
public:
    StatController();

    static double calculerSoldeTotal(const QVector<CompteBancaire>& comptes);
    static double calculerSoldeMoyen(const QVector<CompteBancaire>& comptes);
    static void calculerStatsTransactions(const QVector<Transaction>& transactions,
                                          double* totalDepots, double* totalRetraits);
    static QVector<double> construireCourbeSolde(const QVector<Transaction>& transactions,
                                                  double soldeActuel);
    static double montantSigne(const Transaction& transaction);
};

#endif
