// 
// 
// 

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

	leftSpeedPID.setOutputLimits(-255, 255);
	rightSpeedPID.setOutputLimits(-255, 255);

	movingSpeed = 0;

	delayToStop = 100;
	toleranceTranslation = 50;
	toleranceRotation = 50;

	translationPID.setTunings(12, 0, 0);
	rotationPID.setTunings(15, 0, 0);
	leftSpeedPID.setTunings(10, 0, 0);
	rightSpeedPID.setTunings(10, 0, 0);
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
	currentTrajectory = newTrajectory;
	moving = true;
	blocked = false;
	currentMove = -1; // L'appel à nextMove() incrémentera currentMove et il vaudra bien 0
	nextMove();
}

void MotionControlSystem::nextMove()
{
	currentMove++;
	if (currentMove < currentTrajectory.size())
	{
		if (currentTrajectory[currentMove].bendRadius == 0)
		{
			rotationSetpoint = currentAngle + currentTrajectory[currentMove].length;
		}
		else
		{
			translationSetpoint = currentDistance + currentTrajectory[currentMove].length;
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

	currentLeftSpeed = (leftTicks - previousLeftTicks) * 2000; // (nb-de-tick-passés)*(freq_asserv) (ticks/sec)
	currentRightSpeed = (rightTicks - previousRightTicks) * 2000;

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

			if (currentTrajectory[currentMove].bendRadius == 0)
			{// Cas d'un mouvement purement rotatif
				if ( (currentTrajectory[currentMove].length >= 0 && currentAngle >= rotationSetpoint)
				  || (currentTrajectory[currentMove].length <  0 && currentAngle <= rotationSetpoint) )
				{// Rotation terminée
					nextMove();
				}
			}
			else
			{// Cas d'une trajectoire courbe standard
				if ( (currentTrajectory[currentMove].length >= 0 && currentDistance >= translationSetpoint)
				  || (currentTrajectory[currentMove].length <  0 && currentDistance <= translationSetpoint) )
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
				int32_t maxSpeed = currentTrajectory[currentMove].speed;


				/* Le mouvement élémentaire courant existe est n'est pas terminé, et il s'agit du dernier */
				if (currentMove == currentTrajectory.size() - 1)
				{
					if (currentTrajectory[currentMove].bendRadius == 0)
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
					movingSpeed = maxSpeed;
				}

				/* Calcul des vitesses des deux moteurs à partir de movingSpeed et de bendRadius */
				int16_t radius = currentTrajectory[currentMove].bendRadius;
				if (radius == 0)
				{
					leftSpeedSetpoint = -movingSpeed;
					rightSpeedSetpoint = movingSpeed;
				}
				else if (radius > 0)
				{
					leftSpeedSetpoint = ((double)radius - ROBOT_RADIUS / TICK_TO_MM)*(double)movingSpeed / (double)radius;
					rightSpeedSetpoint = ((double)radius + ROBOT_RADIUS / TICK_TO_MM)*(double)movingSpeed / (double)radius;
				}
				else
				{
					leftSpeedSetpoint = ((double)radius + ROBOT_RADIUS / TICK_TO_MM)*(double)movingSpeed / (double)radius;
					rightSpeedSetpoint = ((double)radius - ROBOT_RADIUS / TICK_TO_MM)*(double)movingSpeed / (double)radius;
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

bool MotionControlSystem::isPhysicallyStopped() {
	return (translationPID.getDerivativeError() == 0) && (rotationPID.getDerivativeError() == 0);
}

void MotionControlSystem::manageStop()
{
	static uint32_t time = 0;

	if (isPhysicallyStopped() && moving)
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
				}
				else if (
							(currentMove == currentTrajectory.size() - 1) && 
							(
								(currentTrajectory[currentMove].bendRadius != 0 && ABS(currentDistance - translationSetpoint) <= toleranceTranslation) || 
								(currentTrajectory[currentMove].bendRadius == 0 && ABS(currentAngle - rotationSetpoint) <= toleranceRotation)
							)
						)
				{// Si on est suffisament proche de la fin de trajectoire
					blocked = false;
				}
				else
				{// Sinon : il d'agit d'un blocage physique
					blocked = true;
					Serial.print("Pos : ");
					Serial.println(currentDistance);
					Serial.print("Aim : ");
					Serial.println(translationSetpoint);
				}
				stop();
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

void MotionControlSystem::updatePosition() {
	static volatile int32_t lastDistance = 0;
	static volatile int32_t lastAngle = 0;

	float deltaDistanceMm = (currentDistance - lastDistance) * TICK_TO_MM;
	lastDistance = currentDistance;

	float deltaAngleRadian = (currentAngle - lastAngle) * TICK_TO_RADIAN;
	lastAngle = currentAngle;

	currentPosition.orientation += deltaAngleRadian;

	currentPosition.x += (deltaDistanceMm * cos(currentPosition.orientation));
	currentPosition.y += (deltaDistanceMm * sin(currentPosition.orientation));
}


void MotionControlSystem::stop() {
	translationSetpoint = currentDistance;
	rotationSetpoint = currentAngle;
	leftSpeedSetpoint = 0;
	rightSpeedSetpoint = 0;

	currentTrajectory.clear();
	currentMove = 0;

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
void MotionControlSystem::setPosition(Position & newPosition)
{
	currentPosition = newPosition;
}

Position & MotionControlSystem::getPosition()
{
	return currentPosition;
}


void MotionControlSystem::resetPosition()
{
	currentPosition.x = 0;
	currentPosition.y = 0;
	currentPosition.orientation = 0;
	currentPosition.xSpeed = 0;
	currentPosition.ySpeed = 0;
	stop();
}

void MotionControlSystem::setDelayToStop(uint32_t delayToStop)
{
	this->delayToStop = delayToStop;
}

bool MotionControlSystem::isMoving() const {
	return moving;
}

bool MotionControlSystem::isBlocked() const {
	return blocked;
}
