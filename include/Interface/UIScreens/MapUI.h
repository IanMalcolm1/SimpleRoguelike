#pragma once

#include <memory>
#include <SDL.h>
#include "Topography/LocalMap.h"


struct MapRenderingData {
	SDL_Rect srcRect; //The part of the map texture to copy from
	SDL_Rect dstRect; //The part of the map viewport to paste on to

	TileCoords startTile, endTile;

	int scale; //Scale mulitiplier
	int scaleSize; //Actual number of pixels to a side of a tile

	MapRenderingData() :  srcRect({ 0,0,0,0 }), dstRect({ 0,0,0,0 }),
		scale(4), scaleSize(scale * 8), startTile(), endTile() {};
};


class MapUI {
private:
	LocalMap* map;
	MapDisplay* mapDisplay; //get from map. holds display information for tiles
	MapRenderingData rData; //keeps track of which tiles have been rendered where

	SDL_Texture* mapTexture;

	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	SDL_Rect mainViewport;

	void calculateMapRenderingData(const SDL_Rect& viewport);
	void calcDataForAxis(const SDL_Rect& viewport, char axis); //as in x/y axis

	void renderTile(int index, SDL_Rect dstrect);
   void renderReticle(int index, SDL_Rect dstrect);

   void setMaps(LocalMap* map);
   void makeTexture();

public:
	bool hidden;

	MapUI() : map(NULL), renderer(NULL), spritesheet(NULL),	mapTexture(NULL),
		mapDisplay(NULL),rData(MapRenderingData()),
		mainViewport({ 0,0,0,0 }), hidden(false) {}

	void initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet);
   void cleanUp();

	void render(LocalMap* map, const SDL_Rect& viewport);

	void processScroll(int x, int y, int offset); //changes scale
	void processCursorLocation(int x, int y);
	TileCoords findMapTileFromScreenCoords(int x, int y);

	void processWindowSizeChange();
};
