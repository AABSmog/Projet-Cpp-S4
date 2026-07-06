#include "authcontroller.h"
#include "comptecontroller.h"

#include "../data/datamanager.h"
#include <QCryptographicHash>
#include "../models/client.h"

AuthController::AuthController() {}

QString AuthController::hashPassword(const QString& password) const
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

bool AuthController::verifierConnexion(const QString& login, const QString& motDePasse)
{
    Client client;
    if (!DataManager::instance().authentifier(login, hashPassword(motDePasse), &client))
        return false;

    DataManager::instance().definirClientConnecte(client);
    CompteController::rechargerComptes();
    return true;
}

QString AuthController::getNomClient()
{
    const Client* c = CompteController::getClientConnecte();
    return c ? c->getNom() : QString();
}

QString AuthController::getPrenomClient()
{
    const Client* c = CompteController::getClientConnecte();
    return c ? c->getPrenom() : QString();
}

QString AuthController::getEmailClient()
{
    const Client* c = CompteController::getClientConnecte();
    return c ? c->getEmail() : QString();
}

QString AuthController::getTelephoneClient()
{
    const Client* c = CompteController::getClientConnecte();
    return c ? c->getTelephone() : QString();
}

QString AuthController::getLoginClient()
{
    const Client* c = CompteController::getClientConnecte();
    return c ? c->getLogin() : QString();
}
