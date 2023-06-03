#include "Adventure/Adventure.h"
#include "Enums/PlayerCommands.h"
#include "SDL_keycode.h"


void Adventure::initialize() {
   scene = &scenes[sceneIndex];
   scene->initialize();
   linkPlayerAndScene();
   terrainGenerator.rectangleRooms(scene, 30, 30);
   updateMapDisplay();
}

Scene* Adventure::getScene() {
   return scene;
}


GameLog* Adventure::getGameLog() {
   return &log;
}
PlayerManager* Adventure::getPlayerManager() {
   return &playerMan;
}


void Adventure::processCommand(PlayerCommand command, Uint16 modification) {
	if (playerMan.isAutoActing()) {
		if (command == PC_ESCAPEKEY) {
			playerMan.clearAutoAct();
			return;
		}
		playerMan.doAutoAct();
		return;
	}

	bool needToRunTurn = false;

	//process player move
   if (command < 9) {
      needToRunTurn = playerMan.processDirectionalCommand(command);
   }

   else if (command == PC_PICKUP) {
      needToRunTurn = playerMan.pickUpItem();
   }

   else if (command == PC_WAIT) {
      if ((modification&KMOD_SHIFT)==KMOD_LSHIFT || (modification&KMOD_SHIFT) == KMOD_RSHIFT) {
         if (playerMan.attemptLevelChange() == true) {
            newScene();
         }
      }
      else {
         playerMan.waitTurn();
         needToRunTurn = true;
      }
   }

   else if (command == PC_TOGGLE_LOOK || command == PC_ESCAPEKEY) {
      playerMan.updateInputState(command);
   }

   else if (command == PC_CHANGE_LEVEL) {
   }


   if (needToRunTurn) {
      scene->runTurn();
   }
}


void Adventure::runTurnIfAutoMoving() {
	if (!alreadyRanTurn && playerMan.isAutoActing()) {
      bool needToRunTurn = playerMan.doAutoAct();
      if (needToRunTurn) {
         scene->runTurn();
      }
	}
	alreadyRanTurn = false;
}


void Adventure::updateMapDisplay() {
   scene->updateMapDisplay();
}


void Adventure::hookupListeners(Listener* listener) {
   //TODO: save the first and reuse it when switching scenes
   scene->hookupListeners(listener, (Listener*) &playerMan);
}

void Adventure::newScene() {
   if (sceneIndex >= scenes.size()) {
      log.sendMessage("Last floor");
      return;
   }

   sceneIndex++;
   initialize();
}

void Adventure::linkPlayerAndScene() {
   playerMan.setSceneDependencies(scene->getTurnQueue(), scene->getMap(), scene->getEffectManager(), scene->getItemManager(), scene->getItemFactory(), scene->getActorUtils());
}
