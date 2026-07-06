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
    void exporterCSV();

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
    QPushButton* btnExportCSV;
    QComboBox* cmbSelecteurCompte;
};