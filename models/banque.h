#ifndef BANQUE_H
#define BANQUE_H

#include <QVector>
#include <QString>
#include "comptebancaire.h"

class Banque
{
private:
    static QVector<CompteBancaire> comptes;

public:
    static bool ajouterCompte(const CompteBancaire& compte);

    static CompteBancaire* chercherCompte(const QString& iban);

    static QVector<CompteBancaire> getComptes();
};

#endif // BANQUE_H