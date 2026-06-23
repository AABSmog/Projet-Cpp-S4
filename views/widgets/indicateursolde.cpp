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

    layout->addWidget(new QLabel(titre));
    layout->addWidget(new QLabel(valeur));

    setStyleSheet(
        "background:#1b3a7a;"
        "border-radius:10px;"
        "padding:10px;"
        );
}