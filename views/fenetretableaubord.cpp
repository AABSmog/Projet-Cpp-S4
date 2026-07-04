#include "fenetretableaubord.h"
#include "FenetreOperations.h"
#include "fenetrestatistiques.h"
#include "widgets/indicateursolde.h"
#include "../controllers/authcontroller.h"
#include "../data/datamanager.h"
#include "../models/banque.h"
#include "../models/transaction.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDoubleValidator>
#include <algorithm>

#include "../models/client.h"

static QVector<Transaction> trierTransactionsChronologiquement(QVector<Transaction> transactions)
{
    std::sort(transactions.begin(), transactions.end(), [](const Transaction& left, const Transaction& right) {
        if (left.getDate() == right.getDate()) {
            return left.getId() < right.getId();
        }

        return left.getDate() < right.getDate();
    });

    return transactions;
}

struct VueDashboardStats
{
    double soldeTotal = 0.0;
    double soldeMoyen = 0.0;
    int nombreComptes = 0;
    int nombrePrets = 0;
};

static QString formatFcfa(double valeur)
{
    return QString::number(valeur, 'f', 0) + " FCFA";
}

static VueDashboardStats calculerStats(const QVector<CompteBancaire>& comptes)
{
    VueDashboardStats stats;
    stats.nombreComptes = comptes.size();

    int nombreSoldes = 0;
    for (const CompteBancaire& compte : comptes) {
        stats.soldeTotal += compte.getSolde();
        stats.soldeMoyen += compte.getSoldeMoyen();
        ++nombreSoldes;
    }

    if (nombreSoldes > 0) {
        stats.soldeMoyen /= nombreSoldes;
    }

    return stats;
}

static void remplirHistorique(QTableWidget* tableHistorique, const QVector<CompteBancaire>& comptes)
{
    tableHistorique->clearContents();

    if (comptes.isEmpty()) {
        tableHistorique->setRowCount(0);
        return;
    }

    const QVector<Transaction> historique = comptes.first().getHistorique(10);
    tableHistorique->setRowCount(historique.size());

    for (int i = 0; i < historique.size(); ++i) {
        const Transaction& transaction = historique.at(i);
        tableHistorique->setItem(i, 0, new QTableWidgetItem(transaction.getDate().toString("dd/MM/yyyy HH:mm")));
        tableHistorique->setItem(i, 1, new QTableWidgetItem(transaction.getType()));
        tableHistorique->setItem(i, 2, new QTableWidgetItem(QString::number(transaction.getMontant(), 'f', 0)));
        tableHistorique->setItem(i, 3, new QTableWidgetItem(QString::number(transaction.getSoldeApres(), 'f', 0)));
    }
}

FenetreTableauBord::FenetreTableauBord(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("BankVision - Tableau de bord");

    auto* principal = new QVBoxLayout(this);

    auto* titre = new QLabel("Tableau de Bord");
    titre->setAlignment(Qt::AlignCenter);

    auto* cartes = new QHBoxLayout;

    const QVector<CompteBancaire> comptes = Banque::getComptes();
    const VueDashboardStats stats = calculerStats(comptes);

    carteSoldeTotal = new IndicateurSolde("Solde total", formatFcfa(stats.soldeTotal));
    carteNombreComptes = new IndicateurSolde("Comptes", QString::number(stats.nombreComptes));
    carteNombrePrets = new IndicateurSolde("Prets", QString::number(stats.nombrePrets));
    carteSoldeMoyen = new IndicateurSolde("Solde moyen", formatFcfa(stats.soldeMoyen));

    cartes->addWidget(carteSoldeTotal);
    cartes->addWidget(carteNombreComptes);
    cartes->addWidget(carteNombrePrets);
    cartes->addWidget(carteSoldeMoyen);

    auto* blocCreation = new QGroupBox("Creation de compte pour le client connecte");
    auto* form = new QFormLayout(blocCreation);

    const Client* client = DataManager::instance().clientConnecte();

    txtNom = new QLineEdit;
    txtNom->setText(client != nullptr ? client->getNom() : "Mariama");
    txtNom->setReadOnly(true);
    txtPrenom = new QLineEdit;
    txtPrenom->setText(client != nullptr ? client->getPrenom() : "Ndiaye");
    txtPrenom->setReadOnly(true);
    txtEmail = new QLineEdit;
    txtEmail->setText(client != nullptr ? client->getEmail() : "mariama.ndiaye@example.com");
    txtEmail->setReadOnly(true);
    txtTelephone = new QLineEdit;
    txtTelephone->setText(client != nullptr ? client->getTelephone() : "+221781112233");
    txtTelephone->setReadOnly(true);
    txtLogin = new QLineEdit;
    txtLogin->setText(client != nullptr ? client->getLogin() : "mariama.ndiaye");
    txtLogin->setReadOnly(true);
    txtMotDePasse = new QLineEdit;
    txtMotDePasse->setEchoMode(QLineEdit::Password);
    txtMotDePasse->setPlaceholderText("Mot de passe du nouveau compte");
    txtIban = new QLineEdit;
    txtIban->setText("SN-BV-2026-0004");
    cmbTypeCompte = new QComboBox;
    cmbTypeCompte->addItems({"Courant", "Epargne", "Professionnel"});
    txtSoldeInitial = new QLineEdit;
    txtSoldeInitial->setText("180000");
    txtSoldeInitial->setValidator(new QDoubleValidator(0, 1e12, 2, txtSoldeInitial));
    btnCreationCompte = new QPushButton("Creer le compte");

    form->addRow("Nom", txtNom);
    form->addRow("Prenom", txtPrenom);
    form->addRow("Email", txtEmail);
    form->addRow("Telephone", txtTelephone);
    form->addRow("Login", txtLogin);
    form->addRow("Mot de passe", txtMotDePasse);
    form->addRow("IBAN", txtIban);
    form->addRow("Type", cmbTypeCompte);
    form->addRow("Solde initial", txtSoldeInitial);
    form->addRow(btnCreationCompte);

    connect(btnCreationCompte, &QPushButton::clicked, this, [this]() {
        creerCompteDepuisLeDashboard();
    });

    blocCreationCompte = blocCreation;

    tableHistorique = new QTableWidget;
    tableHistorique->setColumnCount(5);
    tableHistorique->setHorizontalHeaderLabels({"Date", "IBAN", "Type", "Montant", "Solde après"});
    tableHistorique->horizontalHeader()->setStretchLastSection(true);
    tableHistorique->setAlternatingRowColors(true);

    remplirHistorique(tableHistorique, comptes);

    fenetreOperations = new FenetreOperations;
    fenetreStatistiques = new FenetreStatistiques;

    onglets = new QTabWidget;
    onglets->addTab(fenetreOperations, "Operations");
    onglets->addTab(fenetreStatistiques, "Statistiques");
    onglets->addTab(tableHistorique, "Historique");
    onglets->addTab(blocCreationCompte, "Creer compte");

    principal->addWidget(titre);
    principal->addLayout(cartes);
    principal->addWidget(onglets);
}

void FenetreTableauBord::rafraichirVue()
{
    const QVector<CompteBancaire> comptes = Banque::getComptes();
    const VueDashboardStats stats = calculerStats(comptes);

    carteSoldeTotal->setValeur(formatFcfa(stats.soldeTotal));
    carteNombreComptes->setValeur(QString::number(stats.nombreComptes));
    carteNombrePrets->setValeur(QString::number(stats.nombrePrets));
    carteSoldeMoyen->setValeur(formatFcfa(stats.soldeMoyen));

    remplirHistorique(tableHistorique, comptes);
}

void FenetreTableauBord::creerCompteDepuisLeDashboard()
{
    bool ok = false;
    const double soldeInitial = txtSoldeInitial->text().toDouble(&ok);
    if (!ok || soldeInitial < 0.0) {
        QMessageBox::warning(this, "Creation", "Solde initial invalide.");
        return;
    }

    QString erreur;
    const bool succes = DataManager::instance().creerComptePourClient(
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
        CompteBancaire copie = compte;
        copie.chargerHistorique(DataManager::instance().chargerTransactions(copie.getIBAN()));
        Banque::ajouterCompte(copie);
    }

    rafraichirVue();
    fenetreStatistiques->actualiser();

    QMessageBox::information(this, "Creation", "Compte cree et enregistre dans la base de donnees.");
}