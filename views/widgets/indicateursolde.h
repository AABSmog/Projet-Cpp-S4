#pragma once

#include <QWidget>

class QLabel;

class IndicateurSolde : public QWidget
{
    Q_OBJECT

public:
    explicit IndicateurSolde(
        const QString& titre,
        const QString& valeur,
        QWidget *parent = nullptr);
};


