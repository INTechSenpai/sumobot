/*
	Définit la table de jeu :
	La liste des obstacles présents, connus, supposés, détectés, etc...
	Permet de positionner les obstacles à partir des données des capteurs.
	Repositionne le robot en fonction des bords de la table si ils sont détectés.

	Cette classe est singleton
*/

/*
	Repère utilisé pour toutes les coordonnées :

					
								dune de sable
				 ____________________________________________
				|					 Y						 |
				|					/|\						 |
				|					 ║						 |
				|					 ║						 |
Zone de départ	|					 ║						 | Zone de départ VERTE
VIOLETTE		|					 ║						 |
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
#include "RelativeObstacleMap.h"
#include "utils.h"
#include "math.h"


#define KNOWN_SAND_PRIORITY		90
#define DETECTED_SAND_PRIORITY	16
#define DOORS_PRIORITY			128
#define SHELL_PRIORITY			80


class Table : public Singleton<Table>
{
public:
	Table();


	ObstacleMap getObstacleMap() const;

	/*
		Initialise la 'ostacleMap' avec la configuration connue de la table au début du match.
		side : le côté de la table sur lequel nous nous trouvons
	*/
	void initObstacleMap(Side side);


	/*
		Effectue la mise à jour de la liste des obstacles à partir des informations des capteurs.
		Interprète les obstacles vus comme élément de jeu, bord de table ou bien robot adverse.
		Repositionne éventuellement notre robot si un bord de table détecté le permet.

		relativeObstacleMap : données lues par les capteurs (distances lues en mm).
		notrePosition : position du robot calculée par le bas niveau.
		positionUncertainty : incertitude sur la position
		retour : cette fonction retourne 'true' si les paramètre 'notrePosition' et/ou 'positionUncertainty' a/ont été(s) modifié(s), 'false' sinon.
	*/
	bool updateObstacleMap(const RelativeObstacleMap & relativeObstacleMap, Position & notrePosition, Position & positionUncertainty);

private:
	ObstacleMap obstacleMap;

	/*
		Représente un point de la table pour lequel on dispose d'un information en provenance des capteurs.
		Peut être un point de détection d'obstacle, ou bien une droite de non détection jusqu'à l'horizon.
	*/
	struct DetectionPoint
	{
		float x;	// Position absolue sur la table
		float y;
		bool isAnObstacle;	// Indique si il s'agit d'une détection ou bien d'un horizon
		bool isReliable;	// Vrai uniquement pour les résultats des capteurs ToF (la non-détection correspondra toujours à une absence d'obstacle)
	};

	/*
		Place les points de détection en fonction des données des capteurs
		Rempli le tableau de 'DetectionPoint'
	*/
	void fillDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const RelativeObstacleMap & relativeObstacleMap);
	

};

#endif

