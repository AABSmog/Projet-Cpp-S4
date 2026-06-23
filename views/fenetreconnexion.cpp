#include "fenetreconnexion.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

FenetreConnexion::FenetreConnexion(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("BankVision - Connexion");

    auto* mainLayout = new QVBoxLayout(this);

    auto* titre = new QLabel("BANKVISION");
    titre->setAlignment(Qt::AlignCenter);

    txtIdentifiant = new QLineEdit;
    txtIdentifiant->setPlaceholderText("Identifiant");

    txtPassword = new QLineEdit;
    txtPassword->setPlaceholderText("Mot de passe");
    txtPassword->setEchoMode(QLineEdit::Password);

    btnConnexion = new QPushButton("Se connecter");

    lblEssais = new QLabel("3 essais restants");
    lblEssais->setAlignment(Qt::AlignCenter);

    mainLayout->addStretch();
    mainLayout->addWidget(titre);
    mainLayout->addWidget(txtIdentifiant);
    mainLayout->addWidget(txtPassword);
    mainLayout->addWidget(btnConnexion);
    mainLayout->addWidget(lblEssais);
    mainLayout->addStretch();
}
