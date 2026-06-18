#include "comptecontroller.h"

CompteController::CompteController() {}


bool CompteController::faire_depot(const QString& iban,
                                  double montant,
                                  const QString& description)
{
    // Recherche du compte
    CompteBancaire* compte = banque->chercherCompte(iban);

    if (compte == nullptr)
    {
        return false;
    }

    return compte->deposer(montant, description);
}
bool CompteController::retir_montant(const QString& iban,
                                     double montant,
                                     const QString& desc){
    CompteBancaire* compte=banque->chercherCompte(iban);
    if (compte == nullptr)
    {
        return false;
    }

    return compte->retirer(montant, description);

}
bool CompteController::faireVirement(
    const QString& ibanSource,
    const QString& ibanDestination,
    double montant,
    const QString& description)
{
    CompteBancaire* source =
        compteData.chercherCompte(ibanSource);

    CompteBancaire* destination =
        compteData.chercherCompte(ibanDestination);

    if(source == nullptr || destination == nullptr)
    {
        return false;
    }

    return source->virer(*destination, montant);
}