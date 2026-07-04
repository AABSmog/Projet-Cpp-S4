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
    //elle va recevoir le depot (montant et desc) puis appelé la recherche du compte
    bool retir_montant(const QString& iban,
                      double montant,
                      const QString& desc);
     //elle va recevoir le montant (montant et desc) puis appelé la recherche du compte
    bool faireVirement(const QString& ibanSource,
                       const QString& ibanDestination,
                       double montant,
                       const QString& description
                       );
};

#endif // COMPTECONTROLLER_H
