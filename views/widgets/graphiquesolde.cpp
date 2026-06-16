#include "GraphiqueSolde.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <algorithm>
#include <qwidget.h>
void GraphiqueSolde::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    int W = width(), H = height();
    const int MARGE = 40;
    // Fond
    QLinearGradient bg(0, 0, 0, H);
    bg.setColorAt(0.0, QColor(18, 36, 74));
    bg.setColorAt(1.0, QColor(10, 20, 40));
    p.fillRect(rect(), bg);
    if (valeurs.size() < 2) return;
    double maxV = *std::max_element(valeurs.begin(), valeurs.end());
    double minV = *std::min_element(valeurs.begin(), valeurs.end());
    if (maxV == minV) maxV = minV + 1;
    auto xPt = [&](int i) {
        return MARGE + (double)(i) * (W - 2*MARGE) / (valeurs.size()-1);
    };
    auto yPt = [&](double v) {
        return MARGE + (1.0 - (v - minV)/(maxV - minV)) * (H - 2*MARGE);
    };
    // Zone remplie sous la courbe
    QPainterPath aire;
    aire.moveTo(xPt(0), H - MARGE);
    for (int i = 0; i < valeurs.size(); ++i)
        aire.lineTo(xPt(i), yPt(valeurs[i]));
    aire.lineTo(xPt(valeurs.size()-1), H - MARGE);
    aire.closeSubpath();
    QLinearGradient fill(0, MARGE, 0, H - MARGE);
    fill.setColorAt(0.0, QColor(46, 109, 180, 120));
    fill.setColorAt(1.0, QColor(46, 109, 180, 0));
    p.fillPath(aire, fill);
    // Courbe
    QPainterPath courbe;
    courbe.moveTo(xPt(0), yPt(valeurs[0]));
    for (int i = 1; i < valeurs.size(); ++i)
        courbe.lineTo(xPt(i), yPt(valeurs[i]));
    p.setPen(QPen(QColor(100, 180, 255), 2.5));
    p.drawPath(courbe);
    // Points
    p.setBrush(QColor(100, 180, 255));
    for (int i = 0; i < valeurs.size(); ++i)
        p.drawEllipse(QPointF(xPt(i), yPt(valeurs[i])), 4, 4);
}