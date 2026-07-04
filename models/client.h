#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client
{
public:
    Client();

    Client(int idClient,
           const QString& nom,
           const QString& prenom,
           const QString& email,
           const QString& telephone,
           const QString& login,
           const QString& motDePasseHash,
           int nbTentatives,
           const QString& dateVerrouillage);

    // Getters
    int getIdClient() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getLogin() const;
    QString getMotDePasseHash() const;
    int getNbTentatives() const;
    QString getDateVerrouillage() const;

    // Setters
    void setIdClient(int idClient);
    void setNom(const QString& nom);
    void setPrenom(const QString& prenom);
    void setEmail(const QString& email);
    void setTelephone(const QString& telephone);
    void setLogin(const QString& login);
    void setMotDePasseHash(const QString& motDePasseHash);
    void setNbTentatives(int nbTentatives);
    void setDateVerrouillage(const QString& dateVerrouillage);


private:
    int idClient;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;

    QString login;
    QString motDePasseHash;

    int nbTentatives;
    QString dateVerrouillage;
};

#endif // CLIENT_H
