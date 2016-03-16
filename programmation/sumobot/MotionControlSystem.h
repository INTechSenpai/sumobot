// MotionControlSystem.h

#ifndef _MOTIONCONTROLSYSTEM_h
#define _MOTIONCONTROLSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Singleton.h"
#include "Motor.h"
#include "PID.h"
#include "Average.h"
#include "Encoder.h"
#include "Path.h"
#include "Position.h"
#include "utils.h"
#include <math.h>

#define PIN_A_LEFT_ENCODER	15
#define PIN_B_LEFT_ENCODER	14
#define PIN_A_RIGHT_ENCODER	17
#define PIN_B_RIGHT_ENCODER	16

#define ROBOT_RADIUS		45.0	// Rayon du robot, en mm

#define FREQ_ASSERV			2000	// Fr�quence d'asservissement
#define AVERAGE_SPEED_SIZE	50		// Nombre de valeurs � utiliser dans le calcul de la moyenne glissante permettant de lisser la mesure de vitesse
#define TRACKER_SIZE		800 	// Nombre d'�tats cons�cutifs du syst�me � stocker pour le d�bug

class MotionControlSystem : public Singleton<MotionControlSystem>
{
private:
	Motor motor;
	Encoder leftEncoder;
	Encoder rightEncoder;

	// Trajectoire en cours de parcours
	Trajectory currentTrajectory;

	// Indice courant du tableau de la trajectoire courante
	// (si cet indice d�passe ta taille du tableau, la trajectoire completement parcourue)
	volatile uint32_t currentMove;

	// Position absolue du robot sur la table (en mm et radians)
	Position currentPosition;
	

	/*
	* 		D�finition des variables d'�tat du syst�me (position, vitesse, consigne, ...)
	*
	* 		Les unit�s sont :
	* 			Pour les distances		: ticks
	* 			Pour les vitesses		: ticks/seconde
	* 			Ces unit�s seront vraies pour une fr�quence d'asservissement �gale � FREQ_ASSERV
	*/


	//	Asservissement en vitesse du moteur droit
	PID rightSpeedPID;
	volatile int32_t rightSpeedSetpoint;	// ticks/seconde
	volatile int32_t currentRightSpeed;		// ticks/seconde
	volatile int32_t rightPWM;

	//	Asservissement en vitesse du moteur gauche
	PID leftSpeedPID;
	volatile int32_t leftSpeedSetpoint;		// ticks/seconde
	volatile int32_t currentLeftSpeed;		// ticks/seconde
	volatile int32_t leftPWM;

	//	Asservissement en position : translation
	PID translationPID;
	volatile int32_t translationSetpoint;	// ticks
	volatile int32_t currentDistance;		// ticks

	//	Asservissement en position : rotation
	PID rotationPID;
	volatile int32_t rotationSetpoint;		// angle absolu vis� (en ticks)
	volatile int32_t currentAngle;			// ticks

	// Vitesse de d�placement : variable partag�e par les deux asservissements en position
	volatile int32_t movingSpeed;		// ticks/seconde
	// Pour le calcul de l'acc�l�ration :
	volatile int32_t previousMovingSpeed; // en ticks.s^-2

	// Acc�l�ration maximale (variation maximale de movingSpeed)
	volatile int32_t maxAcceleration;	// ticks*s^-2


	//	Pour faire de jolies courbes de r�ponse du syst�me, la vitesse moyenne c'est mieux !
	Average<int32_t, AVERAGE_SPEED_SIZE> averageLeftSpeed;
	Average<int32_t, AVERAGE_SPEED_SIZE> averageRightSpeed;

	// Variables d'�tat du mouvement
	volatile bool moving;
	volatile bool blocked;

	// Variables d'activation des diff�rents PID
	volatile bool positionControlled;	//  Asservissement en position
	volatile bool leftSpeedControlled;	//	Asservissement en vitesse � gauche
	volatile bool rightSpeedControlled;	//	Asservissement en vitesse � droite
	volatile bool pwmControlled;		//	Mise � jour des PWM gr�ce � l'asservissement en vitesse

	// Variables de r�glage de la d�tection de blocage physique
	unsigned int delayToStop;//En ms
	int minSpeed;// M�me unit� que current[..]Speed
	int speedTolerance;

	//Nombre de ticks de tol�rance pour consid�rer qu'on est arriv� � destination
	int toleranceTranslation;
	int toleranceRotation;


	/*
	* Dispositif d'enregistrement de l'�tat du syst�me pour permettre le d�bug
	* La valeur de TRACKER_SIZE d�pend de la valeur de DEBUG.
	*/
	struct trackerType
	{
		float x;
		float y;
		float angle;

		int consigneVitesseGauche;
		int vitesseGaucheCourante;
		int pwmGauche;

		int consigneVitesseDroite;
		int vitesseDroiteCourante;
		int pwmDroit;

		int consigneTranslation;
		int translationCourante;
		int consigneVitesseTranslation;

		int consigneRotation;
		int rotationCourante;
		int consigneVitesseRotation;
	};

	trackerType trackArray[TRACKER_SIZE];
	unsigned int trackerCursor;


public:
	MotionControlSystem();

	/* Asservissement (fonctions � appeller dans l'interruption associ�e) */
	void control();
	void updatePosition();
	void manageStop();

	/* Tracking pour le d�bug */
	void track();//Stock les valeurs de d�bug
	void printTrackingAll();//Affiche l'int�gralit� du tableau de tracking
	void printTracking(); // Envoie des donn�es pour l'asserv auto
	void printPosition();
	void resetTracking();// Reset le tableau de tracking

	/* Activation et d�sactivation de l'asserv */
	void enablePositionControl(bool);
	void enableLeftSpeedControl(bool);
	void enableRightSpeedControl(bool);
	void enablePwmControl(bool);

	/* Gestion des d�placements */
	void setTrajectory(const Trajectory&);
	uint32_t getCurrentMove();
	void deployMove(); // permet de d�ployer les pelles du robot (� faire en d�but de match)
	void resetMove(); // permet de ranger les pelles du robot (� faire pendant la pr�paration du match)
	bool isMoving() const;
	bool isBlocked() const;
	void stop();
	void setRawPWM(int16_t, int16_t);

private:
	void nextMove();
	bool isPhysicallyBlocked();

public:
	/* Setters et getters des constantes d'asservissement */
	void setTranslationTunings(float, float, float);
	void setRotationTunings(float, float, float);
	void setLeftSpeedTunings(float, float, float);
	void setRightSpeedTunings(float, float, float);
	void getTranslationTunings(float &, float &, float &) const;
	void getRotationTunings(float &, float &, float &) const;
	void getLeftSpeedTunings(float &, float &, float &) const;
	void getRightSpeedTunings(float &, float &, float &) const;

	/* Setter et getter de la position */
	void setPosition(Position &);
	Position & getPosition();
	void resetPosition(void);
	void setDelayToStop(uint32_t);

	/* Getters et setters de d�bug */
	void getPWM(int16_t &, int16_t &);
	void getCurrentSpeed(int32_t &, int32_t &);


	/* TEST */
	void testAsservVitesse(int speed, uint32_t duration, float kp, float ki, float kd);

};


#endif

