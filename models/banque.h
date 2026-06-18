#ifndef BANQUE_H
#define BANQUE_H

#include <QString>

class Banque
{
private:
    int idBanque;
    QString nom;
    QString adresse;
    QString telephone;
    QString email;

public:
    Banque();

    Banque(int idBanque,
           const QString& nom,
           const QString& adresse,
           const QString& telephone,
           const QString& email);

    // Getters
    int getIdBanque() const;
    QString getNom() const;
    QString getAdresse() const;
    QString getTelephone() const;
    QString getEmail() const;

    // Setters
    void setIdBanque(int idBanque);
    void setNom(const QString& nom);
    void setAdresse(const QString& adresse);
    void setTelephone(const QString& telephone);
    void setEmail(const QString& email);
};

#endif // BANQUE_H