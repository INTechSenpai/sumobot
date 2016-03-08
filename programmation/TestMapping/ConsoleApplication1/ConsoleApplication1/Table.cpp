#include "Table.h"


Table::Table()
{
};

Table::Table(uint32_t rayonRobot,uint32_t rayonBord, float xR, float yR)
{
	robotAdverse.rayon = rayonRobot;
	robotAdverse.position.x = xR;
	robotAdverse.position.y = yR;
	robotAdverse.position.orientation = 0;
	robotAdverse.position.xSpeed = 0;
	robotAdverse.position.ySpeed = 0;
	robotAdverse.obstaclePlein = true;
	bordDeTable.rayon = rayonBord;
	bordDeTable.position.x = 0;
	bordDeTable.position.y = 0;
	bordDeTable.position.orientation = 0;
	bordDeTable.position.xSpeed = 0;
	bordDeTable.position.ySpeed = 0;
	bordDeTable.obstaclePlein = false;
};

void Table::updateObstacleMap(RelativeObstacleMap donneesCapteurs, Position notrePosition)
{


	float distanceAway; // la distance entre notre robot et le robot adverse
	float angleAbsolu;//angleAbsolue est ici l'angle entre la demi droite formé par notre robot et le robot adverse et l'axe x
	float cosAngleAbsolu; //le cosinus que l'on sauvegarde car va être utiliser pour la position
	float sinAngleAbsolu; //le sinus que l'on sauvegarde car va être utiliser pour la position
	float cosAngleVitesse; //le cosinus que l'on sauvegarde car va être utiliser pour la vitesse
	float sinAngleVitesse; //le sinus que l'on sauvegarde car va être utiliser pour la vitesse
	/*
	si il y une erreur et que l on obtient un 0 sur un capteur,
	on choisit de l ignorer et de faire comme si le capteur n avait rien detecter 
	*/

	if (donneesCapteurs.avantGauche == 0)
	{
		donneesCapteurs.avantGauche = 255;
	}

	if (donneesCapteurs.gauche == 0)
	{
		donneesCapteurs.gauche = 255;
	}

	if (donneesCapteurs.arriereGauche == 0)
	{
		donneesCapteurs.arriereGauche = 255;
	}

	if (donneesCapteurs.arriereDroit == 0)
	{
		donneesCapteurs.arriereDroit = 255;
	}

	if (donneesCapteurs.droit == 0)
	{
		donneesCapteurs.droit = 255;
	}

	if (donneesCapteurs.avantDroit == 0)
	{
		donneesCapteurs.avantDroit = 255;
	}

	if (donneesCapteurs.avant == 0)
	{
		donneesCapteurs.avant = 630;
	}

	if (donneesCapteurs.arriere == 0)
	{
		donneesCapteurs.arriere = 630;
	}

	/*
	on va checker les capteurs 1 a 1 dans le sens indirect à partir de l avant sachant qu il n est possible theoriquement d avoir
	3 capteurs actifs en meme temps seulement si le robot adverse se situe juste devant ou juste derriere nous
	on va faire le cas arrier comme le cas avant a la place du cas arriere droit que l'on fera juste après 
	*/

	if (donneesCapteurs.avant != 0)
	{
		if ((donneesCapteurs.avantDroit == 255 && donneesCapteurs.avantGauche == 255 ) || (donneesCapteurs.avantDroit < 255 && donneesCapteurs.avantGauche < 255))
			//capteur avant ou les 3 capteurs avants detectent
		{
			angleAbsolu = notrePosition.orientation;
			distanceAway = donneesCapteurs.avant + 100; // 100 pour la taille des robots


			if (donneesCapteurs.speedAvant < MARGE_V) // se rapproche
			{
				robotAdverse.position.orientation = notrePosition.orientation + (float)M_PI;
			}

			if (donneesCapteurs.speedAvant > MARGE_V) // s eloigne
			{
				robotAdverse.position.orientation = notrePosition.orientation;
			}


		}
		else
		{
			if (donneesCapteurs.avantGauche < 255) //capteur avant et avant gauche detectent
			{
				angleAbsolu = notrePosition.orientation + ANGLE_CAPTEUR / 2;
				distanceAway = (donneesCapteurs.avant + donneesCapteurs.avantGauche) / 2 +100;


				if (donneesCapteurs.speedAvantGauche < MARGE_V && donneesCapteurs.speedAvant < MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
				}
				if (donneesCapteurs.speedAvantGauche > MARGE_V && donneesCapteurs.speedAvant > MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu;
				}


			}

			else // capteur avant et avant droite
			{
				angleAbsolu = notrePosition.orientation +2*(float)M_PI - ANGLE_CAPTEUR / 2;
				distanceAway = (donneesCapteurs.avant + donneesCapteurs.avantDroit) / 2 +100;


				if (donneesCapteurs.speedAvantDroit < MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
				}
				if (donneesCapteurs.speedAvantDroit > MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu;
				}


			}
		} 
	}
	else
	{
		if (donneesCapteurs.avantDroit != 0) // capteur avant droit
		{
			if (donneesCapteurs.droit != 0) // avec capteur droit
			{
				angleAbsolu = notrePosition.orientation + 3 * (float)M_PI_2 + ((float)M_PI_4 - ANGLE_CAPTEUR) / 2;
				distanceAway = (donneesCapteurs.avantDroit + donneesCapteurs.droit) / 2 + 100;


				if (donneesCapteurs.speedAvantDroit < MARGE_V && donneesCapteurs.speedDroit < MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
				}
				if (donneesCapteurs.speedAvantDroit > MARGE_V && donneesCapteurs.speedDroit > MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu;
				}


			}
			else // sans capteur droit
			{
				angleAbsolu = notrePosition.orientation + 2 * (float)M_PI - ANGLE_CAPTEUR;
				distanceAway = donneesCapteurs.avantDroit +100 ;


				if (donneesCapteurs.speedAvantDroit < MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
				}
				if (donneesCapteurs.speedAvantDroit > MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu;
				}


			}
		}
		else
		{
			if (donneesCapteurs.droit != 0) // capteur droit
			{
				if (donneesCapteurs.arriereDroit != 0) // avec capteur arriere droit
				{
					angleAbsolu = notrePosition.orientation + (float)M_PI + ((float)M_PI_4 + ANGLE_CAPTEUR) / 2;
					distanceAway = (donneesCapteurs.droit + donneesCapteurs.arriereDroit) / 2 + 100;


					if (donneesCapteurs.speedArriereDroit < MARGE_V && donneesCapteurs.speedDroit < MARGE_V)
					{
						robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
					}
					if (donneesCapteurs.speedArriereDroit > MARGE_V && donneesCapteurs.speedDroit > MARGE_V)
					{
						robotAdverse.position.orientation = angleAbsolu;
					}


				}
				else // sans capteur arriere droit
				{
					angleAbsolu = notrePosition.orientation +3* (float)M_PI_4;
					distanceAway = donneesCapteurs.droit +100;


					if (donneesCapteurs.speedAvantDroit < MARGE_V)
					{
						robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
					}
					if (donneesCapteurs.speedAvantDroit > MARGE_V)
					{
						robotAdverse.position.orientation = angleAbsolu;
					}

				}
			}

			else
			{
				if (donneesCapteurs.avant != 0) // capteur arriere
				{
					if ((donneesCapteurs.arriereDroit == 255 && donneesCapteurs.arriereGauche == 255) || (donneesCapteurs.arriereDroit < 255 && donneesCapteurs.arriereGauche < 255))
						//capteur avant ou les 3 capteurs arriere detectent
					{
						angleAbsolu = notrePosition.orientation + (float)M_PI;
						distanceAway = donneesCapteurs.arriere + 100; // 100 pour la taille des robots


						if (donneesCapteurs.speedArriere < MARGE_V) // se rapproche
						{
							robotAdverse.position.orientation = notrePosition.orientation + (float)M_PI;
						}

						if (donneesCapteurs.speedArriere > MARGE_V) // s eloigne
						{
							robotAdverse.position.orientation = notrePosition.orientation;
						}


					}
					else
					{
						if (donneesCapteurs.arriereGauche < 255) //capteur arriere et arriere gauche detectent
						{
							angleAbsolu = notrePosition.orientation + (float)M_PI - ANGLE_CAPTEUR / 2;
							distanceAway = (donneesCapteurs.arriereGauche + donneesCapteurs.arriere) / 2 + 100;


							if (donneesCapteurs.speedArriereGauche < MARGE_V && donneesCapteurs.speedArriere < MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
							}
							if (donneesCapteurs.speedArriereGauche > MARGE_V && donneesCapteurs.speedArriere > MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu;
							}


						}

						else // capteur arriere et arriere droite
						{
							angleAbsolu = notrePosition.orientation +(float)M_PI + ANGLE_CAPTEUR / 2;
							distanceAway = (donneesCapteurs.arriereDroit + donneesCapteurs.arriere) / 2 +100;


							if (donneesCapteurs.speedAvantDroit < MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
							}
							if (donneesCapteurs.speedAvantDroit > MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu;
							}


						}
					}
				}
				else
				{
					if (donneesCapteurs.arriereDroit != 0) // capteur arriere droit
					{
							angleAbsolu = notrePosition.orientation + (float)M_PI + ANGLE_CAPTEUR;
							distanceAway = donneesCapteurs.arriereDroit + 100;


							if (donneesCapteurs.speedAvantDroit < MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
							}
							if (donneesCapteurs.speedAvantDroit > MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu;
							}
					}
					else
					{
						if (donneesCapteurs.arriereGauche != 0) // capteur arriere gauche
						{
							if (donneesCapteurs.gauche != 0) // avec capteur gauche
							{
								angleAbsolu = notrePosition.orientation + (float)M_PI_2 + ((float)M_PI_4 - ANGLE_CAPTEUR) / 2;
								distanceAway = (donneesCapteurs.gauche + donneesCapteurs.arriereGauche) / 2 + 100;


								if (donneesCapteurs.speedArriereGauche < MARGE_V && donneesCapteurs.speedGauche < MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
								}
								if (donneesCapteurs.speedArriereGauche > MARGE_V && donneesCapteurs.speedGauche > MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu;
								}


							}
							else // sans capteur gauche
							{
								angleAbsolu = notrePosition.orientation + (float)M_PI -ANGLE_CAPTEUR;
								distanceAway = donneesCapteurs.arriereGauche + 100;


								if (donneesCapteurs.speedArriereGauche < MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
								}
								if (donneesCapteurs.speedArriereGauche > MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu;
								}

							}
						}
						else
						{
							if (donneesCapteurs.gauche != 0) // capteur gauche
							{
								if (donneesCapteurs.avantGauche != 0) // avec capteur avant gauche
								{
									angleAbsolu = notrePosition.orientation + ((float)M_PI_4 + ANGLE_CAPTEUR) / 2;
									distanceAway = (donneesCapteurs.gauche + donneesCapteurs.avantGauche) / 2 + 100;


									if (donneesCapteurs.speedAvantGauche < MARGE_V && donneesCapteurs.speedGauche < MARGE_V)
									{
										robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
									}
									if (donneesCapteurs.speedAvantGauche > MARGE_V && donneesCapteurs.speedGauche > MARGE_V)
									{
										robotAdverse.position.orientation = angleAbsolu;
									}


								}
								else // sans capteur avant gauche
								{
									angleAbsolu = notrePosition.orientation + (float)M_PI_2;
									distanceAway = donneesCapteurs.gauche + 100;


									if (donneesCapteurs.speedGauche < MARGE_V)
									{
										robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
									}
									if (donneesCapteurs.speedGauche > MARGE_V)
									{
										robotAdverse.position.orientation = angleAbsolu;
									}
								}
							}
							else
							{
								if (donneesCapteurs.gauche != 0) // capteur avant gauche
								{
									angleAbsolu = notrePosition.orientation + ANGLE_CAPTEUR;
									distanceAway = donneesCapteurs.avantGauche + 100;


									if (donneesCapteurs.speedAvantGauche < MARGE_V)
									{
										robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
									}
									if (donneesCapteurs.speedAvantGauche > MARGE_V)
									{
										robotAdverse.position.orientation = angleAbsolu;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	donneesCapteurs.avantGauche; // ANGLE
	donneesCapteurs.avantDroit; // 2*pi-ANGLE
	donneesCapteurs.avant; // 0
	donneesCapteurs.gauche; // pi/2
	donneesCapteurs.droit; // 3*pi/2
	donneesCapteurs.arriere; // pi
	donneesCapteurs.arriereGauche; // pi-ANGLE
	donneesCapteurs.arriereDroit; // pi+ANGLE
	donneesCapteurs.speedAvantGauche;
	donneesCapteurs.speedAvantDroit;
	donneesCapteurs.speedAvant;
	donneesCapteurs.speedGauche;
	donneesCapteurs.speedDroit;
	donneesCapteurs.speedArriere;
	donneesCapteurs.speedArriereGauche;
	donneesCapteurs.speedArriereDroit;
	donneesCapteurs.solAvantGauche;
	donneesCapteurs.solAvantDroit;
	donneesCapteurs.solArriereGauche;
	donneesCapteurs.solArriereDroit;


	sinAngleAbsolu = sinhf(angleAbsolu);
	cosAngleAbsolu = coshf(angleAbsolu);
	robotAdverse.position.x = notrePosition.x + distanceAway*cosAngleAbsolu;
	robotAdverse.position.y = notrePosition.y + distanceAway*sinAngleAbsolu;

}

/*
Calcul de angles avec arctan et tan=Opposé/adjacent
beaucoup de possibilité en fonction des signes de xSpeed et ySpeed
*/

float Table::calculOrientation(Position position)
{
	if (/*position.ySpeed >= 0 && */position.xSpeed < 0)
	{
		return position.orientation = (float)M_PI + atanhf(position.ySpeed / position.xSpeed);
	}

	if (position.ySpeed > 0 && position.xSpeed > 0)
	{
		return position.orientation = atanhf(position.ySpeed / position.xSpeed);
	}

	if (position.ySpeed < 0 && position.xSpeed > 0)
	{
		return position.orientation = 2 * (float)M_PI + atanhf(position.ySpeed / position.xSpeed);
	}

	/*if (this->position.ySpeed < 0 && this->position.xSpeed < 0)
	{
	return this->position.orientation = M_PI + atan(this->position.ySpeed / this->position.xSpeed);
	}
	Ce if reviens au même que celui du debut, d ou la simplification
	*/

	if (position.ySpeed == 0 && position.xSpeed == 0)
	{
		return position.orientation;
	}

	if (position.ySpeed == 0 && position.xSpeed < 0)
	{
		return position.orientation = (float)M_PI;
	}

	if (position.ySpeed == 0 && position.xSpeed > 0)
	{
		return position.orientation = 0;
	}

	if (position.ySpeed < 0 && position.xSpeed == 0)
	{
		return position.orientation = 3 * (float)M_PI / 2;
	}

	if (position.ySpeed > 0 && position.xSpeed == 0)
	{
		return position.orientation = (float)M_PI / 2;
	}

	return -1;

}
