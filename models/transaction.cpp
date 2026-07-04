#include "transaction.h"

Transaction::Transaction()
        : id(0),
            montant(0.0),
            soldeApres(0.0)
{
}

// Constructeur
Transaction::Transaction(int id,
                         const QDateTime& date,
                         const QString& type,
                         double montant,
                         double soldeApres,
                         const QString& description)
{
    this->id = id;
    this->date = date;
    this->type = type;
    this->montant = montant;
    this->soldeApres = soldeApres;
    this->description = description;
}

// Getters
int Transaction::getId() const
{
    return id;
}

QDateTime Transaction::getDate() const
{
    return date;
}

QString Transaction::getType() const
{
    return type;
}

double Transaction::getMontant() const
{
    return montant;
}

double Transaction::getSoldeApres() const
{
    return soldeApres;
}

QString Transaction::getDescription() const
{
    return description;
}

// Setters
void Transaction::setId(int id)
{
    this->id = id;
}

void Transaction::setDate(const QDateTime& date)
{
    this->date = date;
}

void Transaction::setType(const QString& type)
{
    this->type = type;
}

void Transaction::setMontant(double montant)
{
    this->montant = montant;
}

void Transaction::setSoldeApres(double soldeApres)
{
    this->soldeApres = soldeApres;
}

void Transaction::setDescription(const QString& description)
{
    this->description = description;
}