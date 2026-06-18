#include "datamanager.h"
#include <QSqlError>
#include <QDebug>

QString path = "banque.db";
QSqlQuery query;
DataManager::DataManager()
{
}

bool DataManager::connectDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(path);

    if(db.open())
    {
        qDebug() << "Connexion réussie";
        query.exec(
            "CREATE TABLE IF NOT EXISTS CLIENT ("
            "idClient INTEGER PRIMARY KEY AUTOINCREMENT,"
            "nom TEXT NOT NULL,"
            "prenom TEXT NOT NULL,"
            "email TEXT UNIQUE,"
            "telephone TEXT,"
            "login TEXT UNIQUE NOT NULL,"
            "motDePasseHash TEXT NOT NULL,"
            "nbTentatives INTEGER DEFAULT 0,"
            "dateVerrouillage TEXT)"
            );
        query.exec(
            "CREATE TABLE IF NOT EXISTS BANQUE ("
            "idBanque INTEGER PRIMARY KEY AUTOINCREMENT,"
            "nom TEXT,"
            "adresse TEXT,"
            "telephone TEXT,"
            "email TEXT)"
            );
        query.exec(
            "CREATE TABLE IF NOT EXISTS COMPTEBANCAIRE ("
            "iban TEXT PRIMARY KEY,"
            "solde REAL NOT NULL,"
            "type INTEGER NOT NULL,"
            "statut INTEGER NOT NULL,"
            "idClient INTEGER,"
            "FOREIGN KEY(idClient) REFERENCES CLIENT(idClient))"
            );
        query.exec(
            "CREATE TABLE IF NOT EXISTS TRANSACTION ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "iban TEXT NOT NULL,"
            "date TEXT NOT NULL,"
            "type TEXT NOT NULL,"
            "montant REAL NOT NULL,"
            "soldeApres REAL NOT NULL,"
            "description TEXT,"
            "FOREIGN KEY(iban) REFERENCES COMPTE(iban))"
            );
        query.exec(
            "CREATE TABLE IF NOT EXISTS PRET ("
            "idPret INTEGER PRIMARY KEY AUTOINCREMENT,"
            "iban TEXT,"
            "montant REAL NOT NULL,"
            "tauxInteret REAL NOT NULL,"
            "duree INTEGER NOT NULL,"
            "mensualite REAL NOT NULL,"
            "dateDemande TEXT NOT NULL,"
            "statut TEXT DEFAULT 'En attente',"
            "FOREIGN KEY(iban) REFERENCES COMPTE(iban))"
            );
        return true;
    }

    qDebug() << "Erreur :" << db.lastError().text();
    return false;

}
CompteBancaire* DataManager::selectCompte(const QString& iban)
{
    QSqlQuery query;

    query.prepare(
        "SELECT iban, solde, type, statut "
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

        return new CompteBancaire(
            ibanCompte,
            type,
            solde,
            statut
            );
    }

    return nullptr;
}