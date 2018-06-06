#include "Environnement.h"
#include "Gardien.h"
#include "Chasseur.h"
#include "Labyrinthe.h"
#include <math.h>
#include <iostream> 
#include <stdlib.h>
#define PI 3.1459265
using namespace std;

void Gardien::update (void) {
	
       	//detruire_gard();
       	 
    	 if(!victoir_gagnee ()){
	 float alpha = atan((Environnement::scale/2)/dist_chass())*180/PI;
	 float angle = angle_joueur (this->_x, this->_y, 0);
	 float angle1 = angle + alpha;
	 float angle2 = angle - alpha;
	 _angle = angle;
	 float * xy = new float[2];
	 float * xy1 = new float[2];
	 float * xy2 = new float[2];
	 xy = getXY(angle);
	 xy1 = getXY(angle1);
	 xy2 = getXY(angle2);
	    if (chass_vu(-xy[0], xy[1]) || chass_vu(-xy1[0], xy1[1]) || chass_vu(-xy2[0], xy2[1]))
	     {
		Rotation ( angle );
		this -> move(-xy[0], xy[1]);
		}
	}
}

float * Gardien::getXY(float angle)
{
	float dep=0.1;
	float dx= sin(angle* PI/180)*dep;
	float dy= cos(angle* PI/180)*dep;
	float * xy = new float[2];
	xy[0] = dx;
	xy[1] = dy;
	return xy;
}

float Gardien::dist_chass ()
{
	return sqrt(((_x - _l->_guards[0]->_x)*(_x - _l->_guards[0]->_x)) + ((_y-_l->_guards[0]->_y)*(_y-_l->_guards[0]->_y))); 
}

bool Gardien::move_aux (double dx, double dy)
{
	bool b = gard_chass_collision(_x + dx, _y + dy);
	if ( EMPTY == _l -> data ((int)((_x + Environnement::scale/2 + dx) / Environnement::scale),(int)((_y + Environnement::scale/2 + 	dy) / Environnement::scale)) && !b && vie[0]>0)
	{
			_x +=dx ;
			_y +=dy ;
		return true; 
	}

	return false;

}


//detecter si tu voi le chasseur 
bool Gardien::chass_vu (float dx, float dy)
{
	bool colis = false;
	float xc = _l->_guards[0]->_x;
	float yc = _l->_guards[0]->_y;
	float xg = _x;
	float yg = _y;
	do
	{
		xg +=dx;
		yg +=dy; 
		float diffx = xc - xg;
		float diffy = yc - yg;
		if (diffx<0) diffx = -diffx;
		if (diffy<0) diffy = -diffy;
		if (diffx<=Environnement::scale && diffy<=Environnement::scale){
			colis = true;
		}
	} while( EMPTY == _l -> data ((int)((xg) / Environnement::scale),(int)((yg) / Environnement::scale)) && !colis &&!victoir_gagnee ());
	return colis;

}

/*le gardien tire suivant l'angle donné en paramètre */
/*le bruit de son arme est déclanché au moment du tir*/
		
void Gardien::fire (int angle_vertical) {
	_guard_fire -> play ();
	_fb ->init(_x,_y,10.0,angle_vertical,-_angle);
}

// calculer la distance entre le chasseur et le lieu de l'explosion.
	bool Gardien::process_fireball (float dx, float dy) {		
	float	x = _l->_guards[0]->_x / Environnement::scale;
	float	y = _l->_guards[0]->_y / Environnement::scale;
	float	dist2 =sqrt( x*x + y*y);
//recuperer les coordonner de la boule	
	float xb= (_fb -> get_x ()+dx)/ Environnement::scale;
	float yb= (_fb -> get_y ()+dy)/ Environnement::scale;
// si les coordonner de la boule correspond aux coordonner du chasseur decremente le vie du chasseur	
	if (fabs(x-xb) <= Environnement::scale/2 && fabs(y-yb)<=Environnement::scale/2){
		((Chasseur*)_l->_guards[0])->vie;
		 vie[0]=200;
		for(int i=0; i<=vie[0]; i++){
		
		vie[0]-=1;
		cout <<vie[0] << endl;
		if(vie[0]==0){
		message("Gardien a gagnée relancer le jeu");
		}
	}
}
// la boule ce deplace dans le vide 	
	bool b = gard_chass_collision(dx, dy);	
	if (EMPTY == _l -> data ((int)(((_fb -> get_x () +Environnement::scale/2) + dx) / Environnement::scale),
						(int)(((_fb -> get_y () +Environnement::scale/2 )+ dy) / Environnement::scale))&& !b && vie[0]>0)
	{
		
		message ("attaque gardien ..... %d", (int) dist2);
		
		return true;
	}
	return false;
 }

// tester si ya collision entre le gardien et le chasseur
bool Gardien::gard_chass_collision(float dx, float dy)
{
	bool res = false;
	for (int i=0; i<_l->_nguards; i++)
	{
		float x = _l->_guards[i]->_x;
		float y = _l->_guards[i]->_y;
		if (_x != x || _y != y)
		{
			float diffx = x-dx;
			float diffy = y-dy;
			if (diffx<0) diffx = -diffx;
			if (diffy<0) diffy = -diffy;
			if (diffx<=Environnement::scale && diffy<=Environnement::scale){
				res = true;
			}
		}
	}
	return res;
}
//detecter si ya une colision entre chasseur et gardien
bool Gardien::chass_tresor_collision()
{
	bool res = false;
	float x  = _l->_guards[0]->_x /Environnement::scale/2;
	float y  = _l->_guards[0]->_y /Environnement::scale/2;
	float tx = (_l->_treasor._x -Environnement::scale/2);
	float ty = (_l->_treasor._y -Environnement::scale/2);
	if (abs(x+1-tx)<= Environnement::scale/2 && abs(y+1-ty) <=Environnement::scale/2) 	                                                        
	{
	message("Collision avec tresesor");
	       res = true;
	       
	}
	return res;
}
//
bool Gardien::victoir_gagnee () {
	bool res=false;                 			
                       
		
	if ( vie[0]<=0 || chass_tresor_collision() ){
	message("victoir partie terminer veillez reprendre svp");

    }
  
  return res;
  
}

 // calculer l'angle du jour par rapport aus chasseur   
 float Gardien::angle_joueur (float x,float y, float alpha) {
	 int distX =(_l->_guards[0]->_x-Environnement::scale/2)- x;
	 int distY =(_l->_guards[0]->_y-Environnement::scale/2)- y;
	 float hypo = sqrt ( distX * distX + distY * distY );
	 float angle;

	    if ( distY < 0 ) {
		angle = ( acos ( distY / hypo ) * 180 / PI );
	    } else {
		angle = acos ( distX / hypo ) * 180 / PI + 90;
	    }
	    if ( distX < 0 ) {
		angle = - angle;
	    }

	    if ( distY <= 0 && distX > 0 ) {
		angle = 180 - angle ;
	    }

	    if ( distY < 0 && distX <= 0 ) {
		angle -= 180;
		angle = fabs ( angle );
	    }

	    if ( distY >= 0 && distX < 0 ) {
		angle = fabs ( angle );
	    }
	    return angle + alpha + 180;
}


/* Animation de la rotation des gardiens  */

void Gardien::Rotation ( int angle ) {
    if ( _angle % 360 <= angle % 360 ) {
        rot = ( angle - _angle ) % 360 / 10 ;
        _angle += rot;
    } else if ( _angle % 360 >= angle % 360 ) {
        rot = - ( _angle - angle ) % 360 / 10 ;
        _angle += rot;
    } else {
        rot = 0;
    }

    if ( _angle % 360 == angle % 360 && vie[0]>0) {
        fireFreq++;
        if ( fireFreq == 50 ) {
            fireFreq = 0;
            fire ( 180 );
        }
    } else {
        if ( ( angle - _angle ) % 360 <= 10 ) {
            _angle = angle;
        }
    }
}
	
//cette methode permet de faire disparaitre les gardien qui son mort
void Gardien::detruire_gard() {
      Mover** tab = new Mover*[_l->_nguards];
    int j=0;
    for (int i=1;i<_l->_nguards;i++) {
            tab[j]=_l->_guards[i];
        j++;
      }
  
    ((Labyrinthe*)_l)->set_data((int)_x/Environnement::scale, (int)_y/Environnement::scale,EMPTY);
    _l->_nguards--;
    _l->_guards=tab;
    delete(this);
}
