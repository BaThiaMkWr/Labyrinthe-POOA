#ifndef LABYRINTHE_H
#define LABYRINTHE_H
#include "Environnement.h"
#include <vector>
#include "string.h"
#define	LAB_WIDTH	200
#define	LAB_HEIGHT	200

using namespace std;

class Labyrinthe : public Environnement {
private:
	unsigned int largeur;
	unsigned int hauteur;
	char	_data [LAB_WIDTH][LAB_HEIGHT];
	int **	_data_tresor ;	// on crée un tableau de valeur de distance de chaque case au trésor
	vector <char*> nom_affiches ; 
	vector <char> affiches ; 
 
	/*retourne true si les murs sont correctement construit et les affiches utilisées de manière aproprié */
	bool correct_wall(vector < vector <int> > mat, bool mat_transp);

	/* retourne true si les affiches ont été préalablement déclarées*/
        bool find_affiche ( char aff,vector <char> affiches); 

	/* La fonction labyrinthe_correctly_cons renvoie true si la matrice du fichier texte est correcte, 
	c'est à dire qu'elle respecte les contraintes imposées dans le sujet: un commentaire commence par 
	#les affiches sont déclarées en tête du fichier on ne peut utiliser ques les affiches qui ont été declarées
         Vérifie si les murs sont correctements construits et les affiches sont correctement posées.*/
	bool  labyrinthe_correctly_cons(char* filename, vector < vector <int> > mat, bool mat_transp);

	/*traitement_affiches s'occupe d'afficher les affiches*/
	void   traitement_affiches(void);
public:
	
	Labyrinthe (char*);
       	int width () { return largeur;}		// retourne la largeur du labyrinthe.
	int height () { return hauteur;}	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return _data [i][j];		// retourne la case (i, j).
	}	
	int data_tresor(int i, int j)
   	{
   	   return _data_tresor[i][j];
  	  }

	void set_data (int i, int j, int value); 
	void show_data(void); 			// montrer la table _data
	void show_data_tresor(void);
	void dijkstra(int ** data,int x, int y);
	int partie_gagnee();
	
	
	
};

#endif
