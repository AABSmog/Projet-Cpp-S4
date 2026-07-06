#pragma once

#include <QWidget>

class QComboBox;
class QLineEdit;
class QPushButton;

class FenetreOperations : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreOperations(QWidget* parent = nullptr);

signals:
    void operationEffectuee();

private:
    void executerOperation();

    QComboBox* cmbOperation;
    QLineEdit* txtIbanSource;
    QLineEdit* txtIbanDestination;
    QLineEdit* txtMontant;
    QLineEdit* txtDescription;
    QPushButton* btnExecuter;
};