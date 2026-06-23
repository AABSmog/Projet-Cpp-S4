#pragma once

#include <QWidget>

class GraphiqueSolde : public QWidget
{
    Q_OBJECT

public:
    explicit GraphiqueSolde(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};