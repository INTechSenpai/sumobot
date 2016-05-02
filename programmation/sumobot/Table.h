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
#include "Path.h"
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
#define TTL_DECREASE_NOT_SEEN	10000

/* Valeurs des rayons des obstacles à créer, en mm */
#define NEW_OBSTACLE_RADIUS		38

/* Rayon extérieur du robot, utilisé pour l'évitement, en mm */
#define ROBOT_EXT_RADIUS		100

/* Distance mesurée par les ToF en cas de contact très imminent */
#define CONTACT_OBSTACLE		20

/* Taille maximale du tableau ToBeDeterminated de la ObstacleMap */
#define MAX_SIZE_OBSTACLE_MAP	50

class Table : public Singleton<Table>
{
public:
	Table();

	/* Getters de la 'obstacleMap' */
	ObstacleMap getObstacleMap() const;

	const std::vector<Obstacle> & getMovableVisible() const;
	const std::vector<Obstacle> & getFixedVisible() const;
	const std::vector<Obstacle> & getToBeSpecified() const;

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

	/*
		Active/désactive l'interprétation des données des capteurs, et donc par conséquent l'évitement
		Active/désactive également l'évitement de la dernière chance
	*/
	void enableUpdateObstacleMap(bool enable);

	/*
		Active/désactive l'arrêt pour capteurs renvoyant une trop petite valeur (indépendamment de l'interprétation)
	*/
	void enableLastChanceAvoiding(bool enable);

	/*
	Renvoie vrai si et seulement si le UnitMove courant de la Trajectory donnée est un mouvement autorisé par notre politique d'évitement.
	A savoir : interdiction d'entrer en colision avec les obstacles de 'ToBeDeterminated' et 'OponentRobot'
	*/
	bool isTrajectoryAllowed(int movingDirection);

//DEBUG
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
	void moveObstaclesToMatchDetection(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const Position & incertitude);

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
		Calcule les offsets (x,y) à appliquer à un point de détection afin qu'il soit cohérent avec l'obstacle associé
	*/
	void calculateOffsetToMatch(const DetectionPoint & detectionPoint, float & xOffset, float & yOffset);

	/*
		Indique si l'obstacle (circulaire) centré en 'obstacleCenter' et de rayon au carré 'squaredObstacleRadius' devrait être en vue du capteur dont on donne les coordonnées du point horizon.
	*/
	bool isObstacleInSight(const Position & robotCenter, const Position & obstacleCenter, float obstacleRadius, float xHorizon, float yHorizon, float uncertainty = 0);

	/*
		Supprime tous les obstacles étant arrivés en fin de vie
	*/
	void deleteOutdatedObstacles();

	/*
		Permet d'activer/désactiver la mise à jour des obstacles lors de l'appel à updateObstacleMap
	*/
	bool enableUpdate;

	bool lastChanceAvoiding;

	/*
		Indique si les mouvements avant/arrière sont autorisés étant donné les valeurs des capteurs avant
	*/
	bool forwardMoveAllowed;
	bool backwardMoveAllowed;

	/*
		Met à jour les valeurs de 'forwardMoveAllowed' et 'backwardMoveAllowed'
	*/
	void updateMoveAllowed(DetectionPoint tabDetection[NB_CAPTEURS], const RelativeObstacleMap & relativeObstacleMap);

	/*
		Indique si l'obstacle se trouve sur notre chemin
	*/
	bool isObstacleOnTrajectory(const Obstacle & obstacle, const UnitMove & unitMove, const Position & notrePosition, float moveProgress);

	/*
		Indique si l'obstacle donné est cohérent avec le detectionPoint
		Ne considère que les obstacles MOVABLE_VISIBLE (pour les autres catégories, la réponse sera toujours VRAI)
	*/
	bool isObstacleCoherentWithSensor(const DetectionPoint & sensor, const Position & robotCenter, const ObstacleType & obstacleType, size_t obstacleID, float incertitude);

	size_t getToBeSpecifiedLength();
};

#endif

