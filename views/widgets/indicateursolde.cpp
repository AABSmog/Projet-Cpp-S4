#include "indicateursolde.h"

#include <QVBoxLayout>
#include <QLabel>

IndicateurSolde::IndicateurSolde(
    const QString& titre,
    const QString& valeur,
    QWidget *parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);

    titreLabel = new QLabel(titre);
    valeurLabel = new QLabel(valeur);

    layout->addWidget(titreLabel);
    layout->addWidget(valeurLabel);

    setStyleSheet(
        "background:#1b3a7a;"
        "border-radius:10px;"
        "padding:10px;"
        );
}

void IndicateurSolde::setValeur(const QString& valeur)
{
    valeurLabel->setText(valeur);
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
    if (valeur.startsWith('-')) {
        valeurLabel->setStyleSheet("color: #E74C3C; font-weight: bold; font-size: 18px;");
    } else {
        valeurLabel->setStyleSheet("color: #2ECC71; font-weight: bold; font-size: 18px;");
    }
<<<<<<< HEAD
=======
=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
}

void IndicateurSolde::setTitre(const QString& titre)
{
    titreLabel->setText(titre);
}