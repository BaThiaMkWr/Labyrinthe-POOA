#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <list>
 
using namespace std;

/*on gère les bruits pour les chasseurs   */
Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.
/*on gère les bruit pour les gardien      */
Sound*   Gardien::_guard_fire ;// bruit de l'arme du gardien.
Sound*   Gardien::_guard_die ;//le cri du gardien qui meurt.
Sound*	 Gardien::_wall_hit; // le gardien tire sur un mur


Environnement* Environnement::init (char* filename)
{
  return new Labyrinthe (filename);
}

Labyrinthe::Labyrinthe (char* filename)
{  
  
  /* Remplissage de la matrice à partir d'un chier .txt                                                                                        */

  
  vector < vector <int> > mat;
  vector <int> line; // line est utilisé pour accéder aux elements d'une ligne de la matrice
  _nguards = 0;
    string elem_char ;
  ifstream myfile (filename);
  if (myfile.is_open()) {
    while ( myfile.good() )
      { 
	getline (myfile,elem_char);// bibliothèque String
	for( unsigned int i=0 ; i < elem_char.size(); i++) {  // cout << "i: "<<i <<endl;
	  switch ( elem_char[i] ) {
	  case '-'  : 
	    line.push_back(1);
	    break;
	  case '|' : 
	    line.push_back(8);
	    break;
	  case '+' :
	    line.push_back(2);
	    break;
	  case ' ' : 
	    line.push_back(0);
	    break;
	  case 'G' : 
	    _nguards++;
	    line.push_back(3);
	    break;
	  case 'x' : 
	    line.push_back(4);
	    break;
	  case 'T' : 
	    line.push_back(5);
	    break;
	  case 'C' : 
	    _nguards++;
	    line.push_back(6);
	    break;
	  default :
	    if ( elem_char[i] >= 'a' and  elem_char[i] <= 'z')
	      line.push_back(7);
	      else line.push_back(9);
	    break;
	  }
	}
	mat.push_back(line); //  ajoute a la matrice mat la ligne line
	line.clear();        // supprime  tous les éléments de line
      }
    myfile.close();
  }
  
  else cout << "Unable to open file"; 


 
  

  /* remplissage horizontal de tableau de Wall w

 /* La fonction labyrinthe_correctly_cons renvoie true si la matrice du fichier texte est correcte, c'est à dire qu'elle respecte les contraintes imposées dans le sujet: un commentaire commence par #
     les affiches sont déclarées en tête du fichier
show_data();
     on ne peut utiliser ques les affiches qui ont été declarées
     Vérifie si les murs sont correctements construits et les affiches sont correctement posées.*/

  bool labyrinthe_correctly_const = labyrinthe_correctly_cons( filename, mat, false );
  line.clear() ; // supprime  tous les éléments de line afin de pouvoir le réutilisé dans la suite du code
  Wall w [200];  
  int cpt = 0; //compteur utilisé pour remplir une nouvelle ligne du tableau de mur w
  int i_box = 0; //compteur utilisé pour remplir une nouvelle ligne du tableau _boxes
  int _ntex = 0; // variable globale d'initialisation des textures du mur
  bool new_wall = false ;
  bool in_lab = false ; // booléen utilisé pour savoir si nous sommes dans le labyrinthe ou pas
  _npicts = 0;
  int nb_affiches = 0;
  Wall affiches [200];
  Box caisses [200] ;
  
  const char* guards [] = {"Potator", "Marvin" , "drfreak", "garde"};
  _guards = new Mover* [_nguards  ];	
  int id_g = 1;
  		
  if (labyrinthe_correctly_const ) { 
    if (mat.size()==0 and (mat[0].size()) == 0 ) // check la matrice n'est pas vide
      cout << "matrice vide!" << endl ;
    else 
      {
	for (unsigned int i = 0  ;  i  < mat.size() ; i++) {
	  line = mat[i]; 
	  for (unsigned int j = 0 ;  j < line.size() ; j++) { 
	    switch ( line[j] ) {
	    case 2 : 	// on rencontre un '+'
	      in_lab = true ;
	      if (j+1 < line.size() and (line[j+1] == 1 || line[j+1] == 2) and !new_wall) {
		w[cpt]._x1 =  j ;
		w[cpt]._y1 =  i ;
		w[cpt]._x2 =  0 ;
		w[cpt]._y2 =  0 ;
		w[cpt]._ntex = _ntex;
		new_wall = true;
		_data [j][i] = 1;
	      }
	      else if (j > 0  and (line[j-1] == 1 || line[j-1] == 2) and new_wall and ((j+1 < line.size()  and (line[j+1] != 1 and line[j+1] != 2)) || j == (line.size() - 1 ))) {    
		w[cpt]._x1 =  w[cpt]._x1 ;
		w[cpt]._y1 =  w[cpt]._y1 ;		      
		w[cpt]._x2 =  j ;
		w[cpt]._y2 =  i ;
		w[cpt]._ntex = _ntex;
		cpt++;
		new_wall = false ;
		_data [j][i] = 1;
	      }
	      break;
	    case 0 : 	// quand il n'y a rien
	      break;
	    case 4 : 	// Caisses
	      if (in_lab) {
		caisses[i_box]._x = j;
		caisses[i_box]._y = i;
		caisses[i_box]._ntex = _ntex;
		_data [caisses [i_box]._x][caisses [i_box]._y] = 1;
		i_box++;}
	      break;
	    case 5 : 	// Tresor
	      if (in_lab) {
		_treasor._x = j;
		_treasor._y = i;
		_data [j][i] = 1;
			}
	      break;
	    case 7 : 	//affiches
	      if (in_lab) {
		_npicts ++ ;
		affiches[nb_affiches]._x1 =  j ;
		affiches[nb_affiches]._y1 =  i ;
		if (j+1 < line.size() and line[j+1] == 1) { 	      
		affiches[nb_affiches]._x2 =  j+1 ;
	        affiches[nb_affiches]._y2 =  i ;}
	        else {
		affiches[nb_affiches]._x2 =  j ;
	        affiches[nb_affiches]._y2 =  i+1 ;}
		affiches[nb_affiches]._ntex = _ntex;
		nb_affiches++;
		_data [j][i] = 1;
	      }
	      break;
	      	case 6 : //Chasseur
		if (in_lab) { 
  		
  		_guards [0] = new Chasseur (this);
		_guards [0] -> _x = (float) j * scale; _guards [0] -> _y = (float ) i * scale;
		_guards [0] -> id_g = 0;
		//_data [(int)(_guards [0] -> _x / scale)][(int)(_guards [0] -> _y / scale)] = 1;
		
		} 
		break;
		case 3 : //Gardiens
		if (in_lab) {
		 _guards [id_g] = new Gardien (this, guards[id_g % 4]);
 		 _guards [id_g] -> _x = (float) j * scale ; _guards [id_g] -> _y = (float ) i * scale;
 		 //_data [(int)(_guards [id_g] -> _x / scale)][(int)(_guards [id_g] -> _y / scale)] = 1;
 		 _guards [id_g] ->id_g = id_g;
 		 id_g++;
		
		} 
		break; 
	    default :
	      _data [j][i] = 1;
	     
	      break;
	    }
	  }// end deuxieme boucle for
	}//end premiere boucle for	        
      }
  }//end if labyrinthe_correctly_cons
  else {
    cout << "Votre labyrinthe n'a pas été concu de maniere correct  : les + doivent etre separes par des - ou des | ou des a " << endl;

    exit(1);
  }

 
  //nombre de caisses
  _nboxes = i_box  ;
 


/*                                            Largeur  et hauteur de la matrice 							       */

  for (unsigned int i = 0  ;  i  < mat.size() ; i++) {
    if ( mat[i].size() > largeur)
      largeur = mat[i].size();
  }
  
  hauteur = mat.size();



  /*                                       Parcours Vertical et Remplissage des gardiens, tresors 						*/
  
  /* Pour parcourir la matrice dans le sens vertical, il faut compléter la matrice avec des 0 */
 
  
  for (unsigned int i = 0  ;  i  < mat.size() ; i++) {
    while( mat[i].size() < largeur){
      mat[i].push_back(0);
    }
  }
 
  	
  /*  Transposé de la matrice*/
  vector < vector <int> > matT;
  vector <int> line2 ;
  for (unsigned int i = 0  ;  i  < largeur ; i++) {
    line2.clear();
    for ( unsigned int j = 0 ;  j < mat.size() ; j++) {
      line2.push_back(mat[j][i]);
    }// end deuxieme boucle for
    matT.push_back(line2);
  }//end premiere boucle for
      

  /*construction des murs verticaux*/
  line.clear();
  new_wall = false ;
  if( labyrinthe_correctly_cons( filename, matT, true )) {
  for (unsigned int i = 0  ;  i  < matT.size() ; i++) {
    line = matT[i];
    for (unsigned int j = 0 ;  j < line.size() ; j++) {
      if (line[j] == 2) {
	if (j+1 < line.size() and (line[j+1] == 8 || line[j+1] == 2) and !new_wall) {
	  w[cpt]._x1 =  i ;
	  w[cpt]._y1 =  j ;
	   w[cpt]._x2 =  0 ;
	  w[cpt]._y2 =  0 ;
	  w[cpt]._ntex = _ntex;
	  new_wall = true;
	}
	else 
	  if (j > 0  and (line[j-1] == 8 || line[j-1] == 2) and new_wall and ((j+1 < line.size() and (line[j+1] != 8 and line[j+1] != 2)) || j == (line.size() - 1 )))
	  {     
	    w[cpt]._x1 =  w[cpt]._x1 ;
	    w[cpt]._y1 =  w[cpt]._y1 ;		      
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
  _walls = new Wall [_nwall ]  ;
    for (int i = 0; i <= _nwall-1; i++){
    _walls[i] = w[i]; 
  }

 _boxes = new Box [_nboxes ] ;
for (int i = 0; i <= _nboxes-1; i++){
    _boxes[i] = caisses[i]; 
 }

 _picts = new Wall [_npicts];
 for (int i = 0; i <= _npicts-1; i++){
   _picts[i] = affiches[i]; 
 }
  traitement_affiches();
 
  }

  /*creation du tableau des distance au tresor*/
  _data_tresor = new int* [LAB_HEIGHT];
  for (int i=0;i<LAB_HEIGHT;i++)
    {
      _data_tresor [i] = new int [LAB_WIDTH];
    }
  for (int i=0;i<LAB_HEIGHT;i++)
    for (int j=0;j<LAB_WIDTH;j++)
      _data_tresor [i][j] = std::numeric_limits<int>::max();
  //lancement de dijkstra 
  _data_tresor[_treasor._y][_treasor._x]=0;
  //dijkstra(_data_tresor,_treasor._x,_treasor._y);


}

void Labyrinthe::set_data (int i, int j, int value) {
for (unsigned int i = 0  ;  i  < LAB_WIDTH ; i++) {
    for (unsigned int j = 0 ;  j < LAB_HEIGHT ; j++) {
	 _data [i][j] = value;
	}
   }
}
/*==================================*/
/*permet d'afficher les tables data */
/*=================================*/
void Labyrinthe::show_data(void) {
 for (unsigned int i = 0  ;  i  < LAB_WIDTH  ; i++) {
    for (unsigned int j = 0 ;  j < LAB_HEIGHT ; j++) {
  if ((int)_data [i][j]==0) cout<<" ";
  else cout<<(int)_data [i][j];
  
  }
 }
}


/*==========================================*/
/*calcul du plus court chemin vers le trésor */
/*=========================================*/
void Labyrinthe::dijkstra(int ** data,int x, int y){

  // Création liste
  list< pair<int, int> > file;

  // Ajout des cases voisines
  for(int k=0; k<8; k++) {
    int i=x;
    int j=y;
    if (k==0){ i--; j--;}
    else if (k==1){ j--;}
    else if (k==2){ i++; j--;}
    else if (k==3){ i--; j++;}
    else if (k==4){ i--; }
    else if (k==5){ i++; j++;}
    else if (k==6){ i++;}
    else if (k==7){ j++;}

    if (i<=0 || j <=0 || i>= LAB_WIDTH || j>= LAB_HEIGHT) {}
    else if (_data[j][i]==1) { data[j][i]=-1; }
    else if(data[j][i]>data[y][x]+1 && data[j][i]!= -1){ data[j][i]=data[y][x]+1; file.push_back(make_pair(i, j)); }
  }

  
  while (!file.empty())
    {
      x = file.front().first;
      y = file.front().second;
      file.pop_front();

      for(int k=0; k<8; k++) {
	int i=x;
	int j=y;
	if (k==0){ i--; j--;}
	else if (k==1){ j--;}
	else if (k==2){ i++; j--;}
	else if (k==3){ i--; j++;}
	else if (k==4){ i--; }
	else if (k==5){ i++; j++;}
	else if (k==6){ i++;}
	else if (k==7){ j++;}

	if (i<=0 || j <=0 || i>= LAB_WIDTH || j>= LAB_HEIGHT) {}
	else if (_data[j][i]==1) { data[j][i]=-1; }
	else if(data[j][i]>data[y][x]+1 && data[j][i]!= -1){ data[j][i]=data[y][x]+1; file.push_back(make_pair(i, j)); }
      }
    }

}



/* La fonction labyrinthe_correctly_cons renvoie true si la matrice du fichier texte est correcte, c'est à dire qu'elle respecte les contraintes imposées dans le sujet: un commentaire commence par #
   les affiches sont déclarées en tête du fichier
   on ne peut utiliser que les affiches qui ont été declarées
   Vérifie si les murs sont correctements construits et les affiches sont correctement posées.*/
vector <char*> nom_affiches ; 
vector <char> affiches ;  

bool  Labyrinthe::labyrinthe_correctly_cons(char* filename, vector < vector <int> > mat, bool mat_transp) {
  string elem_char ;
  ifstream myfile (filename);
  if (myfile.is_open()) {
    while ( myfile.good() )         // returns true if none of the stream's error flags (eofbit, failbit and badbit) are set.
      { 
	getline (myfile,elem_char);// bibliothèque String, met la ligne lue  dans elem_char
	for( unsigned int i=0 ; i < elem_char.size(); i++) { 
	  switch ( elem_char[i] ) {
	  case '#'  : 
	    break;
	  case '+' :
	    if (correct_wall(mat, mat_transp)){             // si les murs sont correctement construits
	      for (unsigned int j = i ; j  < elem_char.size(); j++ ) {       // returns true if none of the stream's error flags (eofbit, failbit and badbit) are set.
		if ( elem_char[j] >= 'a' and  elem_char[j] <= 'z' and elem_char[j] != 'x'   ) { // x etant reservé pour les caisses
		  if(find_affiche(elem_char[j],affiches))// trouve un char dans un vecteur de char, retourne true si trouvé	
		    break;
		  else return false;}           //si l'affiche n'a pas été  déclaré précedemment, on retourne false
	      }		
	      return true;
	    } 
	    else {
	      cout<<"Probleme dans labyrinthe_correctly_cons, correct_wall renvoi : "<< correct_wall(mat, mat_transp)<<endl;
	      return false;}
	  default :
	    if (!mat_transp and (elem_char[i] >= 'a' and  elem_char[i] <= 'z') and i+1 < elem_char.size() and elem_char[i+1] == '\t' ){
	      affiches.push_back(elem_char[i]);
	    char * sect = &elem_char[i+1];
	    char * show = NULL ;
	    show = strtok ( sect, " \t" );
	    nom_affiches.push_back(show);
	    }
	    break;
	  }
	}
      }
    myfile.close();
    return true;
  }
  else cout << "Unable to open file"; 
  return false;
}


/*retourne true si les murs sont correctement construit et les affiches utilisées de manière aproprié */
bool Labyrinthe::correct_wall(vector < vector <int> > mat, bool mat_transp)
{ 	vector <int> line; // line est utilisé pour accéder aux elements d'une ligne de la matrice
  bool in_lab = false ; // pour savoir si nous sommes dans le labyrinthe ou encore dans la première partie du fichier , c'est a dire declaration des affiches 
  bool begining_of_wall = true ;
  for (unsigned int i = 0  ;  i  < mat.size() ; i++) {
    line = mat[i]; 
    for (unsigned int j = 0 ;  j < line.size() ; j++) { 
      switch ( line[j] ) {
      case 2 :
	if   ( (j+1 < line.size() and (line[j+1] !=0 ) and  begining_of_wall  )) // on ne doit pas avoir + --- 
	  { in_lab = true ;
	    begining_of_wall = false;
	    break;}
	else if   (j > 0  and (line[j-1] != 0 )  and !begining_of_wall) { // -+ ou ++ 
	  if ((j+1 < line.size() and line[j+1] == 0) or j+1 >= line.size() )
	  begining_of_wall = true;
	  break;}
	else { 
	  cout<<"begining_of_wall: "<<begining_of_wall<<endl; 
	  return false;}
      case 7 :
	if (in_lab) { 
	  if ( j+1 < line.size() and (line[j+1] == 7  )) 
	    return false;
	  }
	break;
	case 1 : 
	  if (in_lab and !mat_transp ) { 
	    if ((j+1 < line.size() and (line[j+1] == 0 ))          // - est  suivi de vide
		 or  (j > 0  and (line[j-1] == 0 ) ))              // - est  précedé du vide 
	      return false;
	  }
	  break;
	case 8 : 
	  if (in_lab and mat_transp ) { 
	    if ((j+1 < line.size() and (line[j+1] == 0 ))          // - est  suivi de vide
		or  (j > 0  and (line[j-1] == 0 ) ))               // - est  précedé du vide 
	          return false;
	   }
	  break;
	default :
	  break;
	}
      return true;
      }
    }
  cout<<"return true"<<endl;
  return true;
}

/* retourne true si les affiches ont été préalablement déclarées*/
bool Labyrinthe::find_affiche ( char aff, vector <char> affiches) {
  if (!affiches.empty()) {
    for( unsigned int i=0 ; i < affiches.size(); i++) { 
      if (aff == affiches[i]){
      	return true;}
    }
    return false ;
  }
  else return false;
}

void  Labyrinthe::traitement_affiches() {
for (unsigned int i = 0; i <affiches.size(); i++){
      char	tmp [128];
      sprintf (tmp, "%s/%s", Environnement::texture_dir, nom_affiches[i]);
      _picts[i]._ntex = wall_texture (tmp);
 }   
}


