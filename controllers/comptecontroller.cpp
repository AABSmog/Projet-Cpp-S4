#include "comptecontroller.h"

CompteController::CompteController() {}


bool CompteController::faire_depot(const QString& iban,
                                  double montant,
                                  const QString& description)
{
    CompteBancaire* compte = Banque::chercherCompte(iban);

    if (compte == nullptr)
    {
        return false;
    }

    compte->deposer(montant, description);
}
//-----------------------------------------------------------------
double CompteController::retir_montant(const QString& iban,
                                     double montant,
                                     const QString& desc){
    CompteBancaire* compte = Banque::chercherCompte(iban);

    if (compte == nullptr)
    {
        return false;
    }

    return compte->retirer(montant, desc);

}
//-----------------------------------------------------------------
bool CompteController::faireVirement(
    const QString& ibanSource,
    const QString& ibanDestination,
    double montant,
    const QString& description)
{
    CompteBancaire* source = Banque::chercherCompte(ibanSource);
    CompteBancaire* destination = Banque::chercherCompte(ibanDestination);

    if (source == nullptr || destination == nullptr)
    {
        return false;
    }

    return source->virer(*destination, montant);
}