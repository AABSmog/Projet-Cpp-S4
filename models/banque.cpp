#include "banque.h"


QVector<CompteBancaire> Banque::comptes;



bool Banque::ajouterCompte(const CompteBancaire& compte)
{
    if (chercherCompte(compte.getIBAN()) != nullptr)
    {
        return false;
    }

    comptes.append(compte);
    return true;
}

void Banque::viderComptes()
{
    comptes.clear();
}

CompteBancaire* Banque::chercherCompte(const QString& iban)
{
    for (CompteBancaire& compte : comptes)
    {
        if (compte.getIBAN() == iban)
        {
            return &compte;
        }
    }

    return nullptr;
}

QVector<CompteBancaire> Banque::getComptes()
{
    return comptes;
}