#include "datamanager.h"
#include "../controllers/authcontroller.h"
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QRandomGenerator>

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
    if (query.exec("SELECT COUNT(*) FROM CLIENT") && query.next() && query.value(0).toInt() > 0)
        return true;

    auto rng = QRandomGenerator::global();
    AuthController auth;
    const QString hash = auth.hashPassword("admin123");

    query.prepare(
        "INSERT INTO CLIENT (nom, prenom, email, telephone, login, motDePasseHash, nbTentatives, dateVerrouillage) "
        "VALUES (:nom, :prenom, :email, :telephone, :login, :hash, 0, '')");
    query.bindValue(":nom", "Admin");
    query.bindValue(":prenom", "BankVision");
    query.bindValue(":email", "admin@bankvision.local");
    query.bindValue(":telephone", "+000000000");
    query.bindValue(":login", "admin");
    query.bindValue(":hash", hash);
    if (!query.exec()) return false;
    const int adminId = query.lastInsertId().toInt();

    QSqlQuery cq(db);
    cq.prepare("INSERT INTO COMPTEBANCAIRE (iban, solde, type, statut, idClient) VALUES (?,?,?,?,?)");
    cq.addBindValue("BV-0001"); cq.addBindValue(150000.0); cq.addBindValue(static_cast<int>(TypeCompte::COURANT));
    cq.addBindValue(static_cast<int>(StatutCompte::ACTIF)); cq.addBindValue(adminId);
    if (!cq.exec()) return false;
    cq.addBindValue("BV-0002"); cq.addBindValue(80000.0); cq.addBindValue(static_cast<int>(TypeCompte::EPARGNE));
    cq.addBindValue(static_cast<int>(StatutCompte::ACTIF)); cq.addBindValue(adminId);
    if (!cq.exec()) return false;

    const char* prenoms[] = {"Mamadou","Fatou","Ousmane","Aissatou","Modou","Khady","Ibrahima","Mariama",
        "Cheikh","Aminata","Papa","Ndeye","Alioune","MameDiarra","Babacar","Awa","ElHadji","Sokhna",
        "Moussa","Adama","Assane","Rama","Birane","Kine","Oumar","Magatte","Mame","Djibril","Fatimata",
        "Souleymane","Anta","MamadouLamine","Ndeye","Malick","Rokhaya","Youssou","Therese","Massamba",
        "Seynabou","Abdoulaye"};
    const char* noms[] = {"Diop","Ndiaye","Fall","Sow","Mbaye","Diallo","Ba","Sy","Diagne","Cisse",
        "Sow","Gueye","Sarr","Faye","Niang","Ba","Ndiaye","Diop","Fall","Diallo","Diouf","Thiam",
        "Ndiaye","Ndiaye","Sy","Sene","Diop","Ndiaye","Diallo","Ba","Ndiaye","Fall","Diagne","Sall",
        "Gueye","Ndour","Faye","Ndiaye","Diop","Wade"};

    int clientIds[40];
    for (int i = 0; i < 40; ++i) {
        query.prepare("INSERT INTO CLIENT (nom, prenom, email, telephone, login, motDePasseHash, nbTentatives, dateVerrouillage) VALUES (?,?,?,?,?,?,0,'')");
        query.addBindValue(noms[i]); query.addBindValue(prenoms[i]);
        query.addBindValue(QString("%1.%2@bankvision.sn").arg(prenoms[i]).arg(noms[i]));
        query.addBindValue("+22177" + QString::number(1000000 + i));
        query.addBindValue(QString("%1.%2").arg(prenoms[i]).arg(noms[i])); query.addBindValue(hash);
        if (!query.exec()) return false;
        clientIds[i] = query.lastInsertId().toInt();
    }

    struct Acc { int ci; TypeCompte tp; double si; };
    Acc accs[50];
    int ai = 0;
    for (int i = 0; i < 10; ++i) {
        accs[ai++] = {i, TypeCompte::COURANT, 100000.0 + rng->bounded(200) * 1000.0};
        accs[ai++] = {i, TypeCompte::EPARGNE, 50000.0 + rng->bounded(100) * 1000.0};
    }
    for (int i = 10; i < 20; ++i) accs[ai++] = {i, TypeCompte::COURANT, 75000.0 + rng->bounded(150) * 1000.0};
    for (int i = 20; i < 30; ++i) accs[ai++] = {i, TypeCompte::EPARGNE, 30000.0 + rng->bounded(80) * 1000.0};
    for (int i = 30; i < 40; ++i) accs[ai++] = {i, TypeCompte::PROFESSIONNEL, 200000.0 + rng->bounded(300) * 1000.0};

    for (int a = 0; a < 50; ++a) {
        QString iban = QString("SN-BV-2026-%1").arg(a + 1, 4, 10, QChar('0'));
        cq.prepare("INSERT INTO COMPTEBANCAIRE (iban, solde, type, statut, idClient) VALUES (?,?,?,?,?)");
        cq.addBindValue(iban); cq.addBindValue(accs[a].si);
        cq.addBindValue(static_cast<int>(accs[a].tp));
        cq.addBindValue(static_cast<int>(StatutCompte::ACTIF));
        cq.addBindValue(clientIds[accs[a].ci]);
        if (!cq.exec()) return false;

        double solde = accs[a].si;
        auto tx = [&](int y, int m, int d, const QString& type, double delta, const QString& desc) {
            solde += delta;
            QSqlQuery tq(db);
            tq.prepare("INSERT INTO TRANSACTIONS (iban, date, type, montant, soldeApres, description) VALUES (?,?,?,?,?,?)");
            tq.addBindValue(iban);
            tq.addBindValue(QDateTime(QDate(y, m, d), QTime(8 + rng->bounded(10), rng->bounded(60))).toString(Qt::ISODate));
            tq.addBindValue(type); tq.addBindValue(qAbs(delta)); tq.addBindValue(solde); tq.addBindValue(desc);
            if (!tq.exec()) qDebug() << "Tx error:" << tq.lastError().text();
        };
        auto dep = [&](int y, int m, int d, double mt, const QString& desc) { tx(y, m, d, "depot", mt, desc); };
        auto ret = [&](int y, int m, int d, double mt, const QString& desc) { tx(y, m, d, "retrait", -mt, desc); };

        tx(2026, 1, 2, "depot_initial", accs[a].si, "Depot initial");

        if (accs[a].tp == TypeCompte::COURANT) {
            for (int m = 1; m <= 6; ++m) {
                dep(2026, m, 25 + rng->bounded(4), 250000.0 + rng->bounded(50) * 1000.0, "Salaire mensuel");
                ret(2026, m, 1 + rng->bounded(5), 75000.0 + rng->bounded(10) * 1000.0, "Loyer");
                ret(2026, m, 8 + rng->bounded(5), 15000.0 + rng->bounded(15) * 1000.0, "Courses alimentaires");
                ret(2026, m, 12 + rng->bounded(5), 5000.0 + rng->bounded(10) * 1000.0, "Transport");
                ret(2026, m, 18 + rng->bounded(5), 10000.0 + rng->bounded(20) * 1000.0, "Factures/EDF");
                if (m % 2 == 1)
                    ret(2026, m, 20 + rng->bounded(5), 25000.0 + rng->bounded(15) * 1000.0, "Shopping/Divers");
                if (m % 2 == 0 && a % 20 < 10)
                    tx(2026, m, 28, "virement", -50000.0, "Virement vers epargne");
            }
        } else if (accs[a].tp == TypeCompte::EPARGNE) {
            for (int m = 1; m <= 6; ++m) {
                dep(2026, m, 5 + rng->bounded(5), 25000.0 + rng->bounded(10) * 1000.0, "Epargne mensuelle");
                if (m % 3 == 0)
                    ret(2026, m, 20 + rng->bounded(5), 30000.0 + rng->bounded(20) * 1000.0, "Retrait epargne");
            }
        } else {
            for (int m = 1; m <= 6; ++m) {
                dep(2026, m, 5 + rng->bounded(3), 500000.0 + rng->bounded(100) * 1000.0, "Prestation client");
                dep(2026, m, 15 + rng->bounded(3), 300000.0 + rng->bounded(80) * 1000.0, "Paiement facture");
                ret(2026, m, 10 + rng->bounded(3), 150000.0 + rng->bounded(50) * 1000.0, "Fournisseurs");
                ret(2026, m, 22 + rng->bounded(3), 100000.0 + rng->bounded(30) * 1000.0, "Charges sociales");
            }
        }

        if (rng->bounded(10) < 1 && a > 0) {
            QString dest = QString("SN-BV-2026-%1").arg(rng->bounded(a) + 1, 4, 10, QChar('0'));
            double mt = 25000.0 + rng->bounded(15) * 1000.0;
            if (solde > mt)
                tx(2026, 3 + rng->bounded(4), 15 + rng->bounded(10), "virement", -mt, QString("Virement vers %1").arg(dest));
        }

        bool negatif = (a == 5 || a == 12 || a == 18 || a == 25 || a == 33 || a == 39 || a == 44 || a == 48);
        if (negatif && solde > 0)
            ret(2026, 6, 28, solde + 5000.0, "Decouvert technique");

        QSqlQuery uq(db);
        uq.prepare("UPDATE COMPTEBANCAIRE SET solde = ? WHERE iban = ?");
        uq.addBindValue(solde); uq.addBindValue(iban);
        uq.exec();
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