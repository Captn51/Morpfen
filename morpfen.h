#ifndef MORPFEN_H
#define MORPFEN_H

#include <QtWidgets>

/**
 * Classe implementant la fenetre du morpion. C'est un petit jeu donc pas de
 * separation interface/metier (c'est pas bien, je sais).
 */
class Morpfen : public QWidget
{
    Q_OBJECT

private:
    QLineEdit* m_case[9];
    QPushButton* m_quitter;
    QGridLayout* m_layout;
    QLabel* m_infojoueur;
    QString m_j1;
    QString m_j2;

    bool test() const;                          // Teste s'il y a un alignement ds le morpion
    bool plein() const;                         // Verifie s'il reste des cases non jouees
    bool egalite(int i, int j, int k) const;    // Teste si les textes des boutons d'indices i, j et k st les memes, et non vides
    void toutGriser();                          // Grise toutes les cases
    void raz();                                 // Vide le morpion et le remet comme au debut

public:
    Morpfen();

public slots:
    void quitter();     // Quitte "agreablement" le morpion
    void analyser();    // Action a l'entree d'un caractere dans une case
};

#endif

