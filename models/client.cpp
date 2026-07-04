#include "client.h"

Client::Client()
    : idClient(0)
{
}

Client::Client(int idClient,
               const QString& nom,
               const QString& prenom,
               const QString& email,
               const QString& telephone,
               const QString& login,
               const QString& motDePasseHash,
               int nbTentatives,
               const QString& dateVerrouillage)
    : idClient(idClient),
      nom(nom),
      prenom(prenom),
      email(email),
      telephone(telephone),
      login(login),
      motDePasseHash(motDePasseHash),
      nbTentatives(nbTentatives),
      dateVerrouillage(dateVerrouillage)
{
}

int Client::getIdClient() const
{
    return idClient;
}

QString Client::getNom() const
{
    return nom;
}

QString Client::getPrenom() const
{
    return prenom;
}

QString Client::getEmail() const
{
    return email;
}

QString Client::getTelephone() const
{
    return telephone;
}

QString Client::getLogin() const
{
    return login;
}

QString Client::getMotDePasseHash() const
{
    return motDePasseHash;
}

int Client::getNbTentatives() const
{
    return nbTentatives;
}

QString Client::getDateVerrouillage() const
{
    return dateVerrouillage;
}

void Client::setIdClient(int idClient)
{
    this->idClient = idClient;
}

void Client::setNom(const QString& nom)
{
    this->nom = nom;
}

void Client::setPrenom(const QString& prenom)
{
    this->prenom = prenom;
}

void Client::setEmail(const QString& email)
{
    this->email = email;
}

void Client::setTelephone(const QString& telephone)
{
    this->telephone = telephone;
}

void Client::setLogin(const QString& login)
{
    this->login = login;
}

void Client::setMotDePasseHash(const QString& motDePasseHash)
{
    this->motDePasseHash = motDePasseHash;
}

void Client::setNbTentatives(int nbTentatives)
{
    this->nbTentatives = nbTentatives;
}

void Client::setDateVerrouillage(const QString& dateVerrouillage)
{
    this->dateVerrouillage = dateVerrouillage;
}
