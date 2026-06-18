#include "authcontroller.h"

AuthController::AuthController() {}
QString AuthController::hashPassword(const QString& password)
{
    return QCryptographicHash::hash(
               password.toUtf8(),
               QCryptographicHash::Sha256
               ).toHex();
}