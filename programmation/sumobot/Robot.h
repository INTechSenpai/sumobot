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
#include "pathfinding.h"
#include "Table.h"
#include "utils.h"

#define PIN_JUMPER			A11 // Pin du jumer d�clenchant le d�but du match
#define PIN_DEL_ONBOARD		13	// Pin de la DEL int�gr�e � la Teensy
#define PIN_DEL_RED			29	// Pin de la DEL verte (du contr�leur de tension)
#define PIN_DEL_GREEN		30	// Pin de la DEL rouge (du contr�leur de tension)

#define XY_TOLERANCE		10		// Unit� : mm     | Si l'erreur de positionnement est inf�rieure � cette tol�rance, on consid�re �tre arriv� � la position souhait�e
#define ANGLE_TOLERANCE		0.02	// Unit� : radian | Idem, mais pour l'orientation du robot

class Robot : public Singleton<Robot>
{
public:
	Robot();

	/*
		D�tecte le bord de table pour d�terminer de quel c�t� on joue. Renvoie le c�t� trouv�.
		Doit �tre appell� avant d'activer le 'battControlerThread', car cette m�thode utilise les DELs rouge et verte du contr�leur de batterie.
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




	// ##### Les m�thodes suivantes seront priv�es, mais sont laiss�es publiques pour les tests #####


	/*
		Permet de se rendre � la position indiqu�e.
		Cette m�thode actualise la 'obstacleMap' et rappelle le path finding en boucle jusqu'� arriver � destination.
		Renvoie vrai si le robot est arriv� � detination, faux sinon.
	*/
	bool goToPoint(const Position & destination);

	/*
		D�termine si le robot est � la position destination ou non, en acceptant une certaine tol�rance.
	*/
	bool areWeArrived(const Position & notrePosition, const Position & destination);


	/*
		Longe le bord de la table allant de la serviette aux portes, en asservissant la trajectoire sur la distance au bord de table.
		Distance du bord : 15cm (du cetre du robot au bord de table)
		La trajectoire s'arr�te � 25 cm du bord qui fait face au robot
		A la fin de la trajectoire, la position du robot est r�gl�e avec pr�cision.

		Arguents
		side : le c�t� de la table (vert ou violet)
	*/
	void driveAlongEdgeOfTable(Side side);


	/*
		Script qui ferme les deux portes du bord de la table.
	*/
	void scriptCloseDoors(Side side);


	/*
		Script qui part � la recherche de blocs de sable isol�s abandonn�s.
		Le script s'ex�cute tant qu'il n'existe aucun obstacle dans la cat�gorie 'movableVisible', et s'arr�te d�s que cette cat�gorie est non vide.
	*/
	void searchForSand();

	/*
		Script qui place le robot � proximit� du bord de table indiqu� en argument et lui fait faire un tour sur place.
		Apr�s cette proc�dure, le robot a une id�e pr�cise de sa position selon x, et de son orientation.
	*/
	void recoverPosition(Side side);

private:
	ObstacleMap obstacleMap;

	Table & table;
	SensorMgr & sensorMgr; // Utilis� uniquement dans checkSide()
	MotionControlSystem & motionControlSystem;
	Pathfinding pathfinding;

};


#endif

