#include "Table.h"

Table::Table()
{
}

void Table::initObstacleMap(ObstacleMap & obstacleMap, uint8_t shellConfig)
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

	// Support à coquillages n°1
	centre.x = 1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);

	// Support à coquillages n°2
	centre.x = -1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);



	/*
		##### Eléments de décors fixe #####
	*/
	
	// Tasseau de limite de la dune, côté x>0
	centre.x = 689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);


	// Tasseau de limite de la dune, côté x<0
	centre.x = -689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Tasseau de limite de notre bac à sable
	centre.x = 312;
	centre.y = 761;
	rectangle.setCenter(centre);
	rectangle.setXRadius(288);
	rectangle.setYRadius(11);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Bac à sable adverse (inclu le tasseau de délimitation)
	centre.x = -312;
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
	centre.x = -1350;
	centre.y = 1150;
	rectangle.setCenter(centre);
	rectangle.setXRadius(150);
	rectangle.setYRadius(250);
	obstacleMap.fixedInvisible.push_back(rectangle);



	/*
		##### Eléments de jeu #####
	*/



}

bool Table::updateObstacleMap(ObstacleMap obstacleMap, const RelativeObstacleMap & relativeObstacleMap, Position & notrePosition)
{
	return false;
}
