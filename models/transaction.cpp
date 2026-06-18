#pragma once
#include <QString>
#include <QVector>
#include <QDateTime>

struct Transaction {
    int id;
    QDateTime date;
    QString type; // "depot", "retrait", "virement"
    double montant;
    double soldeApres;
    QString description;
};