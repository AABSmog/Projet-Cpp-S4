#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDateTime>

class Transaction
{
public:
    // Constructeur
     Transaction();
    Transaction(int id,
                const QDateTime& date,
                const QString& type,
                double montant,
                double soldeApres,
                const QString& description);

    // Getters
    int getId() const;
    QDateTime getDate() const;
    QString getType() const;
    double getMontant() const;
    double getSoldeApres() const;
    QString getDescription() const;

    // Setters
    void setId(int id);
    void setDate(const QDateTime& date);
    void setType(const QString& type);
    void setMontant(double montant);
    void setSoldeApres(double soldeApres);
    void setDescription(const QString& description);

private:
    int id;
    QDateTime date;
    QString type;        // "depot", "retrait", "virement"
    double montant;
    double soldeApres;
    QString description;
};

#endif // TRANSACTION_H