#include "banque.h"

// Constructeur par défaut
Banque::Banque()
{
    idBanque = 0;
    nom = "";
    adresse = "";
    telephone = "";
    email = "";
}

// Constructeur paramétré
Banque::Banque(int idBanque,
               const QString& nom,
               const QString& adresse,
               const QString& telephone,
               const QString& email)
{
    this->idBanque = idBanque;
    this->nom = nom;
    this->adresse = adresse;
    this->telephone = telephone;
    this->email = email;
}

// Getters
int Banque::getIdBanque() const
{
    return idBanque;
}

QString Banque::getNom() const
{
    return nom;
}

QString Banque::getAdresse() const
{
    return adresse;
}

QString Banque::getTelephone() const
{
    return telephone;
}

QString Banque::getEmail() const
{
    return email;
}

// Setters
void Banque::setIdBanque(int idBanque)
{
    this->idBanque = idBanque;
}

void Banque::setNom(const QString& nom)
{
    this->nom = nom;
}

void Banque::setAdresse(const QString& adresse)
{
    this->adresse = adresse;
}

void Banque::setTelephone(const QString& telephone)
{
    this->telephone = telephone;
}

void Banque::setEmail(const QString& email)
{
    this->email = email;
}