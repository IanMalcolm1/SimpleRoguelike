#include "Algorithms/FoV.h"
#include "Entities/Actors/ActorEntity.h"
#include <stack>
#include <cmath>
#include <functional>


/* Slope Functions */

Slope::Slope(int tileDepth, int tileCol) {
	/*
	In the following diagram, the origin (0,0) is located at the 'o'.
	This means that the bottom asterisk has the coordinates (2, -0.5)
	Thus the slope between the two lines is -0.5/2, which we can represent as
	 -1/4 if we multiply by 2/2.

	This is the same as taking the coordinates of the bottom center tile's central point,
	 'c' = (2,0), and multiplying both coordinates by 2 (2*2 = 4, 0*2 = 0) before subtracting
	 1 from the modified column value ( 0 - 1 = -1) and making a fraction out of these values
	 (again -1/4)

	We always make slopes to the center of a tile's left side, so we can use this
	 algorithm alone (if we used the right side instead, we'd just add 1 to rather
	 than subtract)

	 _____ _____ _____
	|     |     |     |
	|     |  o  |     |
	|_____|_____|_____|
	|     |     |     |
	|     |     |     |
	|_____|_____|_____|
	|     |     |     |
	|     *  c  |     |
	|_____|_____|_____|

	*/
	numerator = 2*tileCol - 1;
	denominator = 2*tileDepth;
}

float Slope::multiply(int multiplier) {
	return (numerator * multiplier) / (float)denominator;
}

void Slope::manualSlope(int numerator, int denominator) {
	this->numerator = numerator;
	this->denominator = denominator;
}


/* Row Functions */

Row::Row(int depth, Slope startSlope, Slope endSlope) {
	this->depth = depth;
	this->startSlope =  startSlope;
	this->endSlope = endSlope;
}

void Row::getTilesRange(int& start, int& end) {
	start = (int)floor(startSlope.multiply(depth) + 0.5);

	end = (int)ceil(endSlope.multiply(depth) - 0.5);
}

Row Row::getNext() { return Row(depth + 1, startSlope, endSlope); }

int Row::getDepth() { return depth; }

bool Row::isSymmetric(int tileCol) {
	return tileCol >= startSlope.multiply(depth) && tileCol <= endSlope.multiply(depth);
}

void Row::setStartSlope(int tileColumn) {
	startSlope = Slope(depth, tileColumn);
}

//Note: Edited to subtract 1 from depth, because is only called directly after
//		a getNext() row call
void Row::setEndSlope(int tileColumn) {
	endSlope = Slope(depth-1, tileColumn);
}



/* Quadrant Functions */

TileCoords Quadrant::quadrantToAbsolute(int cardinal, int quadDepth, int quadCol) {
	/*
	Quadrants are treated according to the following diagram:

	. : Tile unnasociated with quadrant
	* : Tile associated with quadrant
	
	Depth:
	0     .  .  .  @  .  .  .
	1     .  .  *  *  *  .  .
	2     .  *  *  *  *  *  .
	3     *  *  *  *  *  *  *

	Col: -3 -2 -1  0  1  2  3

	Then we rotate/reflect this around to get the conversions.
	*/


	TileCoords translatedCoordinates;

	switch (cardinal) {
	case 0: //north
		translatedCoordinates.x = origin.x + quadCol;
		translatedCoordinates.y = origin.y - quadDepth;
		break;
	case 1: //south
		translatedCoordinates.x = origin.x + quadCol;
		translatedCoordinates.y = origin.y + quadDepth;
		break;
	case 2: //east
		translatedCoordinates.x = origin.x + quadDepth;
		translatedCoordinates.y = origin.y + quadCol;
		break;
	case 3: //west
		translatedCoordinates.x = origin.x - quadDepth;
		translatedCoordinates.y = origin.y + quadCol;
		break;
	default:
		translatedCoordinates.x = translatedCoordinates.y = -1;
		printf("You broke something to do with coordinate translation in field of view\n");
	}

	if (translatedCoordinates.x < 0 || translatedCoordinates.y < 0 ||
		translatedCoordinates.x >= localMap->getWidth() || translatedCoordinates.y >= localMap->getHeight()) {
		translatedCoordinates.x = translatedCoordinates.y = -1;
	}

	return translatedCoordinates;
}

bool Quadrant::isOpaque(int cardinal, int tileDepth, int tileCol) {
	TileCoords absoluteCoords = quadrantToAbsolute(cardinal, tileDepth, tileCol);
	if (absoluteCoords.x == -1 || absoluteCoords.y == -1) {
		return true;
	}
	return localMap->isOpaqueAt(absoluteCoords);
}

void Quadrant::makeVisibleToPlayer(int cardinal, int tileDepth, int tileCol) {
	TileCoords absoluteCoords = quadrantToAbsolute(cardinal, tileDepth, tileCol);
	if (absoluteCoords.x == -1 || absoluteCoords.y == -1) {
		return;
	}
   //map
	localMap->makeVisible(absoluteCoords);

   //player
	actor->addVisibleTile(absoluteCoords);
	if (localMap->thereIsAnActorAt(absoluteCoords)) {
		actor->addVisibleActor(localMap->getActorAt(absoluteCoords));
	}
}

void Quadrant::makeVisibleToActor(int cardinal, int tileDepth, int tileColumn) {
	TileCoords absoluteCoords = quadrantToAbsolute(cardinal, tileDepth, tileColumn);
	actor->addVisibleTile(absoluteCoords);
	if (localMap->thereIsAnActorAt(absoluteCoords)) {
		actor->addVisibleActor(localMap->getActorAt(absoluteCoords));
	}
}



/* Lighting Functions */

void FoV::calcFoV(LocalMap* localMap, TileCoords origin, ActorEntity* actor, void(Quadrant::*makeVisible)(int, int, int)) {
	/*
	C++ Version of https://www.albertford.com/shadowcasting/
	*/

	Quadrant quad = Quadrant(origin, localMap, actor);

	std::invoke(makeVisible, quad, 0, 0, 0);

	std::stack<Row> rowStack;

	bool previousTileIsOpaque;
	bool currentTileIsOpaque;

	int startCol, endCol;

	for (int cardinal = 0; cardinal < 4; cardinal++) {

		{
			Slope initialStart = Slope();
			initialStart.manualSlope(-1, 1);
			Slope initialEnd = Slope();
			initialEnd.manualSlope(1, 1);
         Row testRow = Row(1,initialStart, initialEnd);
			rowStack.push(Row(1, initialStart, initialEnd)); //first row
		}

		while (!rowStack.empty()) {

			Row row = rowStack.top();
			rowStack.pop();

			row.getTilesRange(startCol, endCol);

			//breaks if we don't account for first 'previous tile' in a row not actually existing yet
			currentTileIsOpaque = quad.isOpaque(cardinal, row.getDepth(), startCol);
			if (currentTileIsOpaque || row.isSymmetric(startCol)) {
				std::invoke(makeVisible, quad, cardinal, row.getDepth(), startCol);
			}
			previousTileIsOpaque = currentTileIsOpaque;


			for (int column = startCol + 1; column <= endCol; column++) {
				currentTileIsOpaque = quad.isOpaque(cardinal, row.getDepth(), column);

				if (currentTileIsOpaque || row.isSymmetric(column)) {
					std::invoke(makeVisible, quad, cardinal, row.getDepth(), column);
				}
				if (previousTileIsOpaque && !currentTileIsOpaque) {
					row.setStartSlope(column);
				}
				if (!previousTileIsOpaque && currentTileIsOpaque) {
					Row nextRow = row.getNext();
					nextRow.setEndSlope(column);
					rowStack.push(nextRow);
				}
				previousTileIsOpaque = currentTileIsOpaque;
			}
			if (!previousTileIsOpaque) {
				rowStack.push(row.getNext());
			}

		}

	}
}


void FoV::calcPlayerFoV(LocalMap* localMap, ActorEntity* player) {
   player->clearVisibilityArrays();
   localMap->resetVisibleTileDisplays();
	FoV::calcFoV(localMap, player->location, player, &Quadrant::makeVisibleToPlayer);
}


void FoV::calcActorFoV(LocalMap* localMap, ActorEntity* actor) {
	actor->clearVisibilityArrays();
	FoV::calcFoV(localMap, actor->location, actor, &Quadrant::makeVisibleToActor);
}
