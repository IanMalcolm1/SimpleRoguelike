#pragma once

#include "Adventure/Scene/EffectManager.h"
#include "Entities/Actors/ActorEntity.h"
#include "Adventure/Scene/ItemManager.h"
#include "Entities/Items/ItemFactory.h"
#include "Enums/PlayerCommands.h"
#include "Logs/GameLog.h"
#include "Topography/LocalMap.h"
#include "Interface/InputConfirmer.h"
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
   EffectManager* effectMan;
   ItemManager* itemMan;
   ItemFactory* itemFactory;

   GameLog* gameLog;

	PathingRoute autoMoveRoute;


public:
	bool autoActing;

   PlayerManager(GameLog* gameLog);
   ~PlayerManager();

	//returns whether the turn needs to be run
	bool processDirectionalCommand(PlayerCommand command);
   void processClick(bool isRightClick);

	ActorEntity* getPlayer();
	void placePlayer(TileCoords location);

	void updateInputState(PlayerCommand command);
   void lookAtMouseTile();
	bool doAutoAct();
	void clearAutoAct();
	void startAutoMove();

   bool pickUpItem();
   void waitTurn();

   void setSceneDependencies(TurnQueue* queue, LocalMap* localMap, EffectManager* effectManager, ItemManager* itemManager, ItemFactory* itemFactory);
};
