#pragma once

#include <QWidget>

class CamembertDepenses : public QWidget
{
    Q_OBJECT

public:
    explicit CamembertDepenses(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};