#ifndef LABYRINTHE_H
#define LABYRINTHE_H
#include "Environnement.h"


class Labyrinthe : public Environnement {
private:
	unsigned int largeur;
	unsigned int hauteur;

public:
	
	Labyrinthe (char*);
	//void affiche_matrix (vector < vector <int> >);
       	int width () { return largeur;}	// retourne la largeur du labyrinthe.
	int height () { return hauteur;}	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return 0;
	}	// retourne la case (i, j).
};

#endif
