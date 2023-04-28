#pragma once

#include "Entities/ActorEntity.h"
#include "Enums/PlayerCommands.h"
#include "Topography/LocalMap.h"
#include "Interface/UIScreens/InputConfirmer.h"
#include "Adventure/Scene/TurnQueue.h"


enum PlayerInputState {
	PLAYER_INPUT_MOVE,
	PLAYER_INPUT_LOOK,
	PLAYER_INPUT_SELECT
};

class PlayerManager {
private:
	ActorEntity* player;
   void* playerArena;

	PlayerInputState inputState;

	LocalMap* map;
   TurnQueue* turnQueue;

	InputConfirmer* confirmer;

	PathingRoute autoMoveRoute;


public:
	bool autoActing;

   PlayerManager();
   ~PlayerManager();

	//returns whether the turn needs to be run
	bool processDirectionalCommand(PlayerCommand command);

	ActorEntity* getPlayer();
	void placePlayer(TileCoords location);

	void updateInputState(PlayerCommand command);
	bool doAutoAct();
	void clearAutoAct();
	void startAutoMove(PathingRoute route);

   void setSceneDependencies(TurnQueue* queue, LocalMap* localMap, InputConfirmer* adventureConfirmer);
};