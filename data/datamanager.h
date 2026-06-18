#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class DataManager
{
public:
    DataManager();
    bool connectDatabase();
    CompteBancaire* selectCompte(const QString& iban);
private:
    QSqlDatabase db;
};

#endif // DATAMANAGER_H
