#ifndef COMPTECONTROLLER_H
#define COMPTECONTROLLER_H

#include <QString>
#include <QVector>

class CompteBancaire;
class Transaction;
class Client;

class CompteController
{
public:
    CompteController();

    bool faire_depot(const QString& iban, double montant, const QString& desc);
    bool retir_montant(const QString& iban, double montant, const QString& desc);
    bool faireVirement(const QString& ibanSource, const QString& ibanDestination,
                       double montant, const QString& description);
    bool creerComptePourClient(const QString& iban, int typeIndex,
                               double soldeInitial, QString* erreur = nullptr);

    static void rechargerComptes();

    static QVector<CompteBancaire> getComptes();
    static CompteBancaire* chercherCompte(const QString& iban);
    static QVector<CompteBancaire> getComptesPourClient(int clientId);
    static const Client* getClientConnecte();
    static bool estAdmin();
    static int getClientConnecteId();

    static QVector<Transaction> getTransactions(const QString& iban);
    static QVector<Transaction> getToutesTransactions();

    static bool creerClientEtCompte(const QString& nom, const QString& prenom,
                                    const QString& email, const QString& telephone,
                                    const QString& login, const QString& motDePasse,
                                    const QString& iban, int typeIndex,
                                    double soldeInitial, QString* erreur = nullptr);

    static QString typeCompteString(int typeIndex);

    static QString getIban(const CompteBancaire& c);
    static double getSolde(const CompteBancaire& c);
    static QString getTypeString(const CompteBancaire& c);
    static int getClientId(const CompteBancaire& c);
    static double getSoldeMoyen(const CompteBancaire& c);
    static QVector<double> getSoldesMensuels(const CompteBancaire& c);
    static QVector<Transaction> getHistorique(const CompteBancaire& c, int n = 30);

    static QString getDateStr(const Transaction& t);
    static QString getTransactionType(const Transaction& t);
    static double getMontant(const Transaction& t);
    static double getSoldeApres(const Transaction& t);
    static QString getDescription(const Transaction& t);
    static int getTransactionId(const Transaction& t);
    static bool estUnRetrait(const Transaction& t);

    static QVector<Transaction> trierTransactionsParDate(const QVector<Transaction>& txns);
};

#endif
