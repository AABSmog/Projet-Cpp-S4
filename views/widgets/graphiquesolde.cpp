#include "graphiquesolde.h"
#include <QPainter>

GraphiqueSolde::GraphiqueSolde(QWidget *parent)
    : QWidget(parent)
{
}

void GraphiqueSolde::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.drawRect(rect().adjusted(10,10,-10,-10));

    p.drawText(
        rect(),
        Qt::AlignCenter,
        "Graphique du solde");
}