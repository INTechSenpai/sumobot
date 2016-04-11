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
		Fonction bloquante qui attend le retrait du jumper
		Utilise la DEL int�gr�e � la Teensy comme acquittement visuel
	*/
	void waitForBegining();

	void winMatch();

private:
	ObstacleMap obstacleMap;

	Table & table;
	SensorMgr & sensorMgr; // Utilis� uniquement dans checkSide()
	MotionControlSystem & motionControlSystem;
	// PathFinding & pathFinding; // pas encore impl�ment�

};


#endif
