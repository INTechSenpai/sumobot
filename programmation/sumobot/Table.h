/*
	Définit la table de jeu :
	La liste des obstacles présents, connus, supposés, détectés, etc...
	Permet de positionner les obstacles à partir des données des capteurs.
	Repositionne le robot en fonction des bords de la table si ils sont détectés.
*/

/*
	Repère utilisé pour toutes les coordonnées :

					
								dune de sable
				 ____________________________________________
				|					 Y						 |
				|					/|\						 |
				|					 ║						 |
				|					 ║						 |
Zone de départ	|					 ║						 | Notre zone de départ
adverse			|					 ║						 |
				|					 ║						 |
				|					 ║						 |
				|____________________o════════════════════>X |
				 Les poissons, le filet, encore des poissons 
*/


#ifndef _TABLE_h
#define _TABLE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Singleton.h"
#include "Obstacle.h"

class Table : public Singleton<Table>
{
public:
	Table();

	/*
		Initialise la 'ostacleMap' avec la configuration connue de la table au début du match.
		shellConfig : le placement des coquillages se fait selon l'une des 5 configurations données,
		le numéro correspond à celui utilisé dans le règlement de la coupe.
	*/
	void initObstacleMap(ObstacleMap & obstacleMap, uint8_t shellConfig);


	/*
		Effectue la mise à jour de la liste des obstacles à partir des informations des capteurs.
		Interprète les obstacles vus comme élément de jeu, bord de table ou bien robot adverse.
		Repositionne éventuellement notre robot si un bord de table détecté le permet.

		obstacleMap : liste courante des obstacles de la table, elle sera mise à jour par cette fonction.
		relativeObstacleMap : données lues par les capteurs (distances lues en mm).
		notrePosition : position du robot calculée par le bas niveau (avec une certaine incertitude).
		retour : cette fonction retourne 'true' si le paramètre 'notrePosition' a été modifié, 'false' sinon.
	*/
	bool updateObstacleMap(ObstacleMap obstacleMap, const RelativeObstacleMap & relativeObstacleMap, Position & notrePosition);

private:

};

#endif

