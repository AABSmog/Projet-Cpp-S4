#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QString>
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "../models/client.h"
#include "../models/comptebancaire.h"

class DataManager
{
public:
    static DataManager& instance();

    bool initialiser(const QString& databasePath = QStringLiteral("banque.db"));
    bool authentifier(const QString& login, const QString& motDePasseHash, Client* client = nullptr);
    void definirClientConnecte(const Client& client);
    const Client* clientConnecte() const;
    bool creerClientEtCompte(const QString& nom,
                             const QString& prenom,
                             const QString& email,
                             const QString& telephone,
                             const QString& login,
                             const QString& motDePasseHash,
                             const QString& iban,
                             TypeCompte typeCompte,
                             double soldeInitial,
                             QString* erreur = nullptr);

    bool creerComptePourClient(const QString& iban,
                               TypeCompte typeCompte,
                               double soldeInitial,
                               QString* erreur = nullptr);

    bool enregistrerTransaction(const QString& iban,
                                const QString& type,
                                double montant,
                                double soldeApres,
                                const QString& description);

    bool mettreAJourCompte(const QString& iban,
                           double solde,
                           TypeCompte type,
                           StatutCompte statut);

    QVector<CompteBancaire> chargerComptes() const;
    QVector<Transaction> chargerTransactions(const QString& iban) const;
    QVector<Transaction> chargerToutesTransactions() const;
    CompteBancaire* selectCompte(const QString& iban) const;

private:
    DataManager();

    bool creerSchemas();
    bool initialiserDonneesDemo();
    int trouverIdClientParLogin(const QString& login) const;

    QString connectionName;
    QSqlDatabase db;
    Client clientCourant;
    bool clientConnecteEstValide;
};

#endif // DATAMANAGER_H
