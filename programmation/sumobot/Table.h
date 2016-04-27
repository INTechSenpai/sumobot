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

/* Valeurs de 'Time To Live' en ms */
#define TTL_OPONENT_ROBOT		10000
#define TTL_SAND				10000
#define TTL_TO_BE_SPECIFIED		10000
#define TTL_DECREASE_NOT_SEEN	250

/* Valeurs des rayons des obstacles à créer, en mm */
#define NEW_OBSTACLE_RADIUS		38
#define OPONENT_RADIUS			100


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
		retour : cette fonction retourne 'true' si le paramètre 'notrePosition' a été modifié, 'false' sinon.
	*/
	bool updateObstacleMap(const RelativeObstacleMap & relativeObstacleMap, Position & notrePosition, const Position & positionUncertainty);

//private:
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

		/* Pour associer ce point à un obstacle */
		ObstacleType associatedObstacleType; // Type de l'obstacle associé
		size_t associatedObstacle; // Indice de l'obstacle associé dans le tableau correspondant au type
	};

	/*
		Place les points de détection en fonction des données des capteurs
		Rempli le tableau de 'DetectionPoint'
		Les champs 'x' 'y' 'isAnObstacle' 'isReliable' sont renseignés
	*/
	void fillDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const RelativeObstacleMap & relativeObstacleMap);
	
	/*
		Interprète chaque point de détection en fonction de notre incertitude de positionnement et de 'obstacleMap'
		Renseigne les champs 'associatedObstacleType' et 'associatedObstacle'
		Si un bord de table permet un recalage, cette méthode met à jour la position et l'incertitude en conséquence.
		Elle renvoie vrai si et seulement si elle a modifié la position ou l'incetitude.
	*/
	void interpreteDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const Position & notreIncertitude);

	/*
		Indique si le point de détection donné correspond à cet obstacle, en fonction de l'incertitude de notre position
		Valeur retournée :
		Vrai : on vient probablement de détecter cet obstacle à cet endroit
		Faux : aucun lien entre les deux, cherche encore !
	*/
	bool isThisPointThisObstacle(const DetectionPoint & detectionPoint, const Obstacle & obstacle, const Position & uncertainty);

	/*
		Déplace le robot, ainsi que tous les points de détection afin que ces derniers soient cohérents avec les obstacles FIXES VISIBLES
		Renvoie Vrai si et seulement si le robot a été effectivement déplacé
	*/
	bool moveRobotToMatchFixedObstacles(DetectionPoint tabDetection[NB_CAPTEURS], Position & position);

	/*
		Déplace les obstacles 'MovableVisible', 'ToBe'Determined' et 'OponentRobot' qui ont étés "vus" afin qu'il correspondent à la détection des capteurs
	*/
	void moveObstaclesToMatchDetection(DetectionPoint tabDetection[NB_CAPTEURS]);

	/*
		Supprime les obstacles se trouvant dans la ligne de vue d'un capteur voyant "à l'infini"
		Considère tous les obstacles comme circulaires (les carrés et rectangles sont associés à leur cercle circonscrit)
	*/
	void deleteUndetectedObstacles(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition);

	/*
		Ajoute des obstacles 'ToBeDeterminated' pour chaque 
	*/
	void addObstaclesToBeDeterminated(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition);

	/*
		Interprète les obstacles 'ToBeDeterminated' étant en vue. En les transformant éventuellement en 'OponentRobot' ou 'MovableVisible'
	*/
	void interpreteObstaclesInSight(DetectionPoint tabDetection[NB_CAPTEURS]);

	/*
		Calcule les offsets (x,y) à appliquer à un point de détection afin qu'il soit cohérent avec l'obstacle associé
	*/
	void calculateOffsetToMatch(const DetectionPoint & detectionPoint, float & xOffset, float & yOffset);

	/*
		Indique si l'obstacle (circulaire) centré en 'obstacleCenter' et de rayon au carré 'squaredObstacleRadius' devrait être en vue du capteur dont on donne les coordonnées du point horizon.
	*/
	bool isObstacleInSight(const Position & robotCenter, const Position & obstacleCenter, float squaredObstacleRadius, float xHorizon, float yHorizon);

	/*
		Supprime tous les obstacles étant arrivés en fin de vie
	*/
	void deleteOutdatedObstacles();
};

#endif

