#pragma once

#include <QWidget>

class GraphiqueSolde;
class CamembertDepenses;
class IndicateurSolde;

class FenetreStatistiques : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreStatistiques(QWidget *parent = nullptr);
    void actualiser();
};
