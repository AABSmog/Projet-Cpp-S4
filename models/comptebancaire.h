#ifndef COMPTEBANCAIRE_H
#define COMPTEBANCAIRE_H
#include "transaction.h"

enum class TypeCompte { COURANT, EPARGNE, PROFESSIONNEL };
enum class StatutCompte { ACTIF, BLOQUE, FERME };


class CompteBancaire {
public:
    //Constructeur
    CompteBancaire(const QString& iban, TypeCompte type,
                   double soldeInitial = 0.0, StatutCompte statut=StatutCompte::ACTIF);
    // Methodes de transaction
    bool deposer(double montant, const QString& desc = "");
    double retirer(double montant, const QString& desc = "");
    bool virer(CompteBancaire& dest, double montant);
    // Getter et setter
    double getSolde() const ;
    void setSolde(double sold) ;
    QString getIBAN() const ;
    void setIBAN(QString iban);
    TypeCompte getType() const ;
    void setType(TypeCompte type);
    StatutCompte getStatut() const ;
    void setStatut(StatutCompte statut);
    QVector<Transaction> getHistorique(int n = 30) const;
    // Statistiques
    double getSoldeMoyen(int jours = 30) const;
    QVector<double> getSoldesMensuels(int mois = 12) const;

private:
    //LES ATTRIBUTS
    QString iban;
    double solde;
    TypeCompte type;
    StatutCompte statut;
    QVector<Transaction> historique;
    void enregistrerTransaction(const QString& t, double m, const QString& d);
};

#endif // COMPTEBANCAIRE_H
