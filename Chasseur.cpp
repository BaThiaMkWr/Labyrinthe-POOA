#include "Chasseur.h"
#include "Gardien.h"
#include "Environnement.h"
#include "Labyrinthe.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;


/*
 *	Tente un deplacement.
 */

bool Chasseur::move_aux (double dx, double dy)
{	
	if (EMPTY == _l -> data ((int)((_x + dx) / Environnement::scale),
							 (int)((_y +dy) / Environnement::scale)))
	{
		_x += dx;
		_y += dy;
		return true;
	}
	return false;
}

/*
 *	Constructeur.
 */

Chasseur::Chasseur (Labyrinthe* l) : Mover (100, 80, l, 0)
{
	_hunter_fire = new Sound ("sons/hunter_fire.wav");
	_hunter_hit = new Sound ("sons/hunter_hit.wav");
	if (_wall_hit == 0)
		_wall_hit = new Sound ("sons/hit_wall.wav");
	perdu= false;
	vie=new int [_l->_nguards];
	for(int i=0;i<_l->_nguards; i++){
	vie[i]=5;
	}
}

/*
 *	Fait bouger la boule de feu (ceci est une exemple, à vous de traiter les collisions spécifiques...)
 */

bool Chasseur::process_fireball (float dx, float dy)
{
	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb -> get_x ()) / Environnement::scale;
	float	y = (_y - _fb -> get_y ()) / Environnement::scale;
	
	float	dist2 = x*x + y*y;
	// on bouge que dans le vide!
	bool b = gard_collision(dx,dy);
	for (int i=1; i<_l->_nguards;i++){
	if (EMPTY == _l -> data ((int)((_fb -> get_x () + dx) / Environnement::scale),
							 (int)((_fb -> get_y () + dy) / Environnement::scale)) && !b )
	{
		message ("Woooshh ..... %d", (int) dist2);
		// il y a la place.
		return true;
	}
	}
	// calculer la distance maximum en ligne droite.
	float	dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
	// faire exploser la boule de feu avec un bruit fonction de la distance.
	_wall_hit -> play (1. - dist2/dmax2);
	message ("Booom...");
	return false;
}
// collision. entre boule de feu et gardien si oui decremente sa durée de vie
bool Chasseur::gard_collision(float dx,float dy)
{	int xf = _fb -> get_x () + dx;
	int yf = _fb -> get_y () + dy;
	bool res = false;
	for (int i=1; i<_l->_nguards;i++){
		float x = _l->_guards[i]->_x+Environnement::scale/2;
		float y = _l->_guards[i]->_y+Environnement::scale/2;
		if (fabs(x-xf) <= Environnement::scale/2 && fabs(y-yf)<=Environnement::scale/2){
			_l->_guards[i]->tomber();
			vie[i]-=1;
			if (vie[i]<=0 && vie[0]){
			((Gardien*)_l->_guards[i])->detruire_gard();
			//_l->_guards[i]->rester_au_sol();
			}
		
		res = true;
		}
		
	}
	return res;
}

/*
 *	Tire sur un ennemi.
 */

void Chasseur::fire (int angle_vertical)
{
	message ("Woooshh...");
	_hunter_fire -> play ();
	_fb -> init (/* position initiale de la boule */ _x, _y, 10.,
				 /* angles de visée */ angle_vertical, _angle);
				 
}
// teste 
bool Chasseur::test(){
	for(int i=0;i<_l->_nguards; i++)
	
		if(vie[i]<=0){
		perdu=true;
		}
	return perdu;
	
}


