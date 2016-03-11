#include "Table.h"


Table::Table()
{
};

Table::Table(uint32_t rayonRobot,uint32_t rayonBord, float xR, float yR)
{
	robotAdverse.rayon = rayonRobot;
	robotAdverse.position.x = xR;
	robotAdverse.position.y = yR;
	robotAdverse.position.orientation = 0.0;
	robotAdverse.position.xSpeed = 0.0;
	robotAdverse.position.ySpeed = 0.0;
	robotAdverse.obstaclePlein = true;
	bordDeTable.rayon = rayonBord;
	bordDeTable.position.x = 0.0;
	bordDeTable.position.y = 0.0;
	bordDeTable.position.orientation = 0.0;
	bordDeTable.position.xSpeed = 0.0;
	bordDeTable.position.ySpeed = 0.0;
	bordDeTable.obstaclePlein = false;
};

void Table::updateObstacleMap(RelativeObstacleMap donneesCapteurs, Position &notrePosition)
{


float distanceAway; // la distance entre notre robot et le robot adverse
float angleAbsolu; //angleAbsolue est ici l'angle entre la demi droite formé par notre robot et le robot adverse et l'axe x
float cosAngleAbsolu; //le cosinus que l'on sauvegarde car va être utiliser pour la position
float sinAngleAbsolu; //le sinus que l'on sauvegarde car va être utiliser pour la position
float vitesseRelative; //vitesse relative du robot adverse par rapport au notre 
bool Detection; // pour savoir si tout les capteurs ne detectent rien que l'on changera a false si c'est le cas
float rapport; // le rapport de y/x au carre pour recalculer notre positioin en cas de rencontre de bord de table
float notreX; // pour faire le calcul de notre position en cas de detection du bord de table

Detection = true;
notreX = notrePosition.x;

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


if (donneesCapteurs.solAvantDroit < LIMITE_NB && donneesCapteurs.solAvantDroit != 0)
{
	rapport = notrePosition.y / notrePosition.x;
	notrePosition.x = (bordDeTable.rayon-25) / sqrt(1 + notrePosition.y*notrePosition.y / (notrePosition.x*notrePosition.x));
	notrePosition.y = rapport*(bordDeTable.rayon - 25) / sqrt(1 + notrePosition.y*notrePosition.y / (notreX*notreX));
	notrePosition.x = notrePosition.x - D_CAPTEUR_SOL*cosf(notrePosition.orientation - ANGLE_CAPTEUR_SOL);
	notrePosition.y = notrePosition.y - D_CAPTEUR_SOL*sinf(notrePosition.orientation - ANGLE_CAPTEUR_SOL);
}
if (donneesCapteurs.solAvantGauche < LIMITE_NB && donneesCapteurs.solAvantGauche != 0)
{
	rapport = notrePosition.y / notrePosition.x;
	notrePosition.x = (bordDeTable.rayon - 25) / sqrt(1 + notrePosition.y*notrePosition.y / (notrePosition.x*notrePosition.x));
	notrePosition.y = rapport*(bordDeTable.rayon - 25) / sqrt(1 + notrePosition.y*notrePosition.y / (notreX*notreX));
	notrePosition.x = notrePosition.x - D_CAPTEUR_SOL*cosf(notrePosition.orientation + ANGLE_CAPTEUR_SOL);
	notrePosition.y = notrePosition.y - D_CAPTEUR_SOL*sinf(notrePosition.orientation + ANGLE_CAPTEUR_SOL);
}
if (donneesCapteurs.solArriereGauche < LIMITE_NB && donneesCapteurs.solArriereGauche != 0)
{
	rapport = notrePosition.y / notrePosition.x;
	notrePosition.x = (bordDeTable.rayon - 25) / sqrt(1 + notrePosition.y*notrePosition.y / (notrePosition.x*notrePosition.x));
	notrePosition.y = rapport*(bordDeTable.rayon - 25) / sqrt(1 + notrePosition.y*notrePosition.y / (notreX*notreX));
	notrePosition.x = notrePosition.x - D_CAPTEUR_SOL*cosf(notrePosition.orientation - ANGLE_CAPTEUR_SOL + M_PI);
	notrePosition.y = notrePosition.y - D_CAPTEUR_SOL*sinf(notrePosition.orientation - ANGLE_CAPTEUR_SOL + M_PI);
}
if (donneesCapteurs.solArriereDroit < LIMITE_NB && donneesCapteurs.solArriereDroit != 0)
{
	rapport = notrePosition.y / notrePosition.x;
	notrePosition.x = (bordDeTable.rayon - 25) / sqrt(1 + notrePosition.y*notrePosition.y / (notrePosition.x*notrePosition.x));
	notrePosition.y = rapport*(bordDeTable.rayon - 25) / sqrt(1 + notrePosition.y*notrePosition.y / (notreX*notreX));
	notrePosition.x = notrePosition.x - D_CAPTEUR_SOL*cosf(notrePosition.orientation + ANGLE_CAPTEUR_SOL + M_PI);
	notrePosition.y = notrePosition.y - D_CAPTEUR_SOL*sinf(notrePosition.orientation + ANGLE_CAPTEUR_SOL + M_PI);
}


/*
on va checker les capteurs 1 a 1 dans le sens indirect à partir de l avant sachant qu il n est possible theoriquement d avoir
3 capteurs actifs en meme temps seulement si le robot adverse se situe juste devant ou juste derriere nous
on va faire le cas arrier comme le cas avant a la place du cas arriere droit que l'on fera juste après 
*/

if (donneesCapteurs.avant < 630)
{
	if ((donneesCapteurs.avantDroit == 255 && donneesCapteurs.avantGauche == 255 ) || (donneesCapteurs.avantDroit < 255 && donneesCapteurs.avantGauche < 255))
		//capteur avant ou les 3 capteurs avants detectent
	{
		angleAbsolu = notrePosition.orientation;
		distanceAway = (float)donneesCapteurs.avant + (float)TAILLES_ROBOTS; // (float)TAILLES_ROBOTS pour la taille des robots


		if (donneesCapteurs.speedAvant < (int32_t)MARGE_V) // se rapproche
		{
			robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
			vitesseRelative = (float)donneesCapteurs.speedAvant;
		}
		else
		{
			vitesseRelative = 0.0;
		}

		if (donneesCapteurs.speedAvant > (int32_t)MARGE_V) // s eloigne
		{
			robotAdverse.position.orientation = angleAbsolu;
			vitesseRelative = (float)donneesCapteurs.speedAvant;

		}


	}
	else
	{
		if (donneesCapteurs.avantGauche < 255) //capteur avant et avant gauche detectent
		{
			angleAbsolu = notrePosition.orientation + (float)ANGLE_CAPTEUR / 2;
			distanceAway = ((float)donneesCapteurs.avant + (float)donneesCapteurs.avantGauche) / 2 +(float)TAILLES_ROBOTS;


			if (donneesCapteurs.speedAvantGauche < (int32_t)MARGE_V && donneesCapteurs.speedAvant < (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
				vitesseRelative = ((float)donneesCapteurs.speedAvant+ (float)donneesCapteurs.speedAvantGauche)/2;

			}
			else
			{
				vitesseRelative = 0.0;
			}
			if (donneesCapteurs.speedAvantGauche > (int32_t)MARGE_V && donneesCapteurs.speedAvant > (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu;
				vitesseRelative = ((float)donneesCapteurs.speedAvant + (float)donneesCapteurs.speedAvantGauche) / 2;
			}

		}

		else // capteur avant et avant droite
		{
			angleAbsolu = notrePosition.orientation + 2*(float)M_PI - (float)ANGLE_CAPTEUR / 2;
			distanceAway = ((float)donneesCapteurs.avant + (float)donneesCapteurs.avantDroit) / 2 +(float)TAILLES_ROBOTS;


			if (donneesCapteurs.speedAvantDroit < (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
				vitesseRelative = ((float)donneesCapteurs.speedAvant + (float)donneesCapteurs.speedAvantDroit) / 2;
			}
			else
			{
				vitesseRelative = 0.0;
			}
			if (donneesCapteurs.speedAvantDroit > (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu;
				vitesseRelative = ((float)donneesCapteurs.speedAvant + (float)donneesCapteurs.speedAvantDroit) / 2;
			}



		}
	} 
}
else
{
if (donneesCapteurs.avantDroit < 255) // capteur avant droit
{
	if (donneesCapteurs.droit < 255) // avec capteur droit
	{
		angleAbsolu = notrePosition.orientation + 3 * (float)M_PI_2 + ((float)M_PI_2 - (float)ANGLE_CAPTEUR) / 2;
		distanceAway = ((float)donneesCapteurs.avantDroit + (float)donneesCapteurs.droit) / 2 + (float)TAILLES_ROBOTS;


			if (donneesCapteurs.speedAvantDroit < (int32_t)MARGE_V && donneesCapteurs.speedDroit < (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
				vitesseRelative = ((float)donneesCapteurs.speedAvantDroit + (float)donneesCapteurs.speedDroit) /2;
			}
			else
			{
				vitesseRelative = 0.0;
			}
			if (donneesCapteurs.speedAvantDroit > (int32_t)MARGE_V && donneesCapteurs.speedDroit > (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu;
				vitesseRelative = ((float)donneesCapteurs.speedAvantDroit + (float)donneesCapteurs.speedDroit) / 2;
			}



		}
		else // sans capteur droit
		{
			angleAbsolu = notrePosition.orientation + 2 * (float)M_PI - (float)ANGLE_CAPTEUR;
			distanceAway = (float)donneesCapteurs.avantDroit +(float)TAILLES_ROBOTS ;


			if (donneesCapteurs.speedAvantDroit < (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
				vitesseRelative = (float)donneesCapteurs.speedAvantDroit ;
			}
			else
			{
				vitesseRelative = 0.0;
			}
			if (donneesCapteurs.speedAvantDroit > (int32_t)MARGE_V)
			{
				robotAdverse.position.orientation = angleAbsolu;
				vitesseRelative = (float)donneesCapteurs.speedAvantDroit ;
			}



		}
	}
	else
	{
		if (donneesCapteurs.droit < 255) // capteur droit
		{
			if (donneesCapteurs.arriereDroit < 255) // avec capteur arriere droit
			{
				angleAbsolu = notrePosition.orientation + (float)M_PI + ((float)M_PI_2 + (float)ANGLE_CAPTEUR) / 2;
				distanceAway = ((float)donneesCapteurs.droit + (float)donneesCapteurs.arriereDroit) / 2 + (float)TAILLES_ROBOTS;


				if (donneesCapteurs.speedArriereDroit < (int32_t)MARGE_V && donneesCapteurs.speedDroit < (int32_t)MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
					vitesseRelative = ((float)donneesCapteurs.speedDroit + (float)donneesCapteurs.speedArriereDroit) / 2;
				}
				else
				{
					vitesseRelative = 0.0;
				}
				if (donneesCapteurs.speedArriereDroit > (int32_t)MARGE_V && donneesCapteurs.speedDroit > (int32_t)MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu;
					vitesseRelative = ((float)donneesCapteurs.speedDroit + (float)donneesCapteurs.speedArriereDroit) / 2;
				}
				


			}
			else // sans capteur arriere droit
			{
				angleAbsolu = notrePosition.orientation +3* (float)M_PI_2;
				distanceAway = (float)donneesCapteurs.droit +(float)TAILLES_ROBOTS;


				if (donneesCapteurs.speedDroit < (int32_t)MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
					vitesseRelative = (float)donneesCapteurs.speedDroit;
				}
				else
				{
					vitesseRelative = 0.0;
				}
				if (donneesCapteurs.speedDroit > (int32_t)MARGE_V)
				{
					robotAdverse.position.orientation = angleAbsolu;
					vitesseRelative = (float)donneesCapteurs.speedDroit;
				}

			}
		}

		else
		{
			if (donneesCapteurs.arriere < 630) // capteur arriere
			{
				if ((donneesCapteurs.arriereDroit == 255 && donneesCapteurs.arriereGauche == 255) || (donneesCapteurs.arriereDroit < 255 && donneesCapteurs.arriereGauche < 255))
					//capteur avant ou les 3 capteurs arriere detectent
				{
					angleAbsolu = notrePosition.orientation + (float)M_PI;
					distanceAway = (float)donneesCapteurs.arriere + (float)TAILLES_ROBOTS; // (float)TAILLES_ROBOTS pour la taille des robots


					if (donneesCapteurs.speedArriere < (int32_t)MARGE_V) // se rapproche
					{
						robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
						vitesseRelative = (float)donneesCapteurs.speedArriere;
					}
					else
					{
						vitesseRelative = 0.0;
					}

					if (donneesCapteurs.speedArriere > (int32_t)MARGE_V) // s eloigne
					{
						robotAdverse.position.orientation = angleAbsolu;
						vitesseRelative = (float)donneesCapteurs.speedArriere;
					}


				}
				else
				{
					if (donneesCapteurs.arriereGauche < 255) //capteur arriere et arriere gauche detectent
					{
						angleAbsolu = notrePosition.orientation + (float)M_PI - (float)ANGLE_CAPTEUR / 2;
						distanceAway = ((float)donneesCapteurs.arriereGauche + (float)donneesCapteurs.arriere) / 2 + (float)TAILLES_ROBOTS;


						if (donneesCapteurs.speedArriereGauche < (int32_t)MARGE_V && donneesCapteurs.speedArriere < (int32_t)MARGE_V)
						{
							robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
							vitesseRelative = ((float)donneesCapteurs.speedArriere+ (float)donneesCapteurs.speedArriereGauche)/2;
						}
						else
						{
							vitesseRelative = 0.0;
						}
						if (donneesCapteurs.speedArriereGauche > (int32_t)MARGE_V && donneesCapteurs.speedArriere > (int32_t)MARGE_V)
						{
							robotAdverse.position.orientation = angleAbsolu;
							vitesseRelative = ((float)donneesCapteurs.speedArriere + (float)donneesCapteurs.speedArriereGauche) / 2;
						}


					}

					else // capteur arriere et arriere droite
					{
						angleAbsolu = notrePosition.orientation +(float)M_PI + (float)ANGLE_CAPTEUR / 2;
						distanceAway = ((float)donneesCapteurs.arriereDroit + (float)donneesCapteurs.arriere) / 2 +(float)TAILLES_ROBOTS;


						if (donneesCapteurs.speedAvantDroit < (int32_t)MARGE_V)
						{
							robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
							vitesseRelative = ((float)donneesCapteurs.speedArriere + (float)donneesCapteurs.speedArriereDroit) / 2;
						}
						else
						{
							vitesseRelative = 0.0;
						}
						if (donneesCapteurs.speedAvantDroit > (int32_t)MARGE_V)
						{
							robotAdverse.position.orientation = angleAbsolu;
							vitesseRelative = ((float)donneesCapteurs.speedArriere + (float)donneesCapteurs.speedArriereDroit) / 2;
						}

					}
				}
			}
			else
			{
				if (donneesCapteurs.arriereDroit < 255) // capteur arriere droit
				{
						angleAbsolu = notrePosition.orientation + (float)M_PI + (float)ANGLE_CAPTEUR;
						distanceAway = (float)donneesCapteurs.arriereDroit + (float)TAILLES_ROBOTS;


						if (donneesCapteurs.speedAvantDroit < (int32_t)MARGE_V)
						{
							robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
							vitesseRelative = (float)donneesCapteurs.speedArriereDroit;
						}
						else
						{
							vitesseRelative = 0.0;
						}
						if (donneesCapteurs.speedAvantDroit > (int32_t)MARGE_V)
						{
							robotAdverse.position.orientation = angleAbsolu;
							vitesseRelative = (float)donneesCapteurs.speedArriereDroit;
						}

				}
				else
				{
					if (donneesCapteurs.arriereGauche < 255) // capteur arriere gauche
					{
						if (donneesCapteurs.gauche < 255) // avec capteur gauche
						{
							angleAbsolu = notrePosition.orientation + (float)M_PI_2 + ((float)M_PI_2 - (float)ANGLE_CAPTEUR) / 2;
							distanceAway = ((float)donneesCapteurs.gauche + (float)donneesCapteurs.arriereGauche) / 2 + (float)TAILLES_ROBOTS;


							if (donneesCapteurs.speedArriereGauche < (int32_t)MARGE_V && donneesCapteurs.speedGauche < (int32_t)MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
								vitesseRelative = ((float)donneesCapteurs.speedGauche + (float)donneesCapteurs.speedArriereGauche) / 2;
							}
							else
							{
								vitesseRelative = 0.0;
							}
							if (donneesCapteurs.speedArriereGauche > (int32_t)MARGE_V && donneesCapteurs.speedGauche > (int32_t)MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu;
								vitesseRelative = ((float)donneesCapteurs.speedGauche + (float)donneesCapteurs.speedArriereGauche) / 2;
							}



						}
						else // sans capteur gauche
						{
							angleAbsolu = notrePosition.orientation + (float)M_PI -(float)ANGLE_CAPTEUR;
							distanceAway = (float)donneesCapteurs.arriereGauche + (float)TAILLES_ROBOTS;


							if (donneesCapteurs.speedArriereGauche < (int32_t)MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
								vitesseRelative = (float)donneesCapteurs.speedArriereGauche;
							}
							else
							{
								vitesseRelative = 0.0;
							}
							if (donneesCapteurs.speedArriereGauche > (int32_t)MARGE_V)
							{
								robotAdverse.position.orientation = angleAbsolu;
								vitesseRelative = (float)donneesCapteurs.speedArriereGauche;
							}


						}
					}
					else
					{
						if (donneesCapteurs.gauche < 255) // capteur gauche
						{
							if (donneesCapteurs.avantGauche < 255) // avec capteur avant gauche
							{
								angleAbsolu = notrePosition.orientation + ((float)M_PI_2 + (float)ANGLE_CAPTEUR) / 2;
								distanceAway = ((float)donneesCapteurs.gauche + (float)donneesCapteurs.avantGauche) / 2 + (float)TAILLES_ROBOTS;


								if (donneesCapteurs.speedAvantGauche < (int32_t)MARGE_V && donneesCapteurs.speedGauche < (int32_t)MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu - (float)M_PI;
									vitesseRelative = ((float)donneesCapteurs.speedAvantGauche + (float)donneesCapteurs.speedGauche) / 2;
								}
								else
								{
									vitesseRelative = 0.0;
								}
								if (donneesCapteurs.speedAvantGauche > (int32_t)MARGE_V && donneesCapteurs.speedGauche > (int32_t)MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu;
									vitesseRelative = ((float)donneesCapteurs.speedAvantGauche + (float)donneesCapteurs.speedGauche) / 2;
								}


							}
							else // sans capteur avant gauche
							{
								angleAbsolu = notrePosition.orientation + (float)M_PI_2;
								distanceAway = (float)donneesCapteurs.gauche + (float)TAILLES_ROBOTS;


								if (donneesCapteurs.speedGauche < (int32_t)MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
									vitesseRelative = (float)donneesCapteurs.speedGauche;
								}
								else
								{
									vitesseRelative = 0.0;
								}
								if (donneesCapteurs.speedGauche > (int32_t)MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu;
									vitesseRelative = (float)donneesCapteurs.speedGauche;
								}

							}
						}
						else
						{
							if (donneesCapteurs.avantGauche < 255) // capteur avant gauche
							{
								angleAbsolu = notrePosition.orientation + (float)ANGLE_CAPTEUR;
								distanceAway = (float)donneesCapteurs.avantGauche + (float)TAILLES_ROBOTS;


								if (donneesCapteurs.speedAvantGauche < (int32_t)MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu + (float)M_PI;
									vitesseRelative = (float)donneesCapteurs.speedAvantGauche;
								}
								else
								{
									vitesseRelative = 0.0;
								}
								if (donneesCapteurs.speedAvantGauche >(int32_t)MARGE_V)
								{
									robotAdverse.position.orientation = angleAbsolu;
									vitesseRelative = (float)donneesCapteurs.speedAvantGauche;
								}
							}
							else
							{
								Detection = false;
							}
						}
					}
				}
			}
		}
	}
}

if (Detection)
{
	sinAngleAbsolu = sinf(angleAbsolu);
	cosAngleAbsolu = cosf(angleAbsolu);
	robotAdverse.position.x = notrePosition.x + distanceAway*cosAngleAbsolu;
	robotAdverse.position.y = notrePosition.y + distanceAway*sinAngleAbsolu;
	robotAdverse.position.xSpeed = notrePosition.xSpeed + vitesseRelative*cosAngleAbsolu;
	robotAdverse.position.ySpeed = notrePosition.ySpeed + vitesseRelative*sinAngleAbsolu;
}
else
{
	robotAdverse.position.x = 1000;
	robotAdverse.position.y = 1000;
	vitesseRelative = 0.0;
}
}

/*
Calcul de angles avec arctan et tan=Opposé/adjacent
beaucoup de possibilité en fonction des signes de xSpeed et ySpeed
*/

/*float Table::calculOrientation(Position position)
{
	if (/*position.ySpeed >= 0 && *//*position.xSpeed < 0)
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
	*//*

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
		return position.orientation = 0.0;
	}

	if (position.ySpeed < 0 && position.xSpeed == 0)
	{
		return position.orientation = 3 * (float)M_PI / 2;
	}

	if (position.ySpeed > 0 && position.xSpeed == 0)
	{
		return position.orientation = (float)M_PI / 2;
	}

	return 1000;

}*/

uint32_t Table::determinerConditionInitiale(RelativeObstacleMap donneesCapteurs)
{
	static uint32_t departGauche=0;
	static uint32_t departDroit=0;
	static uint32_t departArriere=0;

	if (donneesCapteurs.gauche < 255 && donneesCapteurs.gauche != 0)
	{
		departGauche++;
	}
	if (donneesCapteurs.droit < 255 && donneesCapteurs.droit != 0)
	{
		departDroit++;
	}
	if (donneesCapteurs.arriere < 255 && donneesCapteurs.arriere != 0)
	{
		departArriere++;
	}
	if (departArriere >= departGauche && departArriere >= departDroit)
	{
		return 1;
	}
	if (departDroit >= departGauche)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

ObstacleCercle Table::getBordDeTable()
{
	return bordDeTable;
}
ObstacleCercle Table::getRobotAdverse()
{
	return robotAdverse;
}

