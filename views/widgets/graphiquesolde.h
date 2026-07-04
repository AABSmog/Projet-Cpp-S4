#pragma once

#include <QWidget>
#include <QVector>

class GraphiqueSolde : public QWidget
{
    Q_OBJECT

public:
    explicit GraphiqueSolde(QWidget *parent = nullptr);
    void setDonnees(const QVector<double>& valeurs);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> valeurs;
};