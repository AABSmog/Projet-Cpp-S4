#include "fenetretableaubord.h"
#include "FenetreOperations.h"
#include "fenetrestatistiques.h"
#include "FenetreStatsIndividuelles.h"
#include "fenetreconnexion.h"
#include "widgets/indicateursolde.h"
#include "../controllers/authcontroller.h"
#include "../controllers/comptecontroller.h"
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
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
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

static QString typeCompteString(TypeCompte type)
{
    switch (type) {
        case TypeCompte::COURANT: return "Courant";
        case TypeCompte::EPARGNE: return "Epargne";
        case TypeCompte::PROFESSIONNEL: return "Professionnel";
    }
    return "Inconnu";
}

static bool estAdmin()
{
    const Client* client = DataManager::instance().clientConnecte();
    return client != nullptr && client->getLogin() == "admin";
}

static QVector<CompteBancaire> comptesPourClient(const QVector<CompteBancaire>& comptes)
{
    const Client* client = DataManager::instance().clientConnecte();
    if (!client) return comptes;
    int id = client->getIdClient();
    if (id == 0) return comptes;
    QVector<CompteBancaire> filtres;
    for (const auto& c : comptes) {
        if (c.getClientId() == id) filtres.append(c);
    }
    return filtres.isEmpty() ? comptes : filtres;
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
    tableHistorique->setRowCount(0);

    if (comptes.isEmpty()) return;

    int row = 0;
    for (const CompteBancaire& compte : comptes) {
        const QVector<Transaction> historique = compte.getHistorique(10);
        tableHistorique->setRowCount(tableHistorique->rowCount() + historique.size());
        for (const Transaction& t : historique) {
            tableHistorique->setItem(row, 0, new QTableWidgetItem(t.getDate().toString("dd/MM/yyyy HH:mm")));
            tableHistorique->setItem(row, 1, new QTableWidgetItem(compte.getIBAN()));
            tableHistorique->setItem(row, 2, new QTableWidgetItem(t.getType()));
            tableHistorique->setItem(row, 3, new QTableWidgetItem(QString::number(t.getMontant(), 'f', 0)));
            tableHistorique->setItem(row, 4, new QTableWidgetItem(QString::number(t.getSoldeApres(), 'f', 0)));
            ++row;
        }
    }
}

FenetreTableauBord::FenetreTableauBord(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("BankVision - Tableau de bord");

    auto* principal = new QVBoxLayout(this);

    auto* topBar = new QHBoxLayout;
    auto* titre = new QLabel("Tableau de Bord");

    cmbSelecteurCompte = new QComboBox;
    cmbSelecteurCompte->setMinimumWidth(200);
    cmbSelecteurCompte->setVisible(false);
    connect(cmbSelecteurCompte, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        mettreAJourCartes();
        QVector<CompteBancaire> hComptes = comptesPourClient(Banque::getComptes());
        remplirHistorique(tableHistorique, hComptes);
    });

    btnExportCSV = new QPushButton("Export CSV");
    btnExportCSV->setStyleSheet(
        "QPushButton { background-color: #27AE60; color: white; border: none; "
        "border-radius: 6px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1E8449; }");
    connect(btnExportCSV, &QPushButton::clicked, this, [this]() { exporterCSV(); });

    btnDeconnexion = new QPushButton("Deconnexion");
    btnDeconnexion->setStyleSheet(
        "QPushButton { background-color: #C0392B; color: white; border: none; "
        "border-radius: 6px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #922B21; }");
    topBar->addWidget(titre);
    topBar->addStretch();
    topBar->addWidget(cmbSelecteurCompte);
    topBar->addWidget(btnExportCSV);
    topBar->addWidget(btnDeconnexion);

    connect(btnDeconnexion, &QPushButton::clicked, this, &FenetreTableauBord::deconnecter);

    auto* cartes = new QHBoxLayout;

    carteSoldeTotal = new IndicateurSolde("Solde total", "0 FCFA");
    carteNombreComptes = new IndicateurSolde("Comptes", "0");
    carteNombrePrets = new IndicateurSolde("Prets", "0");
    carteSoldeMoyen = new IndicateurSolde("Solde moyen", "0 FCFA");

    cartes->addWidget(carteSoldeTotal);
    cartes->addWidget(carteNombreComptes);
    cartes->addWidget(carteNombrePrets);
    cartes->addWidget(carteSoldeMoyen);

    mettreAJourCartes();

    btnExportCSV->setVisible(estAdmin());

    if (!estAdmin()) {
        QVector<CompteBancaire> mesComptes = comptesPourClient(Banque::getComptes());
        cmbSelecteurCompte->clear();
        for (const CompteBancaire& c : mesComptes) {
            cmbSelecteurCompte->addItem(
                c.getIBAN() + " - " + typeCompteString(c.getType()),
                c.getIBAN());
        }
        if (cmbSelecteurCompte->count() > 1)
            cmbSelecteurCompte->setVisible(true);
    }

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

    {
        QVector<CompteBancaire> hComptes = estAdmin() ? Banque::getComptes() : comptesPourClient(Banque::getComptes());
        remplirHistorique(tableHistorique, hComptes);
    }

    fenetreOperations = new FenetreOperations;
    connect(fenetreOperations, &FenetreOperations::operationEffectuee, this, &FenetreTableauBord::rafraichirVue);

    fenetreStatistiques = new FenetreStatistiques;
    fenetreStatsIndividuelles = new FenetreStatsIndividuelles;

    onglets = new QTabWidget;
    onglets->addTab(fenetreOperations, "Operations");
    if (estAdmin()) {
        onglets->addTab(fenetreStatistiques, "Statistiques globales");
    }
    onglets->addTab(fenetreStatsIndividuelles, "Mon Compte");
    onglets->addTab(tableHistorique, "Historique");
    onglets->addTab(blocCreationCompte, "Creer compte");

    principal->addLayout(topBar);
    principal->addLayout(cartes);
    principal->addWidget(onglets);
}

void FenetreTableauBord::rafraichirVue()
{
    if (!estAdmin()) {
        QString courant = cmbSelecteurCompte->currentData().toString();
        cmbSelecteurCompte->blockSignals(true);
        cmbSelecteurCompte->clear();
        QVector<CompteBancaire> mesComptes = comptesPourClient(Banque::getComptes());
        int idx = 0;
        for (int i = 0; i < mesComptes.size(); ++i) {
            cmbSelecteurCompte->addItem(
                mesComptes[i].getIBAN() + " - " + typeCompteString(mesComptes[i].getType()),
                mesComptes[i].getIBAN());
            if (mesComptes[i].getIBAN() == courant) idx = i;
        }
        cmbSelecteurCompte->setCurrentIndex(idx);
        cmbSelecteurCompte->setVisible(cmbSelecteurCompte->count() > 1);
        cmbSelecteurCompte->blockSignals(false);
    }
    mettreAJourCartes();
    QVector<CompteBancaire> hComptes = estAdmin() ? Banque::getComptes() : comptesPourClient(Banque::getComptes());
    remplirHistorique(tableHistorique, hComptes);
}

void FenetreTableauBord::mettreAJourCartes()
{
    const QVector<CompteBancaire> comptes = Banque::getComptes();

    if (estAdmin()) {
        const VueDashboardStats stats = calculerStats(comptes);
        carteSoldeTotal->setTitre("Solde total");
        carteSoldeTotal->setValeur(formatFcfa(stats.soldeTotal));
        carteNombreComptes->setTitre("Comptes");
        carteNombreComptes->setValeur(QString::number(stats.nombreComptes));
        carteNombrePrets->setTitre("Prets");
        carteNombrePrets->setValeur(QString::number(stats.nombrePrets));
        carteSoldeMoyen->setTitre("Solde moyen");
        carteSoldeMoyen->setValeur(formatFcfa(stats.soldeMoyen));
    } else {
        const CompteBancaire* compteClient = nullptr;
        QString selectedIban = cmbSelecteurCompte->currentData().toString();
        if (!selectedIban.isEmpty()) {
            for (const CompteBancaire& c : comptes) {
                if (c.getIBAN() == selectedIban) {
                    compteClient = &c;
                    break;
                }
            }
        }
        if (!compteClient) {
            for (const CompteBancaire& c : comptes) {
                if (c.getClientId() == DataManager::instance().clientConnecte()->getIdClient()) {
                    compteClient = &c;
                    break;
                }
            }
        }
        if (compteClient) {
            carteSoldeTotal->setTitre("IBAN");
            carteSoldeTotal->setValeur(compteClient->getIBAN());
            carteNombreComptes->setTitre("Type");
            carteNombreComptes->setValeur(typeCompteString(compteClient->getType()));
            carteNombrePrets->setTitre("Solde");
            carteNombrePrets->setValeur(formatFcfa(compteClient->getSolde()));
            carteSoldeMoyen->setTitre("Solde moyen 30j");
            carteSoldeMoyen->setValeur(formatFcfa(compteClient->getSoldeMoyen()));
        }
    }
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

    CompteController::rechargerComptes();

    rafraichirVue();
    fenetreStatistiques->actualiser();
    fenetreStatsIndividuelles->actualiser();

    QMessageBox::information(this, "Creation", "Compte cree et enregistre dans la base de donnees.");
}

void FenetreTableauBord::exporterCSV()
{
    const QString chemin = QFileDialog::getSaveFileName(
        this, "Exporter les donnees", "bankvision_export.csv", "Fichiers CSV (*.csv)");
    if (chemin.isEmpty()) return;

    QFile fichier(chemin);
    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Export CSV", "Impossible d'ecrire le fichier.");
        return;
    }

    QTextStream out(&fichier);
    out.setEncoding(QStringConverter::Utf8);
    out << "IBAN;Type;Solde;Statut;Date;TypeTransaction;Montant;SoldeApres;Description\n";

    const QVector<CompteBancaire> comptes = Banque::getComptes();
    for (const CompteBancaire& cb : comptes) {
        const QVector<Transaction> txns = DataManager::instance().chargerTransactions(cb.getIBAN());
        if (txns.isEmpty()) {
            out << cb.getIBAN() << ";" << typeCompteString(cb.getType()) << ";"
                << QString::number(cb.getSolde(), 'f', 0) << ";ACTIF;;;;;\n";
        } else {
            for (const Transaction& t : txns) {
                out << cb.getIBAN() << ";" << typeCompteString(cb.getType()) << ";"
                    << QString::number(cb.getSolde(), 'f', 0) << ";ACTIF;"
                    << t.getDate().toString("dd/MM/yyyy") << ";"
                    << t.getType() << ";"
                    << QString::number(t.getMontant(), 'f', 0) << ";"
                    << QString::number(t.getSoldeApres(), 'f', 0) << ";"
                    << t.getDescription() << "\n";
            }
        }
    }

    fichier.close();
    QMessageBox::information(this, "Export CSV",
        QString("Donnees exportees dans %1 (%2 comptes).")
            .arg(chemin).arg(comptes.size()));
}

void FenetreTableauBord::deconnecter()
{
    Banque::viderComptes();
    auto* connexion = new FenetreConnexion;
    connexion->setAttribute(Qt::WA_DeleteOnClose);
    connexion->show();
    close();
}