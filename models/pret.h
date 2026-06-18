#ifndef PRET_H
#define PRET_H

#include <QString>

class Pret
{
private:
    int idPret;
    double montant;
    double tauxInteret;
    int duree;
    double mensualite;

    QString dateDemande;

public:
    Pret();
    Pret(int idPret, double montant,
         int duree, double mensualite,
         QString dateDemande,double tauxInteret=5.0);

    // Getters
    int getIdPret();
    double getMontant();
    double getTauxInteret();
    int getDuree();
    double getMensualite();

    QString getDateDemande();

    // Setters
    void setIdPret(int idPret);
    void setMontant(double montant);
    void setTauxInteret(double tauxInteret);
    void setDuree(int duree);
    void setMensualite(double mensualite);

    void setDateDemande(QString dateDemande);

    // Méthodes métier
    double calculerMensualite();
    bool ajouter();
    bool supprimer(int idPret);
    bool modifier(int idPret);
};

#endif // PRET_H