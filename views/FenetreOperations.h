#pragma once

#include <QWidget>

class QComboBox;
class QLineEdit;
class QPushButton;
class QLabel;

class FenetreOperations : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreOperations(QWidget* parent = nullptr);

signals:
    void operationEffectuee();

private:
    void executerOperation();
    void changerOperation(int index);

    QComboBox* cmbOperation;
    QLineEdit* txtIbanSource;
    QLineEdit* txtIbanDestination;
    QLineEdit* txtMontant;
    QLineEdit* txtDescription;
    QPushButton* btnExecuter;
    QLabel* lblIbanDestination;
};
