#include "camembertdepenses.h"

#include <QPainter>
#include <QtMath>

CamembertDepenses::CamembertDepenses(QWidget *parent)
    : QWidget(parent)
    , revenus(1.0)
    , depenses(1.0)
{
    setMinimumSize(300,300);
}

void CamembertDepenses::setDonnees(double nouveauxRevenus, double nouvellesDepenses)
{
    revenus = qMax(0.0, nouveauxRevenus);
    depenses = qMax(0.0, nouvellesDepenses);
    update();
}

void CamembertDepenses::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing, true);

    const double total = revenus + depenses;
    if (total <= 0.0) {
        p.drawText(rect(), Qt::AlignCenter, "Aucune donnee");
        return;
    }

    QRect zone(40,40,200,200);

    const int revenusSpan = int((revenus / total) * 360.0 * 16.0);
    const int depensesSpan = int((depenses / total) * 360.0 * 16.0);

    p.setBrush(QColor(46, 109, 180));
    p.drawPie(zone, 0, revenusSpan);

    p.setBrush(QColor(192, 57, 43));
    p.drawPie(zone, revenusSpan, depensesSpan);

    p.drawText(270,100,QString("Revenus: %1").arg(revenus, 0, 'f', 0));
    p.drawText(270,130,QString("Depenses: %1").arg(depenses, 0, 'f', 0));
    p.drawText(270,160,QString("Solde net: %1").arg(revenus - depenses, 0, 'f', 0));
}