#include "pret.h"

Pret::Pret()
{
    idPret = 0;
    montant = 0;
    tauxInteret = 0;
    duree = 0;
    mensualite = 0;

    dateDemande = "";
}

Pret::Pret(int idPret, double montant,
           int duree, double mensualite,
            QString dateDemande,double tauxInteret)
{
    this->idPret = idPret;
    this->montant = montant;
    this->tauxInteret = tauxInteret;
    this->duree = duree;
    this->mensualite = mensualite;

    this->dateDemande = dateDemande;
}

// Getters
int Pret::getIdPret() { return idPret; }
double Pret::getMontant() { return montant; }
double Pret::getTauxInteret() { return tauxInteret; }
int Pret::getDuree() { return duree; }
double Pret::getMensualite() { return mensualite; }

QString Pret::getDateDemande() { return dateDemande; }

// Setters
void Pret::setIdPret(int idPret) { this->idPret = idPret; }
void Pret::setMontant(double montant) { this->montant = montant; }
void Pret::setTauxInteret(double tauxInteret) { this->tauxInteret = tauxInteret; }
void Pret::setDuree(int duree) { this->duree = duree; }
void Pret::setMensualite(double mensualite) { this->mensualite = mensualite; }

void Pret::setDateDemande(QString dateDemande) { this->dateDemande = dateDemande; }

double Pret::calculerMensualite()
{
    if(duree == 0)
        return 0;

    double montantTotal = montant + (montant * tauxInteret / 100);
    mensualite = montantTotal / duree;

    return mensualite;
}

bool Pret::ajouter()
{
    return true;
}

bool Pret::supprimer(int idPret)
{
    return true;
}

bool Pret::modifier(int idPret)
{
    return true;
}