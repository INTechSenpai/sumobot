#ifndef _PATH_h
#define _PATH_h

/*
	Définition les types Trajectory et UnitMove
	Ces types représentent respectivement une trajectoire et un mouvement élémentaire.
	Une trajectoire est une succession de mouvements élémentaires enchainés (sans arrêt du robot entre chaque mouvement)
	Il est déconseillé de tenter de parcourir des trajectoires trop discontinues. En cas de variations brutales de 
	direction il est recommandé de décomposer le mouvement en plusieurs trajectoires.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <vector>

/* Le rayon "infini" permet de définir une trajectoire rectiligne
 */
#define INFINITE_RADIUS 424242424

/* Un mouvement élémentaire est représenté par un rayon de courbure et une longueur
	Toutes les unités sont en ticks (unité de longueur ou d'angle selon le contexte).

	### CAS PARTICULIER ###
	Si le rayon de courbure est nul
	La trajectoire est alors une rotation pure
	La "longueur" représente alors l'angle de rotation
	Un angle positif correspond à une rotation trigonométrique,
	à l'inverse : négatif = antitrigo.
	###					###

	Concernant les sens par rapport aux signes :
		Un rayon de courbure positif représente un virage à gauche
		Un rayon de courbure négatif représente un virage à droite
		Une longueur positive représente une marche avant
		Une longueur négative représente une marche arrière

		En résumé :
				   Avant
				  \	 +  /
		           \   / 
		            \_/
		Gauche	+	|R|	  -		Droite
				    / \
				   /   \
				  /  -  \
				  Arrière

*/
struct UnitMove
{
	int32_t length; // Longueur du mouvement, en ticks pour MotionControlSystem et en mm pour le haut niveau
	int32_t bendRadius; // Rayon de courbure, en ticks pour MotionControlSystem et en mm pour le haut niveau
	int32_t speed;	// Vitesse de déplacement (POSITIVE), en ticks/s pour MotionControlSystem et en mm/s pour le haut niveau
};


/* Une trajectoire est représentée par un tableau de mouvements élémentaires
Le premier mouvement effectué est celui d'indice 0 dans ce tableau */
typedef std::vector<UnitMove> Trajectory;

#endif