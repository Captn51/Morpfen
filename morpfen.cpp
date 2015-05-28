#include "morpfen.h"

Morpfen::Morpfen() : QWidget()
{
    // Accueil
    QMessageBox::warning(this, "Morpfen : Accueil", "Bienvenue dans le jeu du MORPFEN !!");

    // Entree des noms des joueurs (+ securite)
    bool ok = false;
    do
    {
        myJ1 = QInputDialog::getText(this,
                                     "Morpfen : Joueurs",
                                     "Joueur 1, entre ton nom :",
                                     QLineEdit::Normal,
                                     QString(),
                                     &ok);
    } while(!ok || myJ1.isEmpty());
    ok = false;
    do
    {
        myJ2 = QInputDialog::getText(this,
                                     "Morpfen : Joueurs",
                                     "Joueur 2, entre ton nom (et un autre nom que Joueur 1 !) :",
                                     QLineEdit::Normal,
                                     QString(),
                                     &ok);
    } while(!ok || myJ2.isEmpty() || (myJ2 == myJ1));

    // Affichage des noms pour verification
    QMessageBox::information(this, "Morpfen : Joueurs", "Les joueurs sont " + myJ1 + " et " + myJ2 + ".");

    // Creation des boutons et des labels
    myLabelPlayerInfo = new QLabel(this);
    myLabelPlayerInfo->setText(myJ1 + ", vas-y !!");
    myPBQuitter = new QPushButton("Quitter", this);   // Bouton pour quitter

    // Les cases du morpion
    for(int i = 0; i < 9; i++)
        myLEBoxes[i] = new QLineEdit(QString(""), this);

    // Creation du layout
    myLayout = new QGridLayout(this);

    // Insertion des boutons ds le layout
    myLayout->addWidget(myLabelPlayerInfo, 0, 0, 1, 3);
    for(int i = 0, i1 = 1; i < 9; i += 3, i1++)
    {
        for(int j = 0; j < 3; j++)
        {
            myLayout->addWidget(myLEBoxes[i+j], i1, j);
        }
    }
    myLayout->addWidget(myPBQuitter, 4, 0, 1, 3);

    // Insertion du layout ds la fenetre
    setLayout(myLayout);

    // Connexion du signal quitter
    connect(myPBQuitter, &QPushButton::clicked, this, &Morpfen::leave);

    // Connexion des autres signaux
    for(int i = 0; i < 9; i++)
    {
        connect(myLEBoxes[i], &QLineEdit::returnPressed, [this, i](){this->myLEBoxes[i]->setEnabled(false);});
        connect(myLEBoxes[i], &QLineEdit::returnPressed, this, &Morpfen::analyze);
    }
}

void Morpfen::leave()
{
    // Veut-on vraiment quitter ?
    if(QMessageBox::critical(this,
                             "Morpfen : Quitter",
                             "Voulez-vous vraiment quitter ?",
                             QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        QGuiApplication::quit();
}

void Morpfen::analyze()
{
    // S'il y a alignement
    if(alignment())
    {
        disableBoxes();  // On desactive les cases quand quelqu'un gagne
        QStringList list = myLabelPlayerInfo->text().split(","); // Nom du joueur qui gagne

        // Blabla + demande si on veut rejouer
        if(QMessageBox::warning(this,
                                "Morpfen : Victoire",
                                list[0]+" gagne la partie !! C'est un boss ;)\n\nVoulez-vous rejouer ? :)",
                                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            reset();
    }
    else    // Le morpion est-il plein ?
    {
        if(full()) // Personne ne gagne
        {
            if(QMessageBox::warning(this,
                                    "Morpfen : Match nul",
                                    "Personne ne gagne la partie :0 Bande de blaireaux -_-\n\nVoulez-vous rejouer ? :)",
                                    QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
                reset();
        }
        else    // On doit encore jouer => on change de joueur
        {
            QStringList list = myLabelPlayerInfo->text().split(",");
            if(list[0] == myJ1)
                myLabelPlayerInfo->setText(myJ2 + ", vas-y !!");
            else
                myLabelPlayerInfo->setText(myJ1 + ", vas-y !!");
        }
    }
}

bool Morpfen::equal(int i, int j, int k) const
{
    // Les textes doivent etre non vides et egaux
    return (!myLEBoxes[i]->text().isEmpty() &&
            !myLEBoxes[j]->text().isEmpty() &&
            !myLEBoxes[k]->text().isEmpty() &&
            myLEBoxes[i]->text() == myLEBoxes[j]->text() &&
            myLEBoxes[j]->text() == myLEBoxes[k]->text());
}

bool Morpfen::alignment() const
{
    return (equal(0, 1, 2) ||  // 1ere ligne
            equal(3, 4, 5) ||  // 2e ligne
            equal(6, 7, 8) ||  // 3e ligne
            equal(0, 3, 6) ||  // 1ere colonne
            equal(1, 4, 7) ||  // 2e colonne
            equal(2, 5, 8) ||  // 3e colonne
            equal(6, 4, 2) ||  // Diagonale y=x
            equal(0, 4, 8));   // Diagonale y=-x
}

bool Morpfen::full() const
{
    // Renvoie false si certaines cases ne sont pas remplies...
    for(QLineEdit* box : myLEBoxes)
    {
        if(box->text().isEmpty())
        {
            return false;
        }
    }

    // ...et true sinon
    return true;
}

void Morpfen::disableBoxes()
{
    // On rend inaccessibles toutes les cases
    for(QLineEdit* box : myLEBoxes)
        box->setEnabled(false);
}

void Morpfen::reset()
{
    for(QLineEdit* box : myLEBoxes)
        box->clear();          // On vide les cases

    for(QLineEdit* box : myLEBoxes)
        box->setEnabled(true); // On rend accessibles toutes les cases
}

