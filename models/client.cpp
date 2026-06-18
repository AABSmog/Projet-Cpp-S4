#pragma once
#include "comptebancaire.h"
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QBytearray>
#include <QCryptographicHash>
class Client{
public :
    Client(){

    };
    // Accesseurs
    QString getnom()const{return nom;}
    QString getprenom()const{return prenom;}
    QString gethash()const{return hashedpwd;}
    QString getemail()const{return email;}
    int getid()const{return id;}
    // Operations
    QString hashpwd(QByteArray input){
        QByteArray rawhash =  QCryptographicHash::hash(input,QCryptographicHash::Sha256);
        QString hexHash = rawhash.toHex();
        return hexHash;
    }
    //
private:
    QString nom, prenom, hashedpwd, email;
    QVector<CompteBancaire> comptes;
    int id;
};
