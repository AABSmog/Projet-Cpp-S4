#pragma once

#include <QWidget>

class CamembertDepenses : public QWidget
{
    Q_OBJECT

public:
    explicit CamembertDepenses(QWidget *parent = nullptr);
    void setDonnees(double revenus, double depenses);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double revenus;
    double depenses;
};