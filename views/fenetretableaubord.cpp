#include "fenetretableaubord.h"
#include "FenetreOperations.h"
#include "fenetrestatistiques.h"
#include "FenetreStatsIndividuelles.h"
#include "fenetreconnexion.h"
#include "widgets/indicateursolde.h"
#include "../controllers/comptecontroller.h"
#include "../controllers/statcontroller.h"
#include "../controllers/authcontroller.h"
<<<<<<< HEAD
=======
#include "../controllers/comptecontroller.h"
#include "../data/datamanager.h"
#include "../models/banque.h"
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
#include "../models/transaction.h"
#include "../models/comptebancaire.h"
#include "../models/client.h"

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
<<<<<<< HEAD
=======
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
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154

static QString formatFcfa(double valeur)
{
    return QString::number(valeur, 'f', 0) + " FCFA";
}

<<<<<<< HEAD
static void remplirHistorique(QTableWidget* table, const QVector<CompteBancaire>& comptes)
=======
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
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
{
    table->clearContents();
    table->setRowCount(0);
    if (comptes.isEmpty()) return;
    int row = 0;
    for (const CompteBancaire& compte : comptes) {
<<<<<<< HEAD
        QVector<Transaction> histo = CompteController::getHistorique(compte, 10);
        table->setRowCount(table->rowCount() + histo.size());
        for (const Transaction& t : histo) {
            table->setItem(row, 0, new QTableWidgetItem(CompteController::getDateStr(t)));
            table->setItem(row, 1, new QTableWidgetItem(CompteController::getIban(compte)));
            table->setItem(row, 2, new QTableWidgetItem(CompteController::getTransactionType(t)));
            table->setItem(row, 3, new QTableWidgetItem(QString::number(CompteController::getMontant(t), 'f', 0)));
            table->setItem(row, 4, new QTableWidgetItem(QString::number(CompteController::getSoldeApres(t), 'f', 0)));
=======
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
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
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
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154

    cmbSelecteurCompte = new QComboBox;
    cmbSelecteurCompte->setMinimumWidth(200);
    cmbSelecteurCompte->setVisible(false);
    connect(cmbSelecteurCompte, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        mettreAJourCartes();
<<<<<<< HEAD
        QVector<CompteBancaire> hComptes = CompteController::estAdmin()
            ? CompteController::getComptes()
            : CompteController::getComptesPourClient(CompteController::getClientConnecteId());
=======
        QVector<CompteBancaire> hComptes = comptesPourClient(Banque::getComptes());
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
        remplirHistorique(tableHistorique, hComptes);
    });

    btnExportCSV = new QPushButton("Export CSV");
    btnExportCSV->setStyleSheet(
        "QPushButton { background-color: #27AE60; color: white; border: none; "
        "border-radius: 6px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1E8449; }");
    connect(btnExportCSV, &QPushButton::clicked, this, [this]() { exporterCSV(); });

<<<<<<< HEAD
=======
=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
    btnDeconnexion = new QPushButton("Deconnexion");
    btnDeconnexion->setStyleSheet(
        "QPushButton { background-color: #C0392B; color: white; border: none; "
        "border-radius: 6px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #922B21; }");
    topBar->addWidget(titre);
    topBar->addStretch();
<<<<<<< HEAD
    topBar->addWidget(cmbSelecteurCompte);
    topBar->addWidget(btnExportCSV);
=======
<<<<<<< HEAD
    topBar->addWidget(cmbSelecteurCompte);
    topBar->addWidget(btnExportCSV);
=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
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

<<<<<<< HEAD
    btnExportCSV->setVisible(CompteController::estAdmin());

    if (!CompteController::estAdmin()) {
        QVector<CompteBancaire> mesComptes = CompteController::getComptesPourClient(
            CompteController::getClientConnecteId());
        cmbSelecteurCompte->clear();
        for (const CompteBancaire& c : mesComptes) {
            cmbSelecteurCompte->addItem(
                CompteController::getIban(c) + " - " + CompteController::getTypeString(c),
                CompteController::getIban(c));
=======
<<<<<<< HEAD
    btnExportCSV->setVisible(estAdmin());

    if (!estAdmin()) {
        QVector<CompteBancaire> mesComptes = comptesPourClient(Banque::getComptes());
        cmbSelecteurCompte->clear();
        for (const CompteBancaire& c : mesComptes) {
            cmbSelecteurCompte->addItem(
                c.getIBAN() + " - " + typeCompteString(c.getType()),
                c.getIBAN());
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
        }
        if (cmbSelecteurCompte->count() > 1)
            cmbSelecteurCompte->setVisible(true);
    }

<<<<<<< HEAD
=======
=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
    auto* blocCreation = new QGroupBox("Creation de compte pour le client connecte");
    auto* form = new QFormLayout(blocCreation);

    txtNom = new QLineEdit;
    txtNom->setText(AuthController::getNomClient().isEmpty() ? "Mariama" : AuthController::getNomClient());
    txtNom->setReadOnly(true);
    txtPrenom = new QLineEdit;
    txtPrenom->setText(AuthController::getPrenomClient().isEmpty() ? "Ndiaye" : AuthController::getPrenomClient());
    txtPrenom->setReadOnly(true);
    txtEmail = new QLineEdit;
    txtEmail->setText(AuthController::getEmailClient().isEmpty() ? "mariama.ndiaye@example.com" : AuthController::getEmailClient());
    txtEmail->setReadOnly(true);
    txtTelephone = new QLineEdit;
    txtTelephone->setText(AuthController::getTelephoneClient().isEmpty() ? "+221781112233" : AuthController::getTelephoneClient());
    txtTelephone->setReadOnly(true);
    txtLogin = new QLineEdit;
    txtLogin->setText(AuthController::getLoginClient().isEmpty() ? "mariama.ndiaye" : AuthController::getLoginClient());
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
<<<<<<< HEAD
        QVector<CompteBancaire> hComptes = CompteController::estAdmin()
            ? CompteController::getComptes()
            : CompteController::getComptesPourClient(CompteController::getClientConnecteId());
=======
        QVector<CompteBancaire> hComptes = estAdmin() ? Banque::getComptes() : comptesPourClient(Banque::getComptes());
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
        remplirHistorique(tableHistorique, hComptes);
    }

    fenetreOperations = new FenetreOperations;
    connect(fenetreOperations, &FenetreOperations::operationEffectuee, this, &FenetreTableauBord::rafraichirVue);

    fenetreStatistiques = new FenetreStatistiques;
    fenetreStatsIndividuelles = new FenetreStatsIndividuelles;

    onglets = new QTabWidget;
    onglets->addTab(fenetreOperations, "Operations");
<<<<<<< HEAD
    if (CompteController::estAdmin())
        onglets->addTab(fenetreStatistiques, "Statistiques globales");
=======
    if (estAdmin()) {
        onglets->addTab(fenetreStatistiques, "Statistiques globales");
    }
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
    onglets->addTab(fenetreStatsIndividuelles, "Mon Compte");
    onglets->addTab(tableHistorique, "Historique");
    onglets->addTab(blocCreationCompte, "Creer compte");

    principal->addLayout(topBar);
    principal->addLayout(cartes);
    principal->addWidget(onglets);
}

void FenetreTableauBord::rafraichirVue()
{
<<<<<<< HEAD
    if (!CompteController::estAdmin()) {
        QString courant = cmbSelecteurCompte->currentData().toString();
        cmbSelecteurCompte->blockSignals(true);
        cmbSelecteurCompte->clear();
        QVector<CompteBancaire> mesComptes = CompteController::getComptesPourClient(
            CompteController::getClientConnecteId());
        int idx = 0;
        for (int i = 0; i < mesComptes.size(); ++i) {
            cmbSelecteurCompte->addItem(
                CompteController::getIban(mesComptes[i]) + " - " + CompteController::getTypeString(mesComptes[i]),
                CompteController::getIban(mesComptes[i]));
            if (CompteController::getIban(mesComptes[i]) == courant) idx = i;
=======
<<<<<<< HEAD
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
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
        }
        cmbSelecteurCompte->setCurrentIndex(idx);
        cmbSelecteurCompte->setVisible(cmbSelecteurCompte->count() > 1);
        cmbSelecteurCompte->blockSignals(false);
    }
<<<<<<< HEAD
    mettreAJourCartes();
    QVector<CompteBancaire> hComptes = CompteController::estAdmin()
        ? CompteController::getComptes()
        : CompteController::getComptesPourClient(CompteController::getClientConnecteId());
    remplirHistorique(tableHistorique, hComptes);
}

void FenetreTableauBord::mettreAJourCartes()
{
    QVector<CompteBancaire> comptes = CompteController::getComptes();

    if (CompteController::estAdmin()) {
        double total = StatController::calculerSoldeTotal(comptes);
        double moyenne = StatController::calculerSoldeMoyen(comptes);
        carteSoldeTotal->setTitre("Solde total");
        carteSoldeTotal->setValeur(formatFcfa(total));
        carteNombreComptes->setTitre("Comptes");
        carteNombreComptes->setValeur(QString::number(comptes.size()));
        carteNombrePrets->setTitre("Prets");
        carteNombrePrets->setValeur("0");
        carteSoldeMoyen->setTitre("Solde moyen");
        carteSoldeMoyen->setValeur(formatFcfa(moyenne));
    } else {
=======
=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
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
<<<<<<< HEAD
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
        const CompteBancaire* compteClient = nullptr;
        QString selectedIban = cmbSelecteurCompte->currentData().toString();
        if (!selectedIban.isEmpty()) {
            for (const CompteBancaire& c : comptes) {
<<<<<<< HEAD
                if (CompteController::getIban(c) == selectedIban) {
=======
                if (c.getIBAN() == selectedIban) {
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
                    compteClient = &c;
                    break;
                }
            }
        }
        if (!compteClient) {
<<<<<<< HEAD
            int clientId = CompteController::getClientConnecteId();
            for (const CompteBancaire& c : comptes) {
                if (CompteController::getClientId(c) == clientId) {
                    compteClient = &c;
                    break;
                }
=======
            for (const CompteBancaire& c : comptes) {
                if (c.getClientId() == DataManager::instance().clientConnecte()->getIdClient()) {
                    compteClient = &c;
                    break;
                }
=======
        const Client* client = DataManager::instance().clientConnecte();
        int clientId = client ? client->getIdClient() : 0;
        const CompteBancaire* compteClient = nullptr;
        for (const CompteBancaire& c : comptes) {
            if (c.getClientId() == clientId) {
                compteClient = &c;
                break;
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
            }
        }
        if (compteClient) {
            carteSoldeTotal->setTitre("IBAN");
<<<<<<< HEAD
            carteSoldeTotal->setValeur(CompteController::getIban(*compteClient));
            carteNombreComptes->setTitre("Type");
            carteNombreComptes->setValeur(CompteController::getTypeString(*compteClient));
            carteNombrePrets->setTitre("Solde");
            carteNombrePrets->setValeur(formatFcfa(CompteController::getSolde(*compteClient)));
            carteSoldeMoyen->setTitre("Solde moyen 30j");
            carteSoldeMoyen->setValeur(formatFcfa(CompteController::getSoldeMoyen(*compteClient)));
=======
            carteSoldeTotal->setValeur(compteClient->getIBAN());
            carteNombreComptes->setTitre("Type");
            carteNombreComptes->setValeur(typeCompteString(compteClient->getType()));
            carteNombrePrets->setTitre("Solde");
            carteNombrePrets->setValeur(formatFcfa(compteClient->getSolde()));
            carteSoldeMoyen->setTitre("Solde moyen 30j");
            carteSoldeMoyen->setValeur(formatFcfa(compteClient->getSoldeMoyen()));
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
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

    CompteController controller;
    QString erreur;
    const bool succes = controller.creerComptePourClient(
        txtIban->text(), cmbTypeCompte->currentIndex(), soldeInitial, &erreur);

    if (!succes) {
        QMessageBox::critical(this, "Creation", erreur.isEmpty() ? "Creation impossible." : erreur);
        return;
    }

    CompteController::rechargerComptes();
<<<<<<< HEAD
    rafraichirVue();
    fenetreStatistiques->actualiser();
    fenetreStatsIndividuelles->actualiser();
    QMessageBox::information(this, "Creation", "Compte cree et enregistre dans la base de donnees.");
}

=======

    rafraichirVue();
    fenetreStatistiques->actualiser();
    fenetreStatsIndividuelles->actualiser();

    QMessageBox::information(this, "Creation", "Compte cree et enregistre dans la base de donnees.");
}

<<<<<<< HEAD
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
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

<<<<<<< HEAD
    const QVector<CompteBancaire> comptes = CompteController::getComptes();
    for (const CompteBancaire& cb : comptes) {
        const QVector<Transaction> txns = CompteController::getTransactions(CompteController::getIban(cb));
        if (txns.isEmpty()) {
            out << CompteController::getIban(cb) << ";"
                << CompteController::getTypeString(cb) << ";"
                << QString::number(CompteController::getSolde(cb), 'f', 0) << ";ACTIF;;;;;\n";
        } else {
            for (const Transaction& t : txns) {
                out << CompteController::getIban(cb) << ";"
                    << CompteController::getTypeString(cb) << ";"
                    << QString::number(CompteController::getSolde(cb), 'f', 0) << ";ACTIF;"
                    << CompteController::getDateStr(t) << ";"
                    << CompteController::getTransactionType(t) << ";"
                    << QString::number(CompteController::getMontant(t), 'f', 0) << ";"
                    << QString::number(CompteController::getSoldeApres(t), 'f', 0) << ";"
                    << CompteController::getDescription(t) << "\n";
=======
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
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
            }
        }
    }

    fichier.close();
    QMessageBox::information(this, "Export CSV",
<<<<<<< HEAD
        QString("Donnees exportees dans %1 (%2 comptes).").arg(chemin).arg(comptes.size()));
}

void FenetreTableauBord::deconnecter()
{
=======
        QString("Donnees exportees dans %1 (%2 comptes).")
            .arg(chemin).arg(comptes.size()));
}

=======
>>>>>>> f820f35defca2484d81e938fe98d949fd62a22f8
void FenetreTableauBord::deconnecter()
{
    Banque::viderComptes();
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
    auto* connexion = new FenetreConnexion;
    connexion->setAttribute(Qt::WA_DeleteOnClose);
    connexion->show();
    close();
<<<<<<< HEAD
}
=======
}
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
