#ifndef COMPTECONTROLLER_H
#define COMPTECONTROLLER_H
#include "../models/banque.h"
#include "../models/comptebancaire.h"
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

#endif // COMPTECONTROLLER_H
