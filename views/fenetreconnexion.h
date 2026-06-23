#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class FenetreConnexion : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreConnexion(QWidget *parent = nullptr);

private:
    QLineEdit* txtIdentifiant;
    QLineEdit* txtPassword;

    QPushButton* btnConnexion;

    QLabel* lblEssais;
};

