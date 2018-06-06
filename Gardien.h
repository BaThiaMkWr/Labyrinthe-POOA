#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"
#include "Environnement.h"
#include "Sound.h"
#include <math.h>

class Labyrinthe;

class Gardien : public Mover {
private: 
         int   fireFreq;
         int   rot;
         int*   vie;
        
public:
	static Sound*   _guard_fire ;
	static Sound*   _guard_die ;
	static Sound*   _wall_hit;
	Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele)
	{
	 vie=new int [_l->_nguards]; 
	
	fireFreq=0;
	 _guard_fire = new Sound ("sons/guard_fire.wav");
	_guard_die = new Sound ("sons/guard_die.wav");
	if (_wall_hit == 0)
		_wall_hit = new Sound ("sons/hit_wall.wav");
	 
	}
	
	bool move_aux(double dx,double dy);
	// mon gardien pense très mal!
	void update (void) ;
	// et ne bouge pas!
	bool move (double dx, double dy) {
	return move_aux (dx, dy) || move_aux (dx, 0.0) || move_aux (0.0, dy);
	 }
	// ne sait pas tirer sur un ennemi.
	void fire (int angle_vertical) ;
	// quand a faire bouger la boule de feu...
	bool process_fireball (float dx, float dy) ;
        //Rotation des gardien en fonction de l'angle
        void Rotation (int angle);
        //retun langle entre le Gardien et le chasseur
        float angle_joueur (float x,float y, float alpha);
        float * getXY(float angle);
        //champs de vision du Gardien
       	bool chass_vu(float dx, float dy);
       	//determine si ya une collision entre le chasseur et le gardien
       	bool gard_chass_collision(float dx, float dy);
       	//determine la distance entre le chasseur et le gardien
       	float dist_chass ();
       	//indique la victoir 
        bool victoir_gagnee () ;
        //fair disparaitre le gardien
	void detruire_gard();
	//verifier la colision entre chasseur et tresor
	bool chass_tresor_collision();
	int index();
};

#endif
