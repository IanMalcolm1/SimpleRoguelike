#pragma once

#include "Adventure/Scene/Scene.h"
#include "Algorithms/Fraction.h"
#include "GraphicsThings/ColorLibrary.h"
#include "GraphicsThings/ColorPalette.h"
#include "Topography/TileCoordinates.h"


struct GeneratorTile {
   TerrainTile tile;
   ColorPalette palette;

   GeneratorTile(TerrainTile tile, ColorPalette palette) :
   tile(tile), palette(palette) {};
};


typedef std::vector<TileCoords> TileVector;

class TerrainGenerator {
   typedef void (TerrainGenerator::*SpawningFunction)(std::vector<TileCoords>&);

private:
   Scene* scene;
   LocalMap* map;
   TerrainMap* terrainMap;
   ItemFactory* itemFactory;
   ActorFactory* actorFactory;

   int mapWidth, mapHeight;

   Randomizer randomizer;
   PathingRoute distanceTester;

   TileVector placeableTiles;

   ColorLibrary colorMap;

   void setSceneAndMap(Scene* scene);

   void fillMap(GeneratorTile& terrain);
   void fillRooms(std::vector<TileVector>& rooms, GeneratorTile& terrain);
   void fillRoom(TileVector& room, GeneratorTile& terrain);

   void populatePlaceableTiles(std::vector<TileVector>& rooms);

   void drawRectangle(SDL_Rect rect, GeneratorTile& terrain);
   void drawRightAngleLine(TileCoords start, TileCoords end, GeneratorTile& terrain);
   TileVector carveDrunkard(GeneratorTile& terrain, TileCoords startTile, int lifespan);

   std::vector<SDL_Rect> makeRectangleRooms(int numRectangles, int maxSideLength, GeneratorTile& terrain);
   std::vector<TileVector> makeDrunkardRooms(GeneratorTile& terrain, Fraction fraction, int drunkLifespan);

   void placeTerrain(TileCoords location, GeneratorTile& terrain);

   void spawnCultists(SDL_Rect room);
   void spawnCaveWildlife(TileVector room);

   //only keep largest group of rooms that all connect. Fill in reset
   std::vector<TileVector> cullRooms(std::vector<TileVector>& rooms, GeneratorTile& fillTerrain);

   //accounts for tiles that may be shared between rooms
   int calcSize(std::vector<TileVector>& rooms);

   TileCoords getRandomTile(int margin = 1);

   bool thereIsAPathBetween(TileCoords start, TileCoords end);
 
   //Catacombs. Best for scene of size 100x100.
	void floor0(Scene* scene);
   //Caves. Best for scene of size 80,80.
	void floor1(Scene* scene);
   //Boss floor. Best for scene of size 50x50.
   void floor2(Scene* scene);  int distanceBetween(TileCoords start, TileCoords end);


public:
   TerrainGenerator() : scene(nullptr), map(nullptr), randomizer() {};

   void makeFloor(int floorNum, Scene* scene);
};
