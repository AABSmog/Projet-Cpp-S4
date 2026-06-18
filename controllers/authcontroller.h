#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H
#include <QString>
#include <QCryptographicHash>
class AuthController
{
public:
    AuthController();
    QString hashPassword(const QString& password);

};

#endif // AUTHCONTROLLER_H
