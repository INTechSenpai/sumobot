#include "MotionControlSystem.h"


MotionControlSystem::MotionControlSystem() : 
motor(), 
leftEncoder(PIN_A_LEFT_ENCODER, PIN_B_LEFT_ENCODER), 
rightEncoder(PIN_A_RIGHT_ENCODER, PIN_B_RIGHT_ENCODER),
rightSpeedPID(&currentRightSpeed, &rightPWM, &rightSpeedSetpoint),
leftSpeedPID(&currentLeftSpeed, &leftPWM, &leftSpeedSetpoint),
translationPID(&currentDistance, &movingSpeed, &translationSetpoint),
rotationPID(&currentAngle, &movingSpeed, &rotationSetpoint),
averageLeftSpeed(), averageRightSpeed()
{
	positionControlled = true;
	leftSpeedControlled = true;
	rightSpeedControlled = true;
	pwmControlled = true;

	rotationSetpoint = 0;
	translationSetpoint = 0;
	leftSpeedSetpoint = 0;
	rightSpeedSetpoint = 0;

	moving = false;
	blocked = false;
	paused = false;

	leftSpeedPID.setOutputLimits(-1023, 1023);
	rightSpeedPID.setOutputLimits(-1023, 1023);

	movingSpeed = 0;
	previousMovingSpeed = 0;
	maxAcceleration = 13000;

	delayToStop = 100;
	minSpeed = 500;
	speedTolerance = 5000;
	toleranceTranslation = 150;
	toleranceRotation = 150;

	translationPID.setTunings(6.5, 0, 250);
	rotationPID.setTunings(10, 0, 1900);
	leftSpeedPID.setTunings(2, 0.01, 100);
	rightSpeedPID.setTunings(2, 0.01, 50);
	resetPosition();
	stop();
}

void MotionControlSystem::enablePositionControl(bool enabled)
{
	positionControlled = enabled;
}

void MotionControlSystem::enableLeftSpeedControl(bool enable)
{
	leftSpeedControlled = enable;
}

void MotionControlSystem::enableRightSpeedControl(bool enable)
{
	rightSpeedControlled = enable;
}

void MotionControlSystem::enablePwmControl(bool enable)
{
	pwmControlled = enable;
}

void MotionControlSystem::setTrajectory(const Trajectory& newTrajectory)
{
	cli();
	currentMove = -1; // L'appel à nextMove() incrémentera currentMove et il vaudra bien 0
	currentTrajectory = newTrajectory;
	moving = true;
	blocked = false;
	nextMove();
	sei();
}

int MotionControlSystem::getMovingDirection() const
{
	if (currentMove < currentTrajectory.size())
	{
		if (currentTrajectory.at(currentMove).getBendRadiusTicks() == 0)
		{
			return 0;
		}
		else if (currentTrajectory.at(currentMove).getLengthTicks() > 0)
		{
			return 1;
		}
		else if (currentTrajectory.at(currentMove).getLengthTicks() < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}


void MotionControlSystem::deployMove()
{
	positionControlled = false;
	leftSpeedControlled = false;
	rightSpeedControlled = false;
	leftPWM = -1023;
	rightPWM = 1023;
	delay(100);
	positionControlled = true;
	leftSpeedControlled = true;
	rightSpeedControlled = true;
	stop();
}

void MotionControlSystem::resetMove()
{
	positionControlled = false;
	leftSpeedControlled = false;
	rightSpeedControlled = false;
	leftPWM = 1023;
	rightPWM = -1023;
	delay(100);
	positionControlled = true;
	leftSpeedControlled = true;
	rightSpeedControlled = true;
	stop();
}

void MotionControlSystem::desengageMove(bool forward, bool asymetric)
{
	positionControlled = false;
	leftSpeedControlled = false;
	rightSpeedControlled = false;
	if (forward)
	{
		leftPWM = 1023;
		if (asymetric)
		{
			rightPWM = 0;
		}
		else
		{
			rightPWM = 1023;
		}
	}
	else
	{
		if (asymetric)
		{
			leftPWM = 0;
		}
		else
		{
			leftPWM = -1023;
		}
		rightPWM = -1023;
	}
	delay(500);
	positionControlled = true;
	leftSpeedControlled = true;
	rightSpeedControlled = true;
	stop();
}

inline void MotionControlSystem::nextMove()
{
	if (currentMove < currentTrajectory.size())
	{
		if (currentTrajectory[currentMove].stopAfterMove)
		{
			movingSpeed = 0;
			previousMovingSpeed = 0;
		}

		if (currentTrajectory[currentMove].getBendRadiusTicks() == 0)
		{
			positionUncertainty.orientation += ERROR_AFTER_ROTATION;
		}
		else
		{
			positionUncertainty.orientation += ERROR_AFTER_TRANSLATION;
			float increment = positionUncertainty.orientation * ABS(currentTrajectory[currentMove].getLengthMm());
			positionUncertainty.x += increment;
			positionUncertainty.y += increment;
		}
	}

	currentMove++;
	
	if (currentMove < currentTrajectory.size())
	{
		if (currentTrajectory[currentMove].getBendRadiusTicks() == 0)
		{
			rotationSetpoint = currentAngle + currentTrajectory[currentMove].getLengthTicks();
		}
		else
		{
			translationSetpoint = currentDistance + currentTrajectory[currentMove].getLengthTicks();
		}
	}
	else
	{
		rotationSetpoint = currentAngle;
		translationSetpoint = currentDistance;
	}
}

void MotionControlSystem::control()
{
	// Pour le calcul de la vitesse instantanée :
	static int32_t previousLeftTicks = 0;
	static int32_t previousRightTicks = 0;

	// Récupération des informations des encodeurs (nombre de ticks)
	int32_t rightTicks = -(rightEncoder.read());
	int32_t leftTicks = leftEncoder.read();

	currentLeftSpeed = (leftTicks - previousLeftTicks) * FREQ_ASSERV; // (nb-de-tick-passés)*(freq_asserv) (ticks/sec)
	currentRightSpeed = (rightTicks - previousRightTicks) * FREQ_ASSERV;

	previousLeftTicks = leftTicks;
	previousRightTicks = rightTicks;

	averageLeftSpeed.add(currentLeftSpeed);
	averageRightSpeed.add(currentRightSpeed);

	// On effectue un lissage des valeurs de current[..]Speed en utilisant une moyenne glissante
	currentLeftSpeed = averageLeftSpeed.value();
	currentRightSpeed = averageRightSpeed.value();

	currentDistance = (leftTicks + rightTicks) / 2;
	currentAngle = (rightTicks - leftTicks) / 2;


	if (positionControlled)
	{
		if (currentMove >= currentTrajectory.size())
		{// Si la trajectoire est terminée ou inexistante : pas de mouvement
			movingSpeed = 0;
			leftSpeedSetpoint = 0;
			rightSpeedSetpoint = 0;
		}
		else
		{

			/* Vérification de fin de mouvement élémentaire */

			if (currentTrajectory[currentMove].getBendRadiusTicks() == 0)
			{// Cas d'un mouvement purement rotatif
				if ( (currentTrajectory[currentMove].getLengthTicks() >= 0 && currentAngle >= rotationSetpoint)
				  || (currentTrajectory[currentMove].getLengthTicks() <  0 && currentAngle <= rotationSetpoint) )
				{// Rotation terminée
					nextMove();
				}
			}
			else
			{// Cas d'une trajectoire courbe standard
				if ( (currentTrajectory[currentMove].getLengthTicks() >= 0 && currentDistance >= translationSetpoint)
				  || (currentTrajectory[currentMove].getLengthTicks() <  0 && currentDistance <= translationSetpoint) )
				{// Translation terminée
					nextMove();
				}
			}


			// On vérifie de nouveau l'existance du mouvement élémentaire courant
			if (currentMove >= currentTrajectory.size())
			{// Si la trajectoire est terminée ou inexistante : pas de mouvement
				movingSpeed = 0;
				leftSpeedSetpoint = 0;
				rightSpeedSetpoint = 0;
			}
			else
			{
				int32_t maxSpeed = currentTrajectory[currentMove].getSpeedTicks_S();


				/*  Si il est spécifié explicitement que ce mouvement élémentaire doit se terminer
					à l'arrêt ("stopAfterMove" est alors passé à TRUE).
				*/
				if (currentTrajectory[currentMove].stopAfterMove)
				{
					if (currentTrajectory[currentMove].getBendRadiusTicks() == 0)
					{// Cas d'un mouvement purement rotatif
						rotationPID.compute();		// Actualise la valeur de 'movingSpeed'
						
						// Limitation de la consigne de vitesse en rotation
						if (movingSpeed > maxSpeed)
							movingSpeed = maxSpeed;
						else if (movingSpeed < -maxSpeed)
							movingSpeed = -maxSpeed;
					}
					else
					{// Cas d'une trajectoire courbe standard
						translationPID.compute();	// Actualise la valeur de 'movingSpeed'

						// Limitation de la consigne de vitesse en translation
						if (movingSpeed > maxSpeed)
							movingSpeed = maxSpeed;
						else if (movingSpeed < -maxSpeed)
							movingSpeed = -maxSpeed;
					}
				}
				else
				{
					if (currentTrajectory[currentMove].getLengthTicks() > 0)
					{
						movingSpeed = maxSpeed;
					}
					else
					{
						movingSpeed = -maxSpeed;
					}
				}


				/* Gestion du mode "PAUSED" */
				if (paused)
				{
					movingSpeed = 0;
				}


				/* Limitation des variations de movingSpeed (limitation de l'accélération) */
				if (movingSpeed - previousMovingSpeed > maxAcceleration / FREQ_ASSERV)
				{
					movingSpeed = previousMovingSpeed + maxAcceleration / FREQ_ASSERV;
				}
				else if (previousMovingSpeed - movingSpeed > maxAcceleration / FREQ_ASSERV)
				{
					movingSpeed = previousMovingSpeed - maxAcceleration / FREQ_ASSERV;
				}
				previousMovingSpeed = movingSpeed;





				/* Calcul des vitesses des deux moteurs à partir de movingSpeed et de bendRadius */
				static int32_t radius;
				radius = currentTrajectory[currentMove].getBendRadiusTicks();
				if (radius == 0)
				{
					leftSpeedSetpoint = -movingSpeed;
					rightSpeedSetpoint = movingSpeed;
				}
				else if (radius == INFINITE_RADIUS)
				{
					leftSpeedSetpoint = movingSpeed;
					rightSpeedSetpoint = movingSpeed;
				}
				else
				{
					leftSpeedSetpoint = (int32_t)(((double)radius - ROBOT_RADIUS / TICK_TO_MM)*(double)movingSpeed / (double)radius);
					rightSpeedSetpoint = (int32_t)(((double)radius + ROBOT_RADIUS / TICK_TO_MM)*(double)movingSpeed / (double)radius);
				}
			}
		}

	}



	if (leftSpeedControlled)
		leftSpeedPID.compute();		// Actualise la valeur de 'leftPWM'
	if (rightSpeedControlled)
		rightSpeedPID.compute();	// Actualise la valeur de 'rightPWM'


	if (pwmControlled)
	{
		motor.runLeft(leftPWM);
		motor.runRight(rightPWM);
	}
}

bool MotionControlSystem::isPhysicallyBlocked()
{
	return
		(ABS(currentLeftSpeed) <= minSpeed && ABS(currentRightSpeed) <= minSpeed) || /* Cas de la fin du mouvement : tout est à l'arrêt ou presque */ 
		(ABS(currentLeftSpeed - leftSpeedSetpoint) > speedTolerance) ||
		(ABS(currentRightSpeed - rightSpeedSetpoint) > speedTolerance);
}

void MotionControlSystem::manageStop()
{
	static uint32_t time = 0;
	static uint32_t previousMove = 0;

	// Au changement de UnitMove, on réinitialise le timer
	if (currentMove != previousMove)
	{
		previousMove = currentMove;
		time = 0;
	}

	if ((isPhysicallyBlocked() && moving) && !paused)
	{

		if (time == 0)
		{ //Début du timer
			time = millis();
		}
		else
		{
			if ((millis() - time) >= delayToStop)
			{ //Si arrêté plus de 'delayToStop' ms
				if (currentMove >= currentTrajectory.size())
				{// Si la trajectoire est terminée
					blocked = false;
					stop();
				}
				else if (
							(currentTrajectory[currentMove].getBendRadiusTicks() != 0 && ABS(currentDistance - translationSetpoint) <= toleranceTranslation) || 
							(currentTrajectory[currentMove].getBendRadiusTicks() == 0 && ABS(currentAngle - rotationSetpoint) <= toleranceRotation)
						)
				{// Si on est suffisament proche de la fin du mouvement élémentaire
					blocked = false;
					if (currentMove == currentTrajectory.size() - 1)
					{// Il s'agit du dernier mouvement élémentaire de la trajectoire
						stop();
					}
					else
					{
						nextMove();
					}
				}
				else
				{// Sinon : il d'agit d'un blocage physique
					blocked = true;
					stop();
				}
			}
		}
	}
	else
	{
		time = 0;
		if (moving)
			blocked = false;
	}
}

void MotionControlSystem::updatePosition() 
{
	static int32_t lastDistance = 0;
	static int32_t lastAngle = 0;

	static float deltaDistanceMm;
	static float deltaAngleRadian;

	deltaDistanceMm = (currentDistance - lastDistance) * TICK_TO_MM;
	lastDistance = currentDistance;

	deltaAngleRadian = (currentAngle - lastAngle) * TICK_TO_RADIAN;
	lastAngle = currentAngle;

	currentPosition.orientation += deltaAngleRadian;

	currentPosition.x += (deltaDistanceMm * cos(currentPosition.orientation));
	currentPosition.y += (deltaDistanceMm * sin(currentPosition.orientation));
}


void MotionControlSystem::stop() 
{

	translationSetpoint = currentDistance;
	rotationSetpoint = currentAngle;
	leftSpeedSetpoint = 0;
	rightSpeedSetpoint = 0;
	leftPWM = 0;
	rightPWM = 0;

	currentTrajectory.clear();
	currentMove = 0;

	movingSpeed = 0;
	previousMovingSpeed = 0;

	motor.runLeft(0);
	motor.runRight(0);
	moving = false;
	translationPID.resetErrors();
	rotationPID.resetErrors();
	leftSpeedPID.resetErrors();
	rightSpeedPID.resetErrors();
}

void MotionControlSystem::setRawPWM(int16_t left, int16_t right)
{
	motor.runLeft(left);
	motor.runRight(right);
}

void MotionControlSystem::setPause(bool enable)
{
	paused = enable;
}

float MotionControlSystem::getMoveProgress()
{
	if (currentMove < currentTrajectory.size())
	{
		return 1 - ABS((float)(currentDistance - translationSetpoint) / (float)currentTrajectory.at(currentMove).getLengthTicks());
	}
	else
	{
		return 1;
	}
}

void MotionControlSystem::track()
{
	this->trackArray[trackerCursor].x = currentPosition.x;
	this->trackArray[trackerCursor].y = currentPosition.y;
	this->trackArray[trackerCursor].angle = currentPosition.orientation;

	this->trackArray[trackerCursor].consigneVitesseGauche = leftSpeedSetpoint;
	this->trackArray[trackerCursor].vitesseGaucheCourante = currentLeftSpeed;
	this->trackArray[trackerCursor].pwmGauche = leftPWM;

	this->trackArray[trackerCursor].consigneVitesseDroite = rightSpeedSetpoint;
	this->trackArray[trackerCursor].vitesseDroiteCourante = currentRightSpeed;
	this->trackArray[trackerCursor].pwmDroit = rightPWM;

	this->trackArray[trackerCursor].consigneTranslation = translationSetpoint;
	this->trackArray[trackerCursor].translationCourante = currentDistance;
	this->trackArray[trackerCursor].consigneVitesseTranslation = movingSpeed;

	this->trackArray[trackerCursor].consigneRotation = rotationSetpoint;
	this->trackArray[trackerCursor].rotationCourante = currentAngle;
	this->trackArray[trackerCursor].consigneVitesseRotation = movingSpeed;

	trackerCursor = (trackerCursor + 1) % (TRACKER_SIZE);
}

void MotionControlSystem::printTrackingAll()
{
	for (int i = 0; i<TRACKER_SIZE; i++)
	{
		Serial.printf("%f\t%f\t%f\t",
			trackArray[i].x, trackArray[i].y, trackArray[i].angle);
		Serial.printf("%d\t%d\t%d\t",
			trackArray[i].consigneVitesseGauche, trackArray[i].vitesseGaucheCourante, trackArray[i].pwmGauche);
		Serial.printf("%d\t%d\t%d\t",
			trackArray[i].consigneVitesseDroite, trackArray[i].vitesseDroiteCourante, trackArray[i].pwmDroit);
		Serial.printf("%d\t%d\t%d\t",
			trackArray[i].consigneTranslation, trackArray[i].translationCourante, trackArray[i].consigneVitesseTranslation);
		Serial.printf("%d\t%d\t%d\t",
			trackArray[i].consigneRotation, trackArray[i].rotationCourante, trackArray[i].consigneVitesseRotation);
		Serial.printf("\r\n");
	}
}

void MotionControlSystem::printTracking() // Envoie les données nécessaires à l'analyse d'asserv / l'asserv auto (Python)
{
	for (int i = 0; i<TRACKER_SIZE; i++)
	{
		Serial.printf("%d\t%d\t", trackArray[i].vitesseGaucheCourante, trackArray[i].vitesseDroiteCourante);
		Serial.printf("%d\t%d\t%d\t%d\t", trackArray[i].consigneVitesseGauche, trackArray[i].consigneVitesseDroite, trackArray[i].pwmGauche, trackArray[i].pwmDroit);
		Serial.printf("\r\n");
	}
}

void MotionControlSystem::printPosition()
{
	for (int i = 0; i<TRACKER_SIZE; i++)
	{
		Serial.printf("%d\t%d\t%d\t",
			trackArray[i].consigneTranslation, trackArray[i].translationCourante, trackArray[i].consigneVitesseTranslation);
		Serial.printf("%d\t%d\t%d\t",
			trackArray[i].consigneRotation, trackArray[i].rotationCourante, trackArray[i].consigneVitesseRotation);
		Serial.printf("\r\n");
	}
}

void MotionControlSystem::resetTracking()
{
	trackerType zero;
	zero.angle = 0;
	zero.consigneRotation = 0;
	zero.consigneTranslation = 0;
	zero.consigneVitesseDroite = 0;
	zero.consigneVitesseGauche = 0;
	zero.consigneVitesseRotation = 0;
	zero.consigneVitesseTranslation = 0;
	zero.pwmDroit = 0;
	zero.pwmGauche = 0;
	zero.rotationCourante = 0;
	zero.translationCourante = 0;
	zero.vitesseDroiteCourante = 0;
	zero.vitesseGaucheCourante = 0;
	zero.x = 0;
	zero.y = 0;

	for (int i = 0; i<TRACKER_SIZE; i++)
	{
		trackArray[i] = zero;
	}
	trackerCursor = 0;
}



/**
* Getters/Setters des constantes d'asservissement en translation/rotation/vitesse
*/

void MotionControlSystem::getTranslationTunings(float &kp, float &ki, float &kd) const {
	kp = translationPID.getKp();
	ki = translationPID.getKi();
	kd = translationPID.getKd();
}
void MotionControlSystem::getRotationTunings(float &kp, float &ki, float &kd) const {
	kp = rotationPID.getKp();
	ki = rotationPID.getKi();
	kd = rotationPID.getKd();
}
void MotionControlSystem::getLeftSpeedTunings(float &kp, float &ki, float &kd) const {
	kp = leftSpeedPID.getKp();
	ki = leftSpeedPID.getKi();
	kd = leftSpeedPID.getKd();
}
void MotionControlSystem::getRightSpeedTunings(float &kp, float &ki, float &kd) const {
	kp = rightSpeedPID.getKp();
	ki = rightSpeedPID.getKi();
	kd = rightSpeedPID.getKd();
}
void MotionControlSystem::setTranslationTunings(float kp, float ki, float kd) {
	translationPID.setTunings(kp, ki, kd);
}
void MotionControlSystem::setRotationTunings(float kp, float ki, float kd) {
	rotationPID.setTunings(kp, ki, kd);
}
void MotionControlSystem::setLeftSpeedTunings(float kp, float ki, float kd) {
	leftSpeedPID.setTunings(kp, ki, kd);
}
void MotionControlSystem::setRightSpeedTunings(float kp, float ki, float kd) {
	rightSpeedPID.setTunings(kp, ki, kd);
}


/*
* Getters/Setters des variables de position haut niveau
*/
void MotionControlSystem::setPosition(const Position & newPosition)
{
	currentPosition = newPosition;
}

void MotionControlSystem::getPosition(Position & returnedPosition)
{
	returnedPosition = currentPosition;
}


void MotionControlSystem::resetPosition()
{
	currentPosition.x = 0;
	currentPosition.y = 0;
	currentPosition.orientation = 0;
	stop();
}

void MotionControlSystem::setDelayToStop(uint32_t delayToStop)
{
	this->delayToStop = delayToStop;
}

void MotionControlSystem::setPositionUncertainty(const Position & positionUncertainty)
{
	this->positionUncertainty = positionUncertainty;
}

void MotionControlSystem::getPositionUncertainty(Position & positionUncertainty)
{
	positionUncertainty = this->positionUncertainty;
}

void MotionControlSystem::getPWM(int32_t & left, int32_t & right)
{
	left = leftPWM;
	right = rightPWM;
}

void MotionControlSystem::getCurrentSpeed(int32_t & left, int32_t & right)
{
	left = currentLeftSpeed;
	right = currentRightSpeed;
}

void MotionControlSystem::getTicks(int32_t & leftTicks, int32_t & rightTicks)
{
	leftTicks = leftEncoder.read();
	rightTicks = -(rightEncoder.read());
}


bool MotionControlSystem::isMoving() const {
	return moving;
}

bool MotionControlSystem::isBlocked() const {
	return blocked;
}

void MotionControlSystem::testAsservVitesse(int speed, uint32_t duration, float kp, float ki, float kd)
{
	setLeftSpeedTunings(kp, ki, kd);
	setRightSpeedTunings(2, 0.01, 50);
	//setRightSpeedTunings(kp, ki, kd);
	enablePositionControl(false);
	enableLeftSpeedControl(true);
	enableRightSpeedControl(true);
	enablePwmControl(true);
	resetTracking();

	uint32_t beginTime = millis();
	leftSpeedSetpoint = speed;
	rightSpeedSetpoint = speed;
	moving = true;

	while (millis() - beginTime < duration)
	{}
	leftSpeedSetpoint = 0;
	rightSpeedSetpoint = 0;
	while(moving)
	{}
	enablePositionControl(true);
	stop();
	printTracking();
}

void MotionControlSystem::testAsservVitesseEtPosition(int speed, int length, float kp, float ki, float kd)
{
	setLeftSpeedTunings(kp, ki, kd);
	setRightSpeedTunings(2, 0.01, 50);
	//setRightSpeedTunings(kp, ki, kd);
	enablePositionControl(true);
	enableLeftSpeedControl(true);
	enableRightSpeedControl(true);
	enablePwmControl(true);
	resetTracking();

	Trajectory trajectory;
	UnitMove unitMove;

	unitMove.setBendRadiusMm(INFINITE_RADIUS);
	unitMove.setLengthMm(length);
	unitMove.setSpeedMm_S(speed);
	unitMove.stopAfterMove = true;

	trajectory.push_back(unitMove);

	setTrajectory(trajectory);
	while (moving)
	{
	}
	stop();
	printTracking();
}