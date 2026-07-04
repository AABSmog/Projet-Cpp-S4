#include "authcontroller.h"

#include "../data/datamanager.h"
#include <QCryptographicHash>
#include "../models/client.h"

AuthController::AuthController() {}
QString AuthController::hashPassword(const QString& password) const
{
    return QCryptographicHash::hash(
               password.toUtf8(),
               QCryptographicHash::Sha256
               ).toHex();
}

bool AuthController::verifierConnexion(const QString& login, const QString& motDePasse)
{
    Client client;
    if (!DataManager::instance().authentifier(login, hashPassword(motDePasse), &client)) {
        return false;
    }

    DataManager::instance().definirClientConnecte(client);
    return true;
}