#ifndef COMPTECONTROLLER_H
#define COMPTECONTROLLER_H

#include <QString>
#include <QVector>

class CompteBancaire;
class Transaction;
class Client;

class CompteController
{
public:
    CompteController();
    bool faire_depot(const QString& iban,
                     double montant,
                     const QString& desc);
    bool retir_montant(const QString& iban,
                      double montant,
                      const QString& desc);
    bool faireVirement(const QString& ibanSource,
                       const QString& ibanDestination,
                       double montant,
                       const QString& description);
    static void rechargerComptes();
};

#endif
