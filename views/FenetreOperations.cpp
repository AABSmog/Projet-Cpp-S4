#include "FenetreOperations.h"

#include "../controllers/comptecontroller.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

FenetreOperations::FenetreOperations(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("BankVision - Operations");

    auto* layout = new QVBoxLayout(this);

    auto* titre = new QLabel("Operations bancaires");
    titre->setAlignment(Qt::AlignCenter);

    cmbOperation = new QComboBox;
    cmbOperation->addItems({"Depot", "Retrait", "Virement"});

    txtIbanSource = new QLineEdit;
    txtIbanSource->setPlaceholderText("IBAN source");

    txtIbanDestination = new QLineEdit;
    txtIbanDestination->setPlaceholderText("IBAN destination");

    txtMontant = new QLineEdit;
    txtMontant->setPlaceholderText("Montant");

    txtDescription = new QLineEdit;
    txtDescription->setPlaceholderText("Description");

    btnExecuter = new QPushButton("Executer");

    auto* form = new QFormLayout;
    form->addRow("Operation", cmbOperation);
    form->addRow("IBAN source", txtIbanSource);
    form->addRow("IBAN destination", txtIbanDestination);
    form->addRow("Montant", txtMontant);
    form->addRow("Description", txtDescription);

    layout->addWidget(titre);
    layout->addLayout(form);
    layout->addWidget(btnExecuter);

    connect(btnExecuter, &QPushButton::clicked, this, [this]() {
        executerOperation();
    });
}

void FenetreOperations::executerOperation()
{
    bool ok = false;
    const double montant = txtMontant->text().toDouble(&ok);
    if (!ok || montant <= 0.0) {
        QMessageBox::warning(this, "Operation", "Montant invalide.");
        return;
    }

    CompteController controller;
    const QString operation = cmbOperation->currentText();
    const QString description = txtDescription->text().trimmed();

    bool succes = false;
    if (operation == "Depot") {
        succes = controller.faire_depot(txtIbanSource->text().trimmed(), montant, description);
    } else if (operation == "Retrait") {
        succes = controller.retir_montant(txtIbanSource->text().trimmed(), montant, description);
    } else {
        succes = controller.faireVirement(
            txtIbanSource->text().trimmed(),
            txtIbanDestination->text().trimmed(),
            montant,
            description);
    }

    if (succes) {
        QMessageBox::information(this, "Operation", "Operation reussie.");
        return;
    }

    QMessageBox::critical(this, "Operation", "Operation echouee.");
}