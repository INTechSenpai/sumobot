#include "Table.h"

Table::Table()
{
};

Table::Table(uint32_t rayonRobot,uint32_t rayonBord)
{
	robotAdverse.rayon = rayonRobot;
	bordDeTable.rayon = rayonBord;
};

void updateObstacleMap(RelativeObstacleMap donneesCapteurs, Position notrePosition)
{

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

	donneesCapteurs.avantGauche;
	donneesCapteurs.avantDroit;
	donneesCapteurs.avant;// infini=630
	donneesCapteurs.gauche;// infini=255
	donneesCapteurs.droit;
	donneesCapteurs.arriere;
	donneesCapteurs.arriereGauche;
	donneesCapteurs.arriereDroit;
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
}

/*
Calcul de angles avec arctan et tan=Opposé/adjacent
beaucoup de possibilité en fonction des signes de xSpeed et ySpeed
*/

float Table::calculOrientation(Position position)
{
	if (/*position.ySpeed >= 0 && */position.xSpeed < 0)
	{
		return position.orientation = M_PI + atan(position.ySpeed / position.xSpeed);
	}

	if (position.ySpeed > 0 && position.xSpeed > 0)
	{
		return position.orientation = atan(position.ySpeed / position.xSpeed);
	}

	if (position.ySpeed < 0 && position.xSpeed > 0)
	{
		return position.orientation = 2 * M_PI + atan(position.ySpeed / position.xSpeed);
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
		return position.orientation = M_PI;
	}

	if (position.ySpeed == 0 && position.xSpeed > 0)
	{
		return position.orientation = 0;
	}

	if (position.ySpeed < 0 && position.xSpeed == 0)
	{
		return position.orientation = 3 * M_PI / 2;
	}

	if (position.ySpeed > 0 && position.xSpeed == 0)
	{
		return position.orientation = M_PI / 2;
	}

}
