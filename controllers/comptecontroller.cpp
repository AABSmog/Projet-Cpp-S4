#include "comptecontroller.h"
<<<<<<< HEAD
#include "authcontroller.h"
#include "../data/datamanager.h"
#include "../models/banque.h"
#include "../models/comptebancaire.h"
#include "../models/client.h"
#include "../models/transaction.h"
#include <algorithm>
=======
#include "../data/datamanager.h"
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154

CompteController::CompteController() {}

bool CompteController::faire_depot(const QString& iban, double montant, const QString& description)
{
    CompteBancaire* compte = Banque::chercherCompte(iban);
    if (compte == nullptr) return false;
    return compte->deposer(montant, description);
}

bool CompteController::retir_montant(const QString& iban, double montant, const QString& desc)
{
    CompteBancaire* compte = Banque::chercherCompte(iban);
    if (compte == nullptr) return false;
    return compte->retirer(montant, desc);
}

bool CompteController::faireVirement(const QString& ibanSource, const QString& ibanDestination,
                                     double montant, const QString& description)
{
    CompteBancaire* source = Banque::chercherCompte(ibanSource);
    CompteBancaire* destination = Banque::chercherCompte(ibanDestination);
    if (source == nullptr || destination == nullptr) return false;
    return source->virer(*destination, montant);
}

<<<<<<< HEAD
bool CompteController::creerComptePourClient(const QString& iban, int typeIndex,
                                              double soldeInitial, QString* erreur)
{
    return DataManager::instance().creerComptePourClient(
        iban, static_cast<TypeCompte>(typeIndex), soldeInitial, erreur);
}

=======
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
void CompteController::rechargerComptes()
{
    Banque::viderComptes();
    const QVector<CompteBancaire> comptes = DataManager::instance().chargerComptes();
    for (const CompteBancaire& compte : comptes) {
        CompteBancaire copie = compte;
        copie.chargerHistorique(DataManager::instance().chargerTransactions(copie.getIBAN()));
        Banque::ajouterCompte(copie);
    }
<<<<<<< HEAD
}

QVector<CompteBancaire> CompteController::getComptes() { return Banque::getComptes(); }
CompteBancaire* CompteController::chercherCompte(const QString& iban) { return Banque::chercherCompte(iban); }

QVector<CompteBancaire> CompteController::getComptesPourClient(int clientId)
{
    QVector<CompteBancaire> result;
    for (const CompteBancaire& c : Banque::getComptes()) {
        if (c.getClientId() == clientId) result.append(c);
    }
    return result.isEmpty() ? Banque::getComptes() : result;
}

const Client* CompteController::getClientConnecte() { return DataManager::instance().clientConnecte(); }

bool CompteController::estAdmin()
{
    const Client* client = DataManager::instance().clientConnecte();
    return client != nullptr && client->getLogin() == "admin";
}

int CompteController::getClientConnecteId()
{
    const Client* client = DataManager::instance().clientConnecte();
    return client ? client->getIdClient() : 0;
}

QVector<Transaction> CompteController::getTransactions(const QString& iban)
    { return DataManager::instance().chargerTransactions(iban); }

QVector<Transaction> CompteController::getToutesTransactions()
    { return DataManager::instance().chargerToutesTransactions(); }

bool CompteController::creerClientEtCompte(const QString& nom, const QString& prenom,
                                            const QString& email, const QString& telephone,
                                            const QString& login, const QString& motDePasse,
                                            const QString& iban, int typeIndex,
                                            double soldeInitial, QString* erreur)
{
    AuthController auth;
    return DataManager::instance().creerClientEtCompte(
        nom, prenom, email, telephone, login, auth.hashPassword(motDePasse),
        iban, static_cast<TypeCompte>(typeIndex), soldeInitial, erreur);
}

QString CompteController::typeCompteString(int typeIndex)
{
    switch (static_cast<TypeCompte>(typeIndex)) {
        case TypeCompte::COURANT: return "Courant";
        case TypeCompte::EPARGNE: return "Epargne";
        case TypeCompte::PROFESSIONNEL: return "Professionnel";
    }
    return "Inconnu";
}

QString CompteController::getIban(const CompteBancaire& c) { return c.getIBAN(); }
double CompteController::getSolde(const CompteBancaire& c) { return c.getSolde(); }
QString CompteController::getTypeString(const CompteBancaire& c) { return typeCompteString(static_cast<int>(c.getType())); }
int CompteController::getClientId(const CompteBancaire& c) { return c.getClientId(); }
double CompteController::getSoldeMoyen(const CompteBancaire& c) { return c.getSoldeMoyen(); }
QVector<double> CompteController::getSoldesMensuels(const CompteBancaire& c) { return c.getSoldesMensuels(); }
QVector<Transaction> CompteController::getHistorique(const CompteBancaire& c, int n) { return c.getHistorique(n); }

QString CompteController::getDateStr(const Transaction& t) { return t.getDate().toString("dd/MM/yyyy HH:mm"); }
QString CompteController::getTransactionType(const Transaction& t) { return t.getType(); }
double CompteController::getMontant(const Transaction& t) { return t.getMontant(); }
double CompteController::getSoldeApres(const Transaction& t) { return t.getSoldeApres(); }
QString CompteController::getDescription(const Transaction& t) { return t.getDescription(); }
int CompteController::getTransactionId(const Transaction& t) { return t.getId(); }
bool CompteController::estUnRetrait(const Transaction& t)
    { return t.getType().contains("retrait", Qt::CaseInsensitive); }

QVector<Transaction> CompteController::trierTransactionsParDate(const QVector<Transaction>& txns)
{
    QVector<Transaction> tries = txns;
    std::sort(tries.begin(), tries.end(), [](const Transaction& left, const Transaction& right) {
        if (left.getDate() == right.getDate()) return left.getId() < right.getId();
        return left.getDate() < right.getDate();
    });
    return tries;
}
=======
}
>>>>>>> 9b452efefeece18804b98c3983e51138a350b154
