#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QString>

class AuthController
{
public:
    AuthController();

    QString hashPassword(const QString& password) const;
    bool verifierConnexion(const QString& login, const QString& motDePasse);

};

#endif // AUTHCONTROLLER_H
