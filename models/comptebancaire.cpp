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
        DataManager::instance().mettreAJourCompte(iban, solde, type, statut);
        return true;
    }
    bool CompteBancaire::retirer(double montant, const QString& desc){
        if (montant <= 0 || montant > solde)
            return false;

        solde -= montant;
        enregistrerTransaction(
            "retrait",
            montant,
            desc
            );
        DataManager::instance().mettreAJourCompte(iban, solde, type, statut);
        return true;
    }
    bool CompteBancaire::virer(CompteBancaire& dest, double montant){
        if (!retirer(montant, QString("Virement vers %1").arg(dest.getIBAN())))
            return false;

        dest.deposer(montant, QString("Virement depuis %1").arg(getIBAN()));
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
    QVector<Transaction> CompteBancaire::getHistorique(int n) const{
        if (n <= 0 || historique.size() <= n) {
            return historique;
        }

        return historique.mid(historique.size() - n);
    }
    void CompteBancaire::chargerHistorique(const QVector<Transaction>& transactions){
        historique = transactions;
    }
    // Statistiques
    double CompteBancaire::getSoldeMoyen(int jours ) const{
        if (historique.isEmpty()) {
            return solde;
        }

        const QDateTime limite = QDateTime::currentDateTime().addDays(-jours);
        double somme = 0.0;
        int compteur = 0;

        for (const Transaction& transaction : historique) {
            if (transaction.getDate() >= limite) {
                somme += transaction.getSoldeApres();
                ++compteur;
            }
        }

        return compteur > 0 ? somme / compteur : solde;
    }
    QVector<double> CompteBancaire::getSoldesMensuels(int mois ) const{
         QVector<double> soldes;
         if (mois <= 0) {
             return soldes;
         }

         const QDate today = QDate::currentDate();
         for (int i = mois - 1; i >= 0; --i) {
             const QDate monthDate = today.addMonths(-i);
             const QDate monthStart(monthDate.year(), monthDate.month(), 1);
             const QDate monthEnd = monthStart.addMonths(1);

             double soldeMois = solde;
             for (const Transaction& transaction : historique) {
                 const QDate transactionDate = transaction.getDate().date();
                 if (transactionDate >= monthStart && transactionDate < monthEnd) {
                     soldeMois = transaction.getSoldeApres();
                 }
             }

             soldes.append(soldeMois);
         }

         return soldes;
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
        DataManager::instance().enregistrerTransaction(iban, t, m, solde, d);
    }

