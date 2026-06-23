#include <QString>
#include <QVector>
#include <QDateTime>
#include "comptebancaire.h"
#include "../data/datamanager.h"

    //Constructeur
    CompteBancaire::CompteBancaire(const QString& iban, TypeCompte type,
                   double soldeInitial, StatutCompte statut){
        this->iban = iban;
        this->type = type;
        this->solde = soldeInitial;
        this->statut = statut;
    }
    // Methodes de transaction
    bool CompteBancaire::deposer(double montant, const QString& desc){
        if (montant<=0){
            return false;}
        this->solde+=montant;
        enregistrerTransaction(
            "depot",
            montant,
            desc
            );
        return true;
    }
    double CompteBancaire::retirer(double montant, const QString& desc){
        if (montant <= 0 || montant > solde)
            return false;

        solde -= montant;
        enregistrerTransaction(
            "retrait",
            montant,
            desc
            );
        return true;
    }
    bool CompteBancaire::virer(CompteBancaire& dest, double montant){
        if (!retirer(montant))
            return false;

        dest.deposer(montant);
        return true;
    }
    //----------------
    // Getter et setter
    double CompteBancaire::getSolde() const
    { return solde; }
    void CompteBancaire::setSolde(double sold){
         solde = sold;
    }
    QString CompteBancaire::getIBAN() const
    { return iban; }
    void CompteBancaire::setIBAN(QString iban){
         this->iban = iban;
    }
    TypeCompte CompteBancaire::getType() const
    { return type; }
    void CompteBancaire::setType(TypeCompte type){
         this->type = type;
    }
    StatutCompte CompteBancaire::getStatut() const { return statut; }
    void CompteBancaire::setStatut(StatutCompte statut)
    {this->statut = statut;}
    QVector<Transaction> CompteBancaire::getHistorique(int n) const{ return historique;}
    // Statistiques
    double CompteBancaire::getSoldeMoyen(int jours ) const{
        // À compléter plus tard
    }
    QVector<double> CompteBancaire::getSoldesMensuels(int mois ) const{
         // À compléter plus tard
    }

    void CompteBancaire::enregistrerTransaction(const QString& t, double m, const QString& d){
        Transaction tr(
            historique.size() + 1,
            QDateTime::currentDateTime(),
            t,
            m,
            solde,
            d
            );

        historique.append(tr);
    }

