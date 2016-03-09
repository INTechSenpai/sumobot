#include "stdio.h"
#include "Table.h"

void testConstructeurTable()
	{
	Position notrePosition;
	notrePosition.x = 6;
	notrePosition.y = 8;
	notrePosition.orientation = (float)M_PI_4;
	Table table(50,800,-12,-16);
	printf("Constructeur :\n rayon robot adverse : %u \n rayon du bord de la table : %u \n position x robotAdverse : %f \n position y robotAdverse : %f \n",
		table.getRobotAdverse().rayon, table.getBordDeTable().rayon, table.getRobotAdverse().position.x, table.getRobotAdverse().position.y);
	}

void testUpdateRobotAdverse()
{
	Position notrePosition;
	notrePosition.x = 6;
	notrePosition.y = 8;
	notrePosition.orientation = (float)M_PI_4-(float)M_PI_2;
	Table table(50, 800, -12, -16);
	RelativeObstacleMap donneesCapteurs;
	donneesCapteurs.avantGauche = 0;
	donneesCapteurs.avantDroit = 0;
	donneesCapteurs.avant = 0;
	donneesCapteurs.gauche = 100;
	donneesCapteurs.droit = 0;
	donneesCapteurs.arriere = 0;
	donneesCapteurs.arriereGauche = 0;
	donneesCapteurs.arriereDroit = 0;
	donneesCapteurs.speedAvantGauche = 0;
	donneesCapteurs.speedAvantDroit = 0;
	donneesCapteurs.speedAvant = 0;
	donneesCapteurs.speedGauche = 0;
	donneesCapteurs.speedDroit = 0;
	donneesCapteurs.speedArriere = 0;
	donneesCapteurs.speedArriereGauche = 0;
	donneesCapteurs.speedArriereDroit = 0;
	donneesCapteurs.solAvantGauche = 0;
	donneesCapteurs.solAvantDroit = 0;
	donneesCapteurs.solArriereGauche = 0;
	donneesCapteurs.solArriereDroit = 0;
	table.updateObstacleMap(donneesCapteurs, notrePosition);

	printf(" position x robotAdverse theorique : %f \n position x robotAdverse : %f \n position y robotAdverse theorique : %f \n position y robotAdverse : %f \n",
		notrePosition.x + (TAILLES_ROBOTS + 100)*sqrt(2) / 2, table.getRobotAdverse().position.x, notrePosition.y + (TAILLES_ROBOTS + 100)*sqrt(2) / 2, table.getRobotAdverse().position.y);

}

int main()
{
	testUpdateRobotAdverse();
	return 0;
}

