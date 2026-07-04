#include "graphiquesolde.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <algorithm>

GraphiqueSolde::GraphiqueSolde(QWidget *parent)
    : QWidget(parent)
{
}

void GraphiqueSolde::setDonnees(const QVector<double>& nouvellesValeurs)
{
    valeurs = nouvellesValeurs;
    update();
}

void GraphiqueSolde::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0.0, QColor(18, 36, 74));
    bg.setColorAt(1.0, QColor(10, 20, 40));
    p.fillRect(rect(), bg);

    if (valeurs.size() < 2) {
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, "Pas assez de donnees");
        return;
    }

    const double minV = *std::min_element(valeurs.begin(), valeurs.end());
    const double maxV = *std::max_element(valeurs.begin(), valeurs.end());
    const double range = (maxV - minV) == 0.0 ? 1.0 : (maxV - minV);
    const int margin = 24;

    QPainterPath chemin;
    auto pointY = [&](double valeur) {
        return margin + (1.0 - (valeur - minV) / range) * (height() - 2 * margin);
    };
    auto pointX = [&](int index) {
        return margin + index * (width() - 2 * margin) / double(valeurs.size() - 1);
    };

    chemin.moveTo(pointX(0), pointY(valeurs[0]));
    for (int i = 1; i < valeurs.size(); ++i) {
        chemin.lineTo(pointX(i), pointY(valeurs[i]));
    }

    p.setPen(QPen(QColor(100, 180, 255), 2.5));
    p.drawPath(chemin);

    p.setBrush(QColor(100, 180, 255));
    for (int i = 0; i < valeurs.size(); ++i) {
        p.drawEllipse(QPointF(pointX(i), pointY(valeurs[i])), 4, 4);
    }

    p.setPen(Qt::white);
    p.drawText(10, 18, QString("Min: %1").arg(minV, 0, 'f', 0));
    p.drawText(width() - 100, 18, QString("Max: %1").arg(maxV, 0, 'f', 0));
}