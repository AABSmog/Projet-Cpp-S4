#include "camembertdepenses.h"

#include <QPainter>

CamembertDepenses::CamembertDepenses(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(300,300);
}

void CamembertDepenses::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QRect zone(50,50,200,200);

    // Alimentation 40%
    p.setBrush(Qt::blue);
    p.drawPie(zone,0,40*16*3.6);

    // Transport 25%
    p.setBrush(Qt::green);
    p.drawPie(zone,40*16*3.6,25*16*3.6);

    // Santé 15%
    p.setBrush(Qt::yellow);
    p.drawPie(zone,65*16*3.6,15*16*3.6);

    // Loisirs 20%
    p.setBrush(Qt::red);
    p.drawPie(zone,80*16*3.6,20*16*3.6);

    p.drawText(270,100,"Alimentation");
    p.drawText(270,130,"Transport");
    p.drawText(270,160,"Sante");
    p.drawText(270,190,"Loisirs");
}