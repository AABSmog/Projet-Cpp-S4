#pragma once

#include <QWidget>

class QTableWidget;

class FenetreTableauBord : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreTableauBord(QWidget *parent = nullptr);

private:
    QTableWidget* tableHistorique;
};