/*
	Définition de la classe Robot
	Cette classe est singleton

	Contient l'IA du robot, ainsi que la procédure d'attente de retrait du jumper.
*/


#ifndef _ROBOT_h
#define _ROBOT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Singleton.h"
#include "Obstacle.h"
#include "Position.h"
#include "MotionControlSystem.h"
#include "SensorMgr.h"
#include "pathfinding.h"
#include "dummyPathFinding.h"
#include "Table.h"
#include "utils.h"

#define PIN_JUMPER			A11 // Pin du jumer déclenchant le début du match
#define PIN_DEL_ONBOARD		13	// Pin de la DEL intégrée à la Teensy
#define PIN_DEL_RED			29	// Pin de la DEL verte (du contrôleur de tension)
#define PIN_DEL_GREEN		30	// Pin de la DEL rouge (du contrôleur de tension)

#define XY_TOLERANCE		10		// Unité : mm     | Si l'erreur de positionnement est inférieure à cette tolérance, on considère être arrivé à la position souhaitée
#define ANGLE_TOLERANCE		0.02	// Unité : radian | Idem, mais pour l'orientation du robot

class Robot : public Singleton<Robot>
{
public:
	Robot();

	/*
		Détecte le bord de table pour déterminer de quel côté on joue. Renvoie le côté trouvé.
		Doit être appellé avant d'activer le 'battControlerThread', car cette méthode utilise les DELs rouge et verte du contrôleur de batterie.
	*/
	Side checkSide();


	/* 
		Initialise les attributs de l'objet Robot. Notamment la Table, ainsi que la position initiale du robot.
	*/
	void init(Side side);


	/*
		Fonction bloquante qui attend le retrait du jumper
		Utilise la DEL intégrée à la Teensy comme acquittement visuel
	*/
	void waitForBegining();


	/*
		Fait fonctionner le robot pendant le temps indiqué, et le fait gagner le match si possible.
		duration : durée d'exécution de la fonction, en ms
	*/
	void winMatch(uint32_t duration, Side side);


	/* Déploie le parasol */
	void deployUmbrella();




	// ##### Les méthodes suivantes seront privées, mais sont laissées publiques pour les tests #####


	/*
		Permet de se rendre à la position indiquée.
		Cette méthode actualise la 'obstacleMap' et rappelle le path finding en boucle jusqu'à arriver à destination.
		Renvoie vrai si le robot est arrivé à detination, faux sinon.
	*/
	bool goToPoint(const Position & destination);

	/*
		Idem que goToPoint, mais n'utilise pas le pathfinding, effectue unet rajectoire rectiligne sans considération des obstacles
	*/
	void goStraightToPoint(const Position & destination, int32_t speed);

	/*
		Détermine si le robot est à la position destination ou non, en acceptant une certaine tolérance.
	*/
	bool areWeArrived(const Position & notrePosition, const Position & destination);


	/*
		Longe le bord de la table allant de la serviette aux portes, en asservissant la trajectoire sur la distance au bord de table.
		Distance du bord : 15cm (du cetre du robot au bord de table)
		La trajectoire s'arrète à 25 cm du bord qui fait face au robot
		A la fin de la trajectoire, la position du robot est réglée avec précision.

		Arguents
		side : le côté de la table (vert ou violet)
	*/
	void driveAlongEdgeOfTable(Side side);

	/*
		Calcule la distance moyenne donnée par trois capteurs frontaux (en prennant en compte les différences de précisions des différents capteurs) 
	*/
	int32_t calculateFrontDistance(uint32_t gauche, uint32_t centre, uint32_t droite);

	/*
		Calcule l'angle du robot par rapport au bord de table lui faisant face (utilise les capteurs ToF uniquement)
	*/
	float calculateFrontAngle(uint32_t gauche, uint32_t droite);

	/*
		Script qui ferme les deux portes du bord de la table.
	*/
	void scriptCloseDoors(Side side);

	/*
		Script qui pousse le sable dans le bac à sable.
		Commence à la seconde porte et fini à dans le bac à sable
	*/
	void scriptGoToTowelFromDoors(Side side);

	/*
		Script qui pousse le tas de sable jusque dans le bac à sable
		Commence sur la serviette, le robot faisant plus ou moins face au tas de sable
	*/
	void scriptPushSand(Side side);

	void scriptYolo(Side side, uint32_t duration);


	/*
		Script qui part à la recherche de blocs de sable isolés abandonnés.
		Le script s'exécute tant qu'il n'existe aucun obstacle dans la catégorie 'movableVisible', et s'arrête dès que cette catégorie est non vide.
	*/
	void searchForSand();

	/*
		Script qui place le robot à proximité du bord de table indiqué en argument et lui fait faire un tour sur place.
		Après cette procédure, le robot a une idée précise de sa position selon x, et de son orientation.
	*/
	void recoverPosition(Side side);

private:
	ObstacleMap obstacleMap;

	Table & table;
	SensorMgr & sensorMgr; // Utilisé uniquement dans checkSide()
	MotionControlSystem & motionControlSystem;
	Pathfinding pathfinding;
	DummyPathFinding dummyPathFinding;

};


#endif

