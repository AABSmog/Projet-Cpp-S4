#include "fenetreconnexion.h"
#include "fenetretableaubord.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>
#include <QDoubleValidator>

#include "../data/datamanager.h"
#include "../models/banque.h"
#include "../models/comptebancaire.h"

FenetreConnexion::FenetreConnexion(QWidget *parent)
    : QWidget(parent)
    , essaisRestants(3)
{
    setWindowTitle("BankVision - Connexion");
    setMinimumWidth(360);

    auto* mainLayout = new QVBoxLayout(this);

    auto* titre = new QLabel("BANKVISION");
    titre->setAlignment(Qt::AlignCenter);

    auto* onglets = new QTabWidget;

    auto* loginTab = new QWidget;
    auto* loginLayout = new QVBoxLayout(loginTab);

    txtIdentifiant = new QLineEdit;
    txtIdentifiant->setPlaceholderText("Identifiant");

    txtPassword = new QLineEdit;
    txtPassword->setPlaceholderText("Mot de passe");
    txtPassword->setEchoMode(QLineEdit::Password);

    btnConnexion = new QPushButton("Se connecter");

    lblEssais = new QLabel("3 essais restants");
    lblEssais->setAlignment(Qt::AlignCenter);

    loginLayout->addWidget(txtIdentifiant);
    loginLayout->addWidget(txtPassword);
    loginLayout->addWidget(btnConnexion);
    loginLayout->addWidget(lblEssais);
    loginLayout->addStretch();

    auto* inscription = new QGroupBox("Creation de client et compte");
    auto* inscriptionLayout = new QFormLayout(inscription);

    txtNom = new QLineEdit;
    txtNom->setPlaceholderText("Nom");
    txtNom->setText("Aminata");

    txtPrenom = new QLineEdit;
    txtPrenom->setPlaceholderText("Prenom");
    txtPrenom->setText("Diallo");

    txtEmail = new QLineEdit;
    txtEmail->setPlaceholderText("Email");
    txtEmail->setText("aminata.diallo@example.com");

    txtTelephone = new QLineEdit;
    txtTelephone->setPlaceholderText("Telephone");
    txtTelephone->setText("+221771234567");

    txtLoginInscription = new QLineEdit;
    txtLoginInscription->setPlaceholderText("Login");
    txtLoginInscription->setText("aminata.diallo");

    txtPasswordInscription = new QLineEdit;
    txtPasswordInscription->setPlaceholderText("Mot de passe");
    txtPasswordInscription->setEchoMode(QLineEdit::Password);
    txtPasswordInscription->setText("Aminata@2026");

    txtIban = new QLineEdit;
    txtIban->setPlaceholderText("IBAN");
    txtIban->setText("SN-BV-2026-0003");

    cmbTypeCompte = new QComboBox;
    cmbTypeCompte->addItems({"Courant", "Epargne", "Professionnel"});

    txtSoldeInitial = new QLineEdit;
    txtSoldeInitial->setPlaceholderText("Solde initial");
    txtSoldeInitial->setText("250000");
    txtSoldeInitial->setValidator(new QDoubleValidator(0, 1e12, 2, txtSoldeInitial));

    inscriptionLayout->addRow("Nom", txtNom);
    inscriptionLayout->addRow("Prenom", txtPrenom);
    inscriptionLayout->addRow("Email", txtEmail);
    inscriptionLayout->addRow("Telephone", txtTelephone);
    inscriptionLayout->addRow("Login", txtLoginInscription);
    inscriptionLayout->addRow("Mot de passe", txtPasswordInscription);
    inscriptionLayout->addRow("IBAN", txtIban);
    inscriptionLayout->addRow("Type compte", cmbTypeCompte);
    inscriptionLayout->addRow("Solde initial", txtSoldeInitial);

    btnCreation = new QPushButton("Creer le client et le compte");

    connect(btnConnexion, &QPushButton::clicked, this, [this]() {
        const QString identifiant = txtIdentifiant->text().trimmed();
        const QString motDePasse = txtPassword->text();

        if (authController.verifierConnexion(identifiant, motDePasse)) {
            auto* tableauDeBord = new FenetreTableauBord;
            tableauDeBord->setAttribute(Qt::WA_DeleteOnClose);
            tableauDeBord->show();
            close();
            return;
        }

        essaisRestants = qMax(0, essaisRestants - 1);
        mettreAJourEssais();

        QMessageBox::warning(this,
                             "Authentification",
                             "Identifiant ou mot de passe incorrect.");

        if (essaisRestants == 0) {
            btnConnexion->setEnabled(false);
            QMessageBox::critical(this,
                                  "Authentification",
                                  "Trop d'essais. Connexion temporairement bloquee.");
        }
    });

    connect(btnCreation, &QPushButton::clicked, this, [this]() {
        creerClientEtCompte();
    });

    auto* inscriptionPage = new QWidget;
    auto* inscriptionLayoutVertical = new QVBoxLayout(inscriptionPage);
    inscriptionLayoutVertical->addWidget(inscription);
    inscriptionLayoutVertical->addWidget(btnCreation);
    inscriptionLayoutVertical->addStretch();

    onglets->addTab(loginTab, "Connexion");
    onglets->addTab(inscriptionPage, "Inscription");

    mainLayout->addStretch();
    mainLayout->addWidget(titre);
    mainLayout->addWidget(onglets);
    mainLayout->addStretch();
}

void FenetreConnexion::mettreAJourEssais()
{
    lblEssais->setText(QString("%1 essai%2 restants")
                           .arg(essaisRestants)
                           .arg(essaisRestants > 1 ? "s" : ""));
}

void FenetreConnexion::creerClientEtCompte()
{
    bool ok = false;
    const double soldeInitial = txtSoldeInitial->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Creation", "Solde initial invalide.");
        return;
    }

    AuthController localAuth;
    QString erreur;
    const bool succes = DataManager::instance().creerClientEtCompte(
        txtNom->text(),
        txtPrenom->text(),
        txtEmail->text(),
        txtTelephone->text(),
        txtLoginInscription->text(),
        localAuth.hashPassword(txtPasswordInscription->text()),
        txtIban->text(),
        static_cast<TypeCompte>(cmbTypeCompte->currentIndex()),
        soldeInitial,
        &erreur);

    if (!succes) {
        QMessageBox::critical(this, "Creation", erreur.isEmpty() ? "Creation impossible." : erreur);
        return;
    }

    Banque::viderComptes();
    const QVector<CompteBancaire> comptes = DataManager::instance().chargerComptes();
    for (const CompteBancaire& compte : comptes) {
        Banque::ajouterCompte(compte);
    }

    QMessageBox::information(this,
                             "Creation",
                             "Client et compte crees et enregistres dans la base de donnees.");
}
