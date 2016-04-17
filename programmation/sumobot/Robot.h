/*
	D�finition de la classe Robot
	Cette classe est singleton

	Contient l'IA du robot, ainsi que la proc�dure d'attente de retrait du jumper.
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

#define PIN_JUMPER			A11 // Pin du jumer d�clenchant le d�but du match
#define PIN_DEL_ONBOARD		13	// Pin de la DEL int�gr�e � la Teensy
#define PIN_DEL_RED			29	// Pin de la DEL verte (du contr�leur de tension)
#define PIN_DEL_GREEN		30	// Pin de la DEL rouge (du contr�leur de tension)

class Robot : public Singleton<Robot>
{
public:
	Robot();

	/*
		D�tecte le bord de table pour d�terminer de quel c�t� on joue. Renvoie le c�t� trouv�.
		Doit �tre appell� avant d'activer le sensorThread, car cette m�thode utilise les DELs rouge et verte du contr�leur de batterie.
	*/
	Side checkSide();


	/* 
		Initialise les attributs de l'objet Robot. Notamment la Table, ainsi que la position initiale du robot.
	*/
	void init(Side side);


	/*
		Fonction bloquante qui attend le retrait du jumper
		Utilise la DEL int�gr�e � la Teensy comme acquittement visuel
	*/
	void waitForBegining();


	/*
		Fait fonctionner le robot pendant le temps indiqu�, et le fait gagner le match si possible.
		duration : dur�e d'ex�cution de la fonction, en ms
	*/
	void winMatch(uint32_t duration);


	/* D�ploie le parasol */
	void deployUmbrella();

private:
	ObstacleMap obstacleMap;

	Table & table;
	SensorMgr & sensorMgr; // Utilis� uniquement dans checkSide()
	MotionControlSystem & motionControlSystem;
	// PathFinding & pathFinding; // pas encore impl�ment�

};


#endif

