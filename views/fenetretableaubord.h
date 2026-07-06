#pragma once

#include <QWidget>

class QTableWidget;
class QTabWidget;
class FenetreOperations;
class FenetreStatistiques;
class FenetreStatsIndividuelles;
class QLineEdit;
class QPushButton;
class QComboBox;
class QGroupBox;
class QLabel;
class IndicateurSolde;

class FenetreTableauBord : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreTableauBord(QWidget *parent = nullptr);

private:
    void rafraichirVue();
    void creerCompteDepuisLeDashboard();
    void mettreAJourCartes();
    void deconnecter();
<<<<<<< HEAD
    void exporterCSV();
=======
<<<<<<< HEAD
    void exporterCSV();
=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154

    IndicateurSolde* carteSoldeTotal;
    IndicateurSolde* carteNombreComptes;
    IndicateurSolde* carteNombrePrets;
    IndicateurSolde* carteSoldeMoyen;
    QTableWidget* tableHistorique;
    QTabWidget* onglets;
    QGroupBox* blocCreationCompte;
    FenetreOperations* fenetreOperations;
    FenetreStatistiques* fenetreStatistiques;
    FenetreStatsIndividuelles* fenetreStatsIndividuelles;

    QLineEdit* txtNom;
    QLineEdit* txtPrenom;
    QLineEdit* txtEmail;
    QLineEdit* txtTelephone;
    QLineEdit* txtLogin;
    QLineEdit* txtMotDePasse;
    QLineEdit* txtIban;
    QComboBox* cmbTypeCompte;
    QLineEdit* txtSoldeInitial;
    QPushButton* btnCreationCompte;
    QPushButton* btnDeconnexion;
<<<<<<< HEAD
    QPushButton* btnExportCSV;
    QComboBox* cmbSelecteurCompte;
=======
<<<<<<< HEAD
    QPushButton* btnExportCSV;
    QComboBox* cmbSelecteurCompte;
=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
};