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
#include "Table.h"
#include "utils.h"

class Robot : public Singleton<Robot>
{
public:
	Robot();

	/*
		Détecte le bord de table pour déterminer de quel côté on joue. Renvoie le côté trouvé.
		Doit être appellé avant d'activer le sensorThread, car cette méthode utilise les DELs rouge et verte du contrôleur de batterie.
	*/
	Side checkSide();


	/* 
		Initialise les attributs de l'objet Robot. Notamment la Table.
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
	void winMatch(uint32_t duration);


	/* Déploie le parasol */
	void deployUmbrella();

private:
	ObstacleMap obstacleMap;

	Table & table;
	SensorMgr & sensorMgr; // Utilisé uniquement dans checkSide()
	MotionControlSystem & motionControlSystem;
	// PathFinding & pathFinding; // pas encore implémenté

};


#endif

