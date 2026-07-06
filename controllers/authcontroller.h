#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QString>

class AuthController
{
public:
    AuthController();

    QString hashPassword(const QString& password) const;
    bool verifierConnexion(const QString& login, const QString& motDePasse);
    static QString getNomClient();
    static QString getPrenomClient();
    static QString getEmailClient();
    static QString getTelephoneClient();
    static QString getLoginClient();
};

#endif
