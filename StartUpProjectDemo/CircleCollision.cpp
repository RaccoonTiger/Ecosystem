#include "CircleCollision.h"


/*

.cpp crée par Stanley Vu
Mis à jour le Dec.11.2020

*/

/* Calcul de distances avec la méthode de pythagore,
*  ensuite une comparaison entre distances et rayons ensemble.
*  retourne 1 si la collision est vraie, 0 si faux (true false)
*/

/*

bool CircleCollision::Detect(Vect2d a, Vect2d b, float radiusA, float radiusB)
{
	float aX = a.x();
	float aY = a.y();
	float bX = b.x();
	float bY = b.y();
	float deltaXsq = pow((aX - bX), 2);
	float deltaYsq = pow((aY - bY), 2);
	float deltaRadius = pow((radiusA + radiusB), 2);

	if ((deltaXsq + deltaYsq) <= deltaRadius) {
		return true;
	}
	else return false;
}

*/