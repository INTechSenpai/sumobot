#ifndef _PATH_h
#define _PATH_h

/*
	D�finition les types Trajectory et UnitMove
	Ces types repr�sentent respectivement une trajectoire et un mouvement �l�mentaire.
	Une trajectoire est une succession de mouvements �l�mentaires enchain�s (sans arr�t du robot entre chaque mouvement)
	Il est d�conseill� de tenter de parcourir des trajectoires trop discontinues. En cas de variations brutales de 
	direction il est recommand� de d�composer le mouvement en plusieurs trajectoires.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <vector>

/* Le rayon infini permet de d�finir une trajectoire rectiligne
 L'infini choisi est le plus grand entier possible sur un int16_t (entier sign� de 16 bits) */
#define INFINITE_RADIUS 32767

/* Un mouvement �l�mentaire est repr�sent� par un rayon de courbure et une longueur
	Toutes les unit�s sont en ticks (unit� de longueur ou d'angle selon le contexte).

	### CAS PARTICULIER ###
	Si le rayon de courbure est nul
	La trajectoire est alors une rotation pure
	La "longueur" repr�sente alors l'angle de rotation
	Un angle positif correspond � une rotation trigonom�trique,
	� l'inverse : n�gatif = antitrigo.
	###					###

	Concernant les sens par rapport aux signes :
		Un rayon de courbure positif repr�sente un virage � gauche
		Un rayon de courbure n�gatif repr�sente un virage � droite
		Une longueur positive repr�sente une marche avant
		Une longueur n�gative repr�sente une marche arri�re

		En r�sum� :
				   Avant
				  \	 +  /
		           \   / 
		            \_/
		Gauche	+	|R|	  -		Droite
				    / \
				   /   \
				  /  -  \
				  Arri�re

*/
struct UnitMove
{
	int16_t length; // Longueur du mouvement, en ticks pour MotionControlSystem et en mm pour le haut niveau
	int16_t bendRadius; // Rayon de courbure, en ticks pour MotionControlSystem et en mm pour le haut niveau
	int32_t speed;	// Vitesse de d�placement (POSITIVE), en ticks/s pour MotionControlSystem et en mm/s pour le haut niveau
};


/* Une trajectoire est repr�sent�e par un tableau de mouvements �l�mentaires
Le premier mouvement effectu� est celui d'indice 0 dans ce tableau */
typedef std::vector<UnitMove> Trajectory;

#endif