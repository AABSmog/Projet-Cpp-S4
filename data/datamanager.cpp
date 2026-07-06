#include "datamanager.h"
#include "../controllers/authcontroller.h"
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

DataManager::DataManager()
    : connectionName(QStringLiteral("bankvision_connection"))
    , clientConnecteEstValide(false)
{
}

DataManager& DataManager::instance()
{
    static DataManager instance;
    return instance;
}

bool DataManager::initialiser(const QString& databasePath)
{
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }

    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qDebug() << "Erreur :" << db.lastError().text();
        return false;
    }

    if (!creerSchemas()) {
        return false;
    }

    return initialiserDonneesDemo();
}

bool DataManager::creerSchemas()
{
    QSqlQuery query(db);

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS CLIENT ("
            "idClient INTEGER PRIMARY KEY AUTOINCREMENT,"
            "nom TEXT NOT NULL,"
            "prenom TEXT NOT NULL,"
            "email TEXT UNIQUE,"
            "telephone TEXT,"
            "login TEXT UNIQUE NOT NULL,"
            "motDePasseHash TEXT NOT NULL,"
            "nbTentatives INTEGER DEFAULT 0,"
            "dateVerrouillage TEXT)")) {
        qDebug() << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS COMPTEBANCAIRE ("
            "iban TEXT PRIMARY KEY,"
            "solde REAL NOT NULL,"
            "type INTEGER NOT NULL,"
            "statut INTEGER NOT NULL,"
            "idClient INTEGER,"
            "FOREIGN KEY(idClient) REFERENCES CLIENT(idClient))")) {
        qDebug() << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS TRANSACTIONS ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "iban TEXT NOT NULL,"
            "date TEXT NOT NULL,"
            "type TEXT NOT NULL,"
            "montant REAL NOT NULL,"
            "soldeApres REAL NOT NULL,"
            "description TEXT,"
            "FOREIGN KEY(iban) REFERENCES COMPTEBANCAIRE(iban))")) {
        qDebug() << query.lastError().text();
        return false;
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS PRET ("
            "idPret INTEGER PRIMARY KEY AUTOINCREMENT,"
            "iban TEXT,"
            "montant REAL NOT NULL,"
            "tauxInteret REAL NOT NULL,"
            "duree INTEGER NOT NULL,"
            "mensualite REAL NOT NULL,"
            "dateDemande TEXT NOT NULL,"
            "statut TEXT DEFAULT 'En attente',"
            "FOREIGN KEY(iban) REFERENCES COMPTEBANCAIRE(iban))")) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool DataManager::initialiserDonneesDemo()
{
    QSqlQuery query(db);

    if (query.exec("SELECT COUNT(*) FROM CLIENT") && query.next() && query.value(0).toInt() == 0) {
        AuthController auth;
        query.prepare(
            "INSERT INTO CLIENT (nom, prenom, email, telephone, login, motDePasseHash, nbTentatives, dateVerrouillage) "
            "VALUES (:nom, :prenom, :email, :telephone, :login, :hash, 0, '')");
        query.bindValue(":nom", "Admin");
        query.bindValue(":prenom", "BankVision");
        query.bindValue(":email", "admin@bankvision.local");
        query.bindValue(":telephone", "+000000000");
        query.bindValue(":login", "admin");
        query.bindValue(":hash", auth.hashPassword("admin123"));
        if (!query.exec()) {
            qDebug() << query.lastError().text();
            return false;
        }
    }

    if (query.exec("SELECT COUNT(*) FROM COMPTEBANCAIRE") && query.next() && query.value(0).toInt() == 0) {
        QSqlQuery clientQuery(db);
        clientQuery.prepare("SELECT idClient FROM CLIENT WHERE login = :login");
        clientQuery.bindValue(":login", "admin");
        if (!clientQuery.exec() || !clientQuery.next()) {
            qDebug() << clientQuery.lastError().text();
            return false;
        }

        const int idClient = clientQuery.value(0).toInt();
        const QDateTime now = QDateTime::currentDateTime();

        QSqlQuery compteQuery(db);
        compteQuery.prepare(
            "INSERT INTO COMPTEBANCAIRE (iban, solde, type, statut, idClient) "
            "VALUES (:iban, :solde, :type, :statut, :idClient)");

        const struct {
            const char* iban;
            double solde;
            int type;
        } comptes[] = {
            {"BV-0001", 150000.0, static_cast<int>(TypeCompte::COURANT)},
            {"BV-0002", 80000.0, static_cast<int>(TypeCompte::EPARGNE)}
        };

        for (const auto& compte : comptes) {
            compteQuery.bindValue(":iban", compte.iban);
            compteQuery.bindValue(":solde", compte.solde);
            compteQuery.bindValue(":type", compte.type);
            compteQuery.bindValue(":statut", static_cast<int>(StatutCompte::ACTIF));
            compteQuery.bindValue(":idClient", idClient);
            if (!compteQuery.exec()) {
                qDebug() << compteQuery.lastError().text();
                return false;
            }
        }

        QSqlQuery transactionQuery(db);
        transactionQuery.prepare(
            "INSERT INTO TRANSACTIONS (iban, date, type, montant, soldeApres, description) "
            "VALUES (:iban, :date, :type, :montant, :soldeApres, :description)");

        transactionQuery.bindValue(":iban", "BV-0001");
        transactionQuery.bindValue(":date", now.addMonths(-2).toString(Qt::ISODate));
        transactionQuery.bindValue(":type", "depot");
        transactionQuery.bindValue(":montant", 150000.0);
        transactionQuery.bindValue(":soldeApres", 150000.0);
        transactionQuery.bindValue(":description", "Capital initial");
        if (!transactionQuery.exec()) {
            qDebug() << transactionQuery.lastError().text();
            return false;
        }

        transactionQuery.bindValue(":iban", "BV-0001");
        transactionQuery.bindValue(":date", now.addMonths(-1).toString(Qt::ISODate));
        transactionQuery.bindValue(":type", "retrait");
        transactionQuery.bindValue(":montant", 25000.0);
        transactionQuery.bindValue(":soldeApres", 125000.0);
        transactionQuery.bindValue(":description", "Frais de service");
        if (!transactionQuery.exec()) {
            qDebug() << transactionQuery.lastError().text();
            return false;
        }
    }

    return true;
}

bool DataManager::authentifier(const QString& login, const QString& motDePasseHash, Client* client)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT idClient, nom, prenom, email, telephone, login, motDePasseHash, nbTentatives, dateVerrouillage "
        "FROM CLIENT WHERE login = :login AND motDePasseHash = :hash");
    query.bindValue(":login", login);
    query.bindValue(":hash", motDePasseHash);

    if (!query.exec() || !query.next()) {
        return false;
    }

    if (client != nullptr) {
        client->setIdClient(query.value("idClient").toInt());
        client->setNom(query.value("nom").toString());
        client->setPrenom(query.value("prenom").toString());
        client->setEmail(query.value("email").toString());
        client->setTelephone(query.value("telephone").toString());
        client->setLogin(query.value("login").toString());
        client->setMotDePasseHash(query.value("motDePasseHash").toString());
        client->setNbTentatives(query.value("nbTentatives").toInt());
        client->setDateVerrouillage(query.value("dateVerrouillage").toString());

        clientCourant = *client;
        clientConnecteEstValide = true;
    }

    return true;
}

void DataManager::definirClientConnecte(const Client& client)
{
    clientCourant = client;
    clientConnecteEstValide = true;
}

const Client* DataManager::clientConnecte() const
{
    return clientConnecteEstValide ? &clientCourant : nullptr;
}

int DataManager::trouverIdClientParLogin(const QString& login) const
{
    QSqlQuery query(db);
    query.prepare("SELECT idClient FROM CLIENT WHERE login = :login");
    query.bindValue(":login", login);

    if (!query.exec() || !query.next()) {
        return 0;
    }

    return query.value(0).toInt();
}

bool DataManager::creerClientEtCompte(const QString& nom,
                                      const QString& prenom,
                                      const QString& email,
                                      const QString& telephone,
                                      const QString& login,
                                      const QString& motDePasseHash,
                                      const QString& iban,
                                      TypeCompte typeCompte,
                                      double soldeInitial,
                                      QString* erreur)
{
    if (nom.trimmed().isEmpty() || prenom.trimmed().isEmpty() || login.trimmed().isEmpty() || iban.trimmed().isEmpty()) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("Les champs obligatoires sont vides.");
        }
        return false;
    }

    if (soldeInitial < 0.0) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("Le solde initial doit etre positif.");
        }
        return false;
    }

    if (trouverIdClientParLogin(login) != 0) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("Ce login existe deja.");
        }
        return false;
    }

    QSqlQuery query(db);
    if (!db.transaction()) {
        if (erreur != nullptr) {
            *erreur = db.lastError().text();
        }
        return false;
    }

    query.prepare(
        "INSERT INTO CLIENT (nom, prenom, email, telephone, login, motDePasseHash, nbTentatives, dateVerrouillage) "
        "VALUES (:nom, :prenom, :email, :telephone, :login, :hash, 0, '')");
    query.bindValue(":nom", nom.trimmed());
    query.bindValue(":prenom", prenom.trimmed());
    query.bindValue(":email", email.trimmed());
    query.bindValue(":telephone", telephone.trimmed());
    query.bindValue(":login", login.trimmed());
    query.bindValue(":hash", motDePasseHash);

    if (!query.exec()) {
        if (erreur != nullptr) {
            *erreur = query.lastError().text();
        }
        db.rollback();
        return false;
    }

    const int idClient = query.lastInsertId().toInt();
    if (idClient <= 0) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("Impossible de recuperer l'identifiant du client.");
        }
        db.rollback();
        return false;
    }

    query.prepare(
        "INSERT INTO COMPTEBANCAIRE (iban, solde, type, statut, idClient) "
        "VALUES (:iban, :solde, :type, :statut, :idClient)");
    query.bindValue(":iban", iban.trimmed());
    query.bindValue(":solde", soldeInitial);
    query.bindValue(":type", static_cast<int>(typeCompte));
    query.bindValue(":statut", static_cast<int>(StatutCompte::ACTIF));
    query.bindValue(":idClient", idClient);

    if (!query.exec()) {
        if (erreur != nullptr) {
            *erreur = query.lastError().text();
        }
        db.rollback();
        return false;
    }

    QSqlQuery transactionQuery(db);
    transactionQuery.prepare(
        "INSERT INTO TRANSACTIONS (iban, date, type, montant, soldeApres, description) "
        "VALUES (:iban, :date, :type, :montant, :soldeApres, :description)");
    transactionQuery.bindValue(":iban", iban.trimmed());
    transactionQuery.bindValue(":date", QDateTime::currentDateTime().toString(Qt::ISODate));
    transactionQuery.bindValue(":type", "depot_initial");
    transactionQuery.bindValue(":montant", soldeInitial);
    transactionQuery.bindValue(":soldeApres", soldeInitial);
    transactionQuery.bindValue(":description", QStringLiteral("Depot initial"));

    if (!transactionQuery.exec()) {
        if (erreur != nullptr) {
            *erreur = transactionQuery.lastError().text();
        }
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        if (erreur != nullptr) {
            *erreur = db.lastError().text();
        }
        db.rollback();
        return false;
    }

    return true;
}

bool DataManager::creerComptePourClient(const QString& iban,
                                       TypeCompte typeCompte,
                                       double soldeInitial,
                                       QString* erreur)
{
    if (!clientConnecteEstValide || clientCourant.getIdClient() <= 0) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("Aucun client connecte.");
        }
        return false;
    }

    if (iban.trimmed().isEmpty()) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("IBAN invalide.");
        }
        return false;
    }

    if (soldeInitial < 0.0) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("Le solde initial doit etre positif.");
        }
        return false;
    }

    if (selectCompte(iban) != nullptr) {
        if (erreur != nullptr) {
            *erreur = QStringLiteral("Cet IBAN existe deja.");
        }
        return false;
    }

    QSqlQuery query(db);
    if (!db.transaction()) {
        if (erreur != nullptr) {
            *erreur = db.lastError().text();
        }
        return false;
    }

    query.prepare(
        "INSERT INTO COMPTEBANCAIRE (iban, solde, type, statut, idClient) "
        "VALUES (:iban, :solde, :type, :statut, :idClient)");
    query.bindValue(":iban", iban.trimmed());
    query.bindValue(":solde", soldeInitial);
    query.bindValue(":type", static_cast<int>(typeCompte));
    query.bindValue(":statut", static_cast<int>(StatutCompte::ACTIF));
    query.bindValue(":idClient", clientCourant.getIdClient());

    if (!query.exec()) {
        if (erreur != nullptr) {
            *erreur = query.lastError().text();
        }
        db.rollback();
        return false;
    }

    QSqlQuery transactionQuery(db);
    transactionQuery.prepare(
        "INSERT INTO TRANSACTIONS (iban, date, type, montant, soldeApres, description) "
        "VALUES (:iban, :date, :type, :montant, :soldeApres, :description)");
    transactionQuery.bindValue(":iban", iban.trimmed());
    transactionQuery.bindValue(":date", QDateTime::currentDateTime().toString(Qt::ISODate));
    transactionQuery.bindValue(":type", "ouverture_compte");
    transactionQuery.bindValue(":montant", soldeInitial);
    transactionQuery.bindValue(":soldeApres", soldeInitial);
    transactionQuery.bindValue(":description", QStringLiteral("Ouverture du compte"));

    if (!transactionQuery.exec()) {
        if (erreur != nullptr) {
            *erreur = transactionQuery.lastError().text();
        }
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        if (erreur != nullptr) {
            *erreur = db.lastError().text();
        }
        db.rollback();
        return false;
    }

    return true;
}

bool DataManager::enregistrerTransaction(const QString& iban,
                                         const QString& type,
                                         double montant,
                                         double soldeApres,
                                         const QString& description)
{
    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO TRANSACTIONS (iban, date, type, montant, soldeApres, description) "
        "VALUES (:iban, :date, :type, :montant, :soldeApres, :description)");
    query.bindValue(":iban", iban);
    query.bindValue(":date", QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(":type", type);
    query.bindValue(":montant", montant);
    query.bindValue(":soldeApres", soldeApres);
    query.bindValue(":description", description);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool DataManager::mettreAJourCompte(const QString& iban,
                                   double solde,
                                   TypeCompte type,
                                   StatutCompte statut)
{
    QSqlQuery query(db);
    query.prepare(
        "UPDATE COMPTEBANCAIRE SET solde = :solde, type = :type, statut = :statut WHERE iban = :iban");
    query.bindValue(":solde", solde);
    query.bindValue(":type", static_cast<int>(type));
    query.bindValue(":statut", static_cast<int>(statut));
    query.bindValue(":iban", iban);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

QVector<CompteBancaire> DataManager::chargerComptes() const
{
    QVector<CompteBancaire> comptes;
    QSqlQuery query(db);
    if (!query.exec("SELECT iban, solde, type, statut, idClient FROM COMPTEBANCAIRE ORDER BY iban")) {
        return comptes;
    }

    while (query.next()) {
        CompteBancaire cb(
            query.value("iban").toString(),
            static_cast<TypeCompte>(query.value("type").toInt()),
            query.value("solde").toDouble(),
            static_cast<StatutCompte>(query.value("statut").toInt()),
            query.value("idClient").toInt());
        comptes.append(cb);
    }

    return comptes;
}

QVector<Transaction> DataManager::chargerTransactions(const QString& iban) const
{
    QVector<Transaction> transactions;
    QSqlQuery query(db);
    query.prepare(
        "SELECT id, date, type, montant, soldeApres, description "
        "FROM TRANSACTIONS WHERE iban = :iban ORDER BY date ASC, id ASC");
    query.bindValue(":iban", iban);

    if (!query.exec()) {
        return transactions;
    }

    while (query.next()) {
        transactions.append(Transaction(
            query.value("id").toInt(),
            QDateTime::fromString(query.value("date").toString(), Qt::ISODate),
            query.value("type").toString(),
            query.value("montant").toDouble(),
            query.value("soldeApres").toDouble(),
            query.value("description").toString()));
    }

    return transactions;
}

QVector<Transaction> DataManager::chargerToutesTransactions() const
{
    QVector<Transaction> transactions;
    QSqlQuery query(db);
    if (!query.exec(
            "SELECT id, date, type, montant, soldeApres, description "
            "FROM TRANSACTIONS ORDER BY date DESC, id DESC")) {
        return transactions;
    }

    while (query.next()) {
        transactions.append(Transaction(
            query.value("id").toInt(),
            QDateTime::fromString(query.value("date").toString(), Qt::ISODate),
            query.value("type").toString(),
            query.value("montant").toDouble(),
            query.value("soldeApres").toDouble(),
            query.value("description").toString()));
    }

    return transactions;
}

CompteBancaire* DataManager::selectCompte(const QString& iban) const
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT iban, solde, type, statut, idClient "
        "FROM COMPTEBANCAIRE "
        "WHERE iban = :iban"
        );

    query.bindValue(":iban", iban);

    if(query.exec() && query.next())
    {
        QString ibanCompte = query.value("iban").toString();
        double solde = query.value("solde").toDouble();

        TypeCompte type =
            static_cast<TypeCompte>(query.value("type").toInt());

        StatutCompte statut =
            static_cast<StatutCompte>(query.value("statut").toInt());

        int idClient = query.value("idClient").toInt();

        return new CompteBancaire(
            ibanCompte,
            type,
            solde,
            statut,
            idClient
            );
    }

    return nullptr;
}