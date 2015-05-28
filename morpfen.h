#ifndef MORPFEN_H
#define MORPFEN_H

#include <QtWidgets>

/**
 * Classe implementant la fenetre du morpion. C'est un petit jeu donc pas de
 * separation interface/metier.
 */
class Morpfen : public QWidget
{
    Q_OBJECT

private:
    QLineEdit* myLEBoxes[9];
    QPushButton* myPBQuitter;
    QGridLayout* myLayout;
    QLabel* myLabelPlayerInfo;
    QString myJ1;
    QString myJ2;

private:
    bool alignment() const;                /// Teste s'il y a un alignement dans le morpion.
    bool full() const;                     /// Verifie s'il reste des cases non jouees.
    bool equal(int i, int j, int k) const; /// Teste si les textes des cases d'indices i, j et k sont non vides et egaux.
    void disableBoxes();                   /// Grise toutes les cases.
    void reset();                          /// Vide le morpion et le remet comme au debut.

public:
    Morpfen();
    Morpfen(const Morpfen& m) = delete;
    Morpfen(Morpfen&& m) = delete;
    Morpfen& operator=(Morpfen m) = delete;

public slots:
    void leave();     /// Quitte "agreablement" le morpion.
    void analyze();   /// Action a l'entree d'un caractere dans une case.
};

#endif

