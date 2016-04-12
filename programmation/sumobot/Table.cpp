#include "Table.h"

Table::Table()
{
}

ObstacleMap Table::getObstacleMap() const
{
	return obstacleMap;
}

void Table::initObstacleMap(Side side)
{
	obstacleMap.fixedInvisible.clear();
	obstacleMap.fixedVisible.clear();
	obstacleMap.movableInvisible.clear();
	obstacleMap.movableVisible.clear();
	obstacleMap.oponentRobot.clear();
	obstacleMap.toBeSpecified.clear();

	Position centre;
	Obstacle cercle(centre, CIRCLE);
	Obstacle rectangle(centre, RECTANGLE);
	Obstacle edgeOfTable(centre, EDGE_OF_TABLE);


	/*
		##### Bords de table ##### 
	*/
	// Bord principal
	centre.x = 0;
	centre.y = 1000;
	edgeOfTable.setCenter(centre);
	edgeOfTable.setXRadius(1500);
	edgeOfTable.setYRadius(1000);
	obstacleMap.fixedVisible.push_back(edgeOfTable);

	// Support � coquillages n�1
	centre.x = 1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);

	// Support � coquillages n�2
	centre.x = -1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);



	/*
		##### El�ments de d�cors fixe #####
	*/
	
	// Tasseau de limite de la dune, c�t� x>0
	centre.x = 689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Tasseau de limite de la dune, c�t� x<0
	centre.x = -689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Tasseau de limite de NOTRE bac � sable
	if(side == GREEN)
		centre.x = 312;
	else
		centre.x = -312;
	centre.y = 761;
	rectangle.setCenter(centre);
	rectangle.setXRadius(288);
	rectangle.setYRadius(11);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Bac � sable adverse (inclu le tasseau de d�limitation)
	if(side == GREEN)
		centre.x = -312;
	else
		centre.x = 312;
	centre.y = 950;
	rectangle.setCenter(centre);
	rectangle.setXRadius(288);
	rectangle.setYRadius(300);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Vitre centrale 
	centre.x = 0;
	centre.y = 950;
	rectangle.setCenter(centre);
	rectangle.setXRadius(24);
	rectangle.setYRadius(300);
	obstacleMap.fixedVisible.push_back(rectangle);

	// Serivette adverse
	if (side == GREEN)
		centre.x = -1350;
	else
		centre.x = 1350;
	centre.y = 1150;
	rectangle.setCenter(centre);
	rectangle.setXRadius(150);
	rectangle.setYRadius(250);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Dune de sable (simplifi�e)
	centre.x = 0;
	centre.y = 1940;
	rectangle.setCenter(centre);
	rectangle.setXRadius(678);
	rectangle.setYRadius(60);
	obstacleMap.fixedInvisible.push_back(rectangle);


	/*
		##### El�ments de jeu #####
	*/

	// Tas de sable face � notre serviette
	if (side == GREEN)
		centre.x = 850;
	else
		centre.x = -850;
	centre.y = 1100;
	rectangle.setCenter(centre);
	rectangle.setXRadius(60);
	rectangle.setYRadius(60);
	if (side == GREEN)
		rectangle.setOrientationToPush(PI);
	else
		rectangle.setOrientationToPush(0);
	rectangle.setPriority(KNOWN_SAND_PRIORITY);
	obstacleMap.movableVisible.push_back(rectangle);



	/*
	################################
		   ## Coquillages ##
	################################
	*/
	
	// Gestion de la sym�trie
	int mirror;
	if (side == GREEN)
		mirror = 1;
	else
		mirror = -1;

	// De notre c�t�
	centre.x = mirror * 1300;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 1300;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 800;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 3 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 800;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * 800;
	centre.y = 150;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 600;
	centre.y = 550;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 2 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 300;
	centre.y = 350;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	// Au centre
	centre.x = 0;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 2 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = 0;
	centre.y = 150;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 2 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	// C�t� adverse
	centre.x = mirror * -1300;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -1300;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -800;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -800;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 3 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * -800;
	centre.y = 150;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 1 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * -600;
	centre.y = 550;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -300;
	centre.y = 350;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

}

bool Table::updateObstacleMap(const RelativeObstacleMap & relativeObstacleMap, Position & notrePosition, Position & positionUncertainty)
{
	return false;
}
