#pragma once

#include <QWidget>
#include "../controllers/authcontroller.h"

class QLabel;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QPushButton;

class FenetreConnexion : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreConnexion(QWidget *parent = nullptr);

private:
    void mettreAJourEssais();
    void mettreAJourCompte();
    void creerClientEtCompte();

    QLineEdit* txtIdentifiant;
    QLineEdit* txtPassword;

    QPushButton* btnConnexion;
    QPushButton* btnCreation;

    QLabel* lblEssais;
    int essaisRestants;
    AuthController authController;

    QLineEdit* txtNom;
    QLineEdit* txtPrenom;
    QLineEdit* txtEmail;
    QLineEdit* txtTelephone;
    QLineEdit* txtLoginInscription;
    QLineEdit* txtPasswordInscription;
    QLineEdit* txtIban;
    QComboBox* cmbTypeCompte;
    QLineEdit* txtSoldeInitial;
};

