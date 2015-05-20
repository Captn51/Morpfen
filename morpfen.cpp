#include "morpfen.h"

Morpfen::Morpfen() : QWidget()
{
    // Accueil
    QMessageBox::warning(this, "Morpfen : Accueil", "Bienvenue dans le jeu du MORPFEN !!");

    // Entree des noms des joueurs (+ securite)
    bool ok = false;
    do
    {
        m_j1 = QInputDialog::getText(this,
                                     "Morpfen : Joueurs",
                                     "Joueur 1, entre ton nom :",
                                     QLineEdit::Normal,
                                     QString(),
                                     &ok);
    } while(!ok || m_j1.isEmpty());
    ok = false;
    do
    {
        m_j2 = QInputDialog::getText(this,
                                     "Morpfen : Joueurs",
                                     "Joueur 2, entre ton nom (et un autre nom que Joueur 1 !) :",
                                     QLineEdit::Normal,
                                     QString(),
                                     &ok);
    } while(!ok || m_j2.isEmpty() || (m_j2 == m_j1));

    // Affichage des noms pour verification
    QMessageBox::information(this, "Morpfen : Joueurs", "Les joueurs sont " + m_j1 + " et " + m_j2 + ".");

    // Creation des boutons et des labels
    m_infojoueur = new QLabel(this);
    m_infojoueur->setText(m_j1 + ", vas-y !!");
    m_quitter = new QPushButton("Quitter", this);   // Bouton pour quitter

    // Les cases du morpion
    for(int i = 0; i < 9; i++)
        m_cases[i] = new QLineEdit(QString(""), this);

    // Creation du layout
    m_layout = new QGridLayout(this);

    // Insertion des boutons ds le layout
    m_layout->addWidget(m_infojoueur, 0, 0, 1, 3);
    for(int i = 0, i1 = 1; i < 9; i += 3, i1++)
    {
        for(int j = 0; j < 3; j++)
        {
            m_layout->addWidget(m_cases[i+j], i1, j);
        }
    }
    m_layout->addWidget(m_quitter, 4, 0, 1, 3);

    // Insertion du layout ds la fenetre
    setLayout(m_layout);

    // Connexion du signal quitter
    connect(m_quitter, &QPushButton::clicked, this, &Morpfen::quitter);

    // Connexion des autres signaux
    for(int i = 0; i < 9; i++)
    {
        connect(m_cases[i], &QLineEdit::returnPressed, [this, i](){this->m_cases[i]->setEnabled(false);});
        connect(m_cases[i], &QLineEdit::returnPressed, this, &Morpfen::analyser);
    }
}

void Morpfen::quitter()
{
    // Veut-on vraiment quitter ?
    if(QMessageBox::critical(this,
                             "Morpfen : Quitter",
                             "Voulez-vous vraiment quitter ?",
                             QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        QGuiApplication::quit();
}

void Morpfen::analyser()
{
    // S'il y a alignement
    if(alignement())
    {
        toutGriser();  // On desactive les cases quand quelqu'un gagne
        QStringList list = m_infojoueur->text().split(","); // Nom du joueur qui gagne

        // Blabla + demande si on veut rejouer
        if(QMessageBox::warning(this,
                                "Morpfen : Victoire",
                                list[0]+" gagne la partie !! C'est un boss ;)\n\nVoulez-vous rejouer ? :)",
                                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            raz();
    }
    else    // Le morpion est-il plein ?
    {
        if(plein()) // Personne ne gagne
        {
            if(QMessageBox::warning(this,
                                    "Morpfen : Match nul",
                                    "Personne ne gagne la partie :0 Bande de blaireaux -_-\n\nVoulez-vous rejouer ? :)",
                                    QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
                raz();
        }
        else    // On doit encore jouer => on change de joueur
        {
            QStringList list = m_infojoueur->text().split(",");
            if(list[0] == m_j1)
                m_infojoueur->setText(m_j2 + ", vas-y !!");
            else
                m_infojoueur->setText(m_j1 + ", vas-y !!");
        }
    }
}

bool Morpfen::egalite(int i, int j, int k) const
{
    // Les textes doivent etre non vides et egaux
    return (!m_cases[i]->text().isEmpty() &&
            !m_cases[j]->text().isEmpty() &&
            !m_cases[k]->text().isEmpty() &&
            m_cases[i]->text() == m_cases[j]->text() &&
            m_cases[j]->text() == m_cases[k]->text());
}

bool Morpfen::alignement() const
{
    return (egalite(0, 1, 2) ||  // 1ere ligne
            egalite(3, 4, 5) ||  // 2e ligne
            egalite(6, 7, 8) ||  // 3e ligne
            egalite(0, 3, 6) ||  // 1ere colonne
            egalite(1, 4, 7) ||  // 2e colonne
            egalite(2, 5, 8) ||  // 3e colonne
            egalite(6, 4, 2) ||  // Diagonale y=x
            egalite(0, 4, 8));   // Diagonale y=-x
}

bool Morpfen::plein() const
{
    // Renvoie false si certaines cases ne sont pas remplies...
    for(QLineEdit* c : m_cases)
    {
        if(c->text().isEmpty())
        {
            return false;
        }
    }

    // ...et true sinon
    return true;
}

void Morpfen::toutGriser()
{
    // On rend inaccessibles toutes les cases
    for(QLineEdit* c : m_cases)
        c->setEnabled(false);
}

void Morpfen::raz()
{
    for(QLineEdit* c : m_cases)
        c->clear();          // On vide les cases

    for(QLineEdit* c : m_cases)
        c->setEnabled(true); // On rend accessibles toutes les cases
}

