#pragma once
#include <QString>
#include <QVector>
#include <QDateTime>
enum class TypeCompte { COURANT, EPARGNE, PROFESSIONNEL };
enum class StatutCompte { ACTIF, BLOQUE, FERME };
struct Transaction {
    int id;
    QDateTime date;
    QString type; // "depot", "retrait", "virement"
    double montant;
    double soldeApres;
    QString description;
};
class CompteBancaire {
public:
    CompteBancaire(const QString& iban, TypeCompte type,
                   double soldeInitial = 0.0);
    // Opérations
    bool deposer(double montant, const QString& desc = "");
    bool retirer(double montant, const QString& desc = "");
    bool virer(CompteBancaire& dest, double montant);
    // Accesseurs
    double getSolde() const { return solde; }
    QString getIBAN() const { return iban; }
    TypeCompte getType() const { return type; }
    StatutCompte getStatut() const { return statut; }
    QVector<Transaction> getHistorique(int n = 30) const;
    // Statistiques
    double getSoldeMoyen(int jours = 30) const;
    QVector<double> getSoldesMensuels(int mois = 12) const;
private:
    QString iban;
    double solde;
    TypeCompte type;
    StatutCompte statut;
    QVector<Transaction> historique;
    void enregistrerTransaction(const QString& t, double m, const QString& d);
};
