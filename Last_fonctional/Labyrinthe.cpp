#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#include <iostream>
#include <fstream>
#include <vector>
 
using namespace std;


Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.

Environnement* Environnement::init (char* filename)
{
  return new Labyrinthe (filename);
}

Labyrinthe::Labyrinthe (char* filename)
{
  /*************************** Remplissage de la matrice à partir d'un fichier .txt **********************************/
  /* La fonction labyrinthe_correctly_cons renvoie true si la matrice du fichier texte est correcte, c'est à dire qu'elle respecte les contraintes imposées dans le sujet: un commentaire commence par #
     les affiches sont déclarées en tête du fichier

     on ne peut utiliser ques les affiches qui ont été declarées
     Vérifie si les murs sont correctements construits et les affiches sont correctement posées.*/
  vector < vector <int> > mat;

  /**** remplire la matrice a partir du fichier texte****/
  vector <int> line; // line est utilisé pour accéder aux elements d'une ligne de la matrice
  string elem_char ;
  ifstream myfile (filename);
  if (myfile.is_open()) {
    while ( myfile.good() )
      { 
	getline (myfile,elem_char);
	for( unsigned int i=0 ; i < elem_char.size(); i++) {  // cout << "i: "<<i <<endl;
	  switch ( elem_char[i] ) {
	  case '-'  : 
	    line.push_back(1);
	    continue;
	  case '|' : 
	    line.push_back(8);
	    continue;
	  case '+' :
	    line.push_back(2);
	    continue;
	  case ' ' : 
	    line.push_back(0);
	    continue;
	  case 'G' : 
	    line.push_back(3);
	    continue;
	  case 'x' : 
	    line.push_back(4);
	    continue;
	  case 'T' : 
	    line.push_back(5);
	    continue;
	  case 'C' : 
	    line.push_back(6);
	    continue;
	  default :
	    if ( elem_char[i] >= 'a' or  elem_char[i] <= 'z')
	      line.push_back(7);
	    continue;
	  }
	}
	mat.push_back(line); //  ajoute a la matrice mat la ligne line
	line.clear();        // supprime  tous les éléments de line
      }
    myfile.close();
  }
  
  else cout << "Unable to open file"; 
  /*affichage de la matrice pour debug*/
  if (mat.size()==0 and (mat[0].size()) == 0 )
    cout << "matrice vide!" << endl ;	
  for (unsigned int i(0) ;  i  < mat.size() ; i++) {
    for (unsigned int j(0) ;  j < mat[i].size() ; j++) {
      line = mat[i] ;
       // if (line[j]== 0)
       // 	cout << " ";
       //  else
	  cout << line[j] ;
    }
    cout<<endl;
  }
  
  /***********************************************************************************************************************************************/
  						/* remplissage horizontal de tableau de Wall w */
  /***********************************************************************************************************************************************/
 	/*new_wall = false ;
	    if ( i+1 < elem_char.size()) { 
	      if  (elem_char[i+1] == ' ') {
		line.push_back(2);
		new_wall = true;}
	      else if (new_wall) {
		line.push_back(2);
		new_wall = false ;
	      }
	      else line.push_back(1); 
	    }
	    else {
	      line.push_back(2);  
	    new_wall = true ;}*/
  bool labyrinthe_correctly_cons = true  ;// variable booléene qui sera une fonction qui teste si le labyrinthe est correctement construit
  line.clear() ; // supprime  tous les éléments de line afin de pouvoir le réutilisé dans la suite du code
  Wall w [200];  
  int cpt = 0; //compteur utilisé pour savoir remplir une nouvelle ligne du tableau de mur w
  int _ntex = 0; // variable globale d'initialisation des textures du mur
  bool new_wall = false ;	
  if (labyrinthe_correctly_cons ) { 
    if (mat.size()==0 and (mat[0].size()) == 0 ) // check la matrice n'est pas vide
      cout << "matrice vide!" << endl ;
    else 
      {
      	
	for (unsigned int i = 0  ;  i  < mat.size() ; i++) {
	  line = mat[i]; 
	 // cout << "i: "<< i <<"     mat.size(): "<< mat.size()<<endl;     
	  //cout << "mat[i].size(): "<< line.size()<<endl;
	  for (unsigned int j = 0 ;  j < line.size() ; j++) {
	  //cout<<"j: "<<j<<endl;
	     if (line[j] == 2) {
	      if (j+1 < line.size() and (line[j+1] == 1 || line[j+1] == 2) and !new_wall) {
		w[cpt]._x1 =  j ;
		w[cpt]._y1 =  i ;
		// cout<<"i: "<< i << endl;
		// cout<<"w[cpt].y_1 c1: "<< w[cpt]._y1 << endl;
		w[cpt]._x2 =  0 ;
		w[cpt]._y2 =  0 ;
		w[cpt]._ntex = _ntex;
		new_wall = true;
	      }
	      else if (j > 0  and (line[j-1] == 1 || line[j-1] == 2) and new_wall and ((j+1 < line.size()  and (line[j+1] != 1 and line[j+1] != 2)) || j == (line.size() - 1 ))) {    
	      	cout<<"line["<<j-1<<"]: "<<line[j-1]<<endl; 
		w[cpt]._x1 =  w[cpt]._x1 ;
		w[cpt]._y1 =  w[cpt]._y1 ;		      
		// cout<<"w[cpt].y_1 c2: "<< w[cpt]._y1 << endl;
		w[cpt]._x2 =  j ;
		w[cpt]._y2 =  i ;
		w[cpt]._ntex = _ntex;
		cpt++;
		new_wall = false ;
		
	      }
	    }
	  }// end deuxieme boucle for
	}//end premiere boucle for	        
      }
  }//end if labyrinthe_correctly_cons
  else cout << "Votre labyrinthe n'a pas été concu de maniere correct  : les + doivent etre separes par des - ou des | ou des a " << endl;
  //=============================================================================================================================================/
  
 // cout << "ON EST LA!" << endl; // DEBUG
 


/***********************************************************************************************************************************************/
  						/* Largeur  et hauteur de la matrice  */
/***********************************************************************************************************************************************/  
  for (unsigned int i = 0  ;  i  < mat.size() ; i++) {
    if ( mat[i].size() > largeur)
      largeur = mat[i].size();
    }
  
  hauteur = mat.size();


/***********************************************************************************************************************************************/
  						/* Parcours Vertical */
/***********************************************************************************************************************************************/  
  /* Pour parcourir la matrice dans le sens vertical, il faut compléter la matrice avec des 0 */
 // cout<<"largeur: "<<largeur<<endl;
 // cout << "hauteur"<< mat.size() << endl;

  for (unsigned int i = 0  ;  i  < mat.size() ; i++) {
  //cout << "largeur"<< mat[i].size() << endl;
    while( mat[i].size() < largeur){
      mat[i].push_back(0);
    }
  }//end boucle for 
 
  cout << "ON EST LA!" << endl;
  
  	
  //  Transposé de la matrice
  vector < vector <int> > matT;
  vector <int> line2 ;
  for (unsigned int i = 0  ;  i  < largeur ; i++) {
    line2.clear();
    for ( unsigned int j = 0 ;  j < mat.size() ; j++) {
      line2.push_back(mat[j][i]);
      // if (mat[j][i]== 0)
      // 	  cout << " ";
      // else
      // cout<<mat[j][i];
    }// end deuxieme boucle for
    matT.push_back(line2);
    // cout<<endl;
  }//end premiere boucle for
  
  /* la matrice nécessite une correction car les murs verticaux ne sont pas correctement traité,  on ne pouvait pas parcourir la matrice verticalement*/
  /* remplace les chaines de 22222222 par des murs corrects 21111112 */ 
 /*<============
  cout<<"avant fun de re-remplissage"<<endl;
  cout<<"matT.size(): "<< matT.size()<<endl;
  new_wall =true;
  for (unsigned int i(0) ;  i  < matT.size() ; i++) {
     cout<<"matT[i].size(): "<< matT[i].size()<<endl;
    //line = mat[i];
    for (unsigned int j(0) ;  j < matT[i].size() ; j++) {
      //cout <<"ici : "<< matT[i].size() ;
      cout<<matT[i][j];
      if (matT[i][j] == 2){
	//cout<<"j+1: "<< j+1 <<" matTi.size: "<< matT[i].size()<<endl;
	if ( j+1  < matT[i].size()) {  
	  if  (matT[i][j+1] == 0) { 
	    new_wall = true;
	  }
	  else if (new_wall) { 
	    //cout <<"new_wall ";
	    new_wall = false ;
	  }
      	  else matT[i][j] = 1; 
      	}
      	else new_wall = true ;
	// if (line[j]== 0) 
	//   cout << " ";
	// else
      }
    }//end boucle for
    cout<<endl;
  }//end boucle for   =======>*/
  
  //affichage de la matrice pour debug
  cout<<"largeur: "<<largeur<<"    hauteur :  "<<hauteur<<endl;
  for (unsigned int i(0) ;  i  <  matT.size() ; i++) {
    for (unsigned int j(0) ;  j < matT[i].size() ; j++) {
      // if (matT[i][j]== 0)
      // 	  cout << " ";
      // 	  else 
  	    cout << matT[i][j] ;
  	  }
      cout<<endl;
    }
  //construction des murs verticaux

  cout<<"cpt avant :"<<cpt<<endl;
  line.clear();
  new_wall = false ; 
 for (unsigned int i = 0  ;  i  < matT.size() ; i++) {
 	  line = matT[i];
 	  for (unsigned int j = 0 ;  j < line.size() ; j++) {
 	    if (line[j] == 2) {
 	      if (j+1 < line.size() and (line[j+1] == 8 || line[j+1] == 2) and !new_wall) {
 		w[cpt]._x1 =  i ;
 		w[cpt]._y1 =  j ;
 		 cout<<"i: "<< i << endl; 
 		 cout<<"j: "<< j << endl;
 		 cout<<"cpt:"<<cpt<<endl;
 		// cout<<"w[cpt].y_1 c1: "<< w[cpt]._y1 << endl;
 		w[cpt]._x2 =  0 ;
 		w[cpt]._y2 =  0 ;
 		w[cpt]._ntex = _ntex;
 		new_wall = true;
 	      }
 	       else if (j > 0  and (line[j-1] == 8 || line[j-1] == 2) and new_wall and ((j+1 < line.size()  and (line[j+1] != 8 and line[j+1] != 2)) || j == (line.size() - 1 )))
 	       {     
 		w[cpt]._x1 =  w[cpt]._x1 ;
 		w[cpt]._y1 =  w[cpt]._y1 ;		      
 		// cout<<"w[cpt].y_1 c2: "<< w[cpt]._y1 << endl;
 		w[cpt]._x2 =  i ;
 		w[cpt]._y2 =  j ;
 		w[cpt]._ntex = _ntex;
 		cpt++;
 		new_wall = false;
 	      }
 	    }
 	  }// end deuxieme boucle for
 	}//end premiere boucle for
    /****************************/  
    
  _nwall = cpt ;
  cerr << "cpt = " << cpt << endl;
 _walls = new Wall [_nwall ]  ;
 for (int i = 0; i <= _nwall-1; i++){
   _walls[i] = w[i]; 
   cout<<"i= "<<i<<":"<< "{ "<<w[i]._x1<<"; "<<w[i]._y1 <<"; "<< w[i]._x2 <<"; "<<w[i]._y2 <<"; "<<w[i]._ntex<<"; "<<" }"<<endl;
 }



 
 //  cout << "taille tableau"<<_nwall << endl ; 
  _npicts = 0;
  _nboxes = 0;
#if 0
  // la deuxième à une texture différente.
  char	tmp [128];
  sprintf (tmp, "%s/%s", texture_dir, "voiture.jpg");
  _picts [1]._ntex = wall_texture (tmp);
#endif
  // le chasseur.
  _nguards = 1;
  _guards = new Mover* [_nguards];
  _guards [0] = new Chasseur (this);
#if 0
  _guards [1] = new Gardien (this, "Potator");
  
  // indiquer qu'on ne marche pas sur les gardiens.
  _data [(int)(_guards [1] -> _x / scale)][(int)(_guards [1] -> _y / scale)] = 1;
#endif
  //::exit (1);
}

/****************************/

#if 0

/******************************** Fonction d'affichage d'une matrice ********************************/
void affiche_matrix (vector < vector <int> > mat){
  vector < int > ligne ;
  if (mat.size()==0 and (mat[0].size()) == 0 )
    cout << "matrice vide!" << endl ;	
  for (int i(0) ;  i  < mat.size() ; i++)
    {
      for (int j(0) ;  j < mat[i].size() ; j++)
	{
	  ligne = mat[i] ;
	  if (ligne[j]== 0)
	    cout << " ";
	  else cout << ligne[j] ;
	}
      cout<<endl;
        
    }
}
/****************************************************************************************************/


/****************************************************************************************************/


// les 4 murs.
static Wall walls [] = {
  { 0, 0, LAB_WIDTH-1, 0, 0 },
  { LAB_WIDTH-1, 0, LAB_WIDTH-1, LAB_HEIGHT-1, 0 },
  { LAB_WIDTH-1, LAB_HEIGHT-1, 0, LAB_HEIGHT-1, 0 },
  { 0, LAB_HEIGHT-1, 0, 0, 0 },
};
// une affiche.
//  (attention: pour des raisons de rapport d'aspect, les affiches doivent faire 2 de long)
static Wall affiche [] = {
  { 4, 0, 6, 0, 0 },		// première affiche.
  { 8, 0, 10, 0, 0 },		// autre affiche.
};
// 3 caisses.
static Box	caisses [] = {
  { 70, 12, 0 },
  { 10, 5, 0 },
  { 65, 22, 0 },
};
// juste un mur autour et les 3 caisses et le trésor dedans.
for (int i = 0; i < LAB_WIDTH; ++i)
  for (int j = 0; j < LAB_HEIGHT; ++j) {
    if (i == 0 || i == LAB_WIDTH-1 || j == 0 || j == LAB_HEIGHT-1)
      _data [i][j] = 1;
    el se
      _data [i][j] = EMPTY;
  }
// indiquer qu'on ne marche pas sur une caisse.
_data [caisses [0]._x][caisses [0]._y] = 1;
_data [caisses [1]._x][caisses [1]._y] = 1;
_data [caisses [2]._x][caisses [2]._y] = 1;
// les 4 murs.
_nwall = 4;
_walls = walls;
// deux affiches.
_npicts = 2;
_picts = affiche;
// la deuxième à une texture différente.
char	tmp [128];
sprintf (tmp, "%s/%s", texture_dir, "voiture.jpg");
_picts [1]._ntex = wall_texture (tmp);
// 3 caisses.
_nboxes = 3;
_boxes = caisses;
// le trésor.
_treasor._x = 10;
_treasor._y = 10;
_data [_treasor._x][_treasor._y] = 1;	// indiquer qu'on ne marche pas sur le trésor.
// le chasseur et les 4 gardiens.
_nguards = 5;
_guards = new Mover* [_nguards];
_guards [0] = new Chasseur (this);
_guards [1] = new Gardien (this, "drfreak");
_guards [2] = new Gardien (this, "Marvin"); _guards [2] -> _x = 90.; _guards [2] -> _y = 70.;
_guards [3] = new Gardien (this, "Potator"); _guards [3] -> _x = 60.; _guards [3] -> _y = 10.;
_guards [4] = new Gardien (this, "garde"); _guards [4] -> _x = 130.; _guards [4] -> _y = 100.;
// indiquer qu'on ne marche pas sur les gardiens.
_data [(int)(_guards [1] -> _x / scale)][(int)(_guards [1] -> _y / scale)] = 1;
_data [(int)(_guards [2] -> _x / scale)][(int)(_guards [2] -> _y / scale)] = 1;
_data [(int)(_guards [3] -> _x / scale)][(int)(_guards [3] -> _y / scale)] = 1;
_data [(int)(_guards [4] -> _x / scale)][(int)(_guards [4] -> _y / scale)] = 1;
	
	
#endif




