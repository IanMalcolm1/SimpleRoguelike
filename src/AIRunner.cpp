#include "Adventure/Scene/AIRunner.h"
#include "Algorithms/FoV.h"
#include "Algorithms/Pathfinding.h"
#include "Algorithms/PathfindingRoute.h"
#include "Algorithms/PathingSpecs.h"
#include "Entities/Actors/AI.h"
#include "Entities/Actors/ActorEntity.h"
#include "Enums/TurnTime.h"
#include "Logs/DebugLogger.h"
#include <climits>


void AIRunner::initialize(LocalMap* map, ActorManager* actorMan, ActorUtils* actorUtils, AbilityManager* abilityMan) {
   this->map = map;
   this->actorMan = actorMan;
   this->actorUtils = actorUtils;
   this->abilityMan = abilityMan;
}


void AIRunner::runActorTurn(ActorEntity *actor) {
	FoV::calcActorFoV(map, actor);

   int turnTime;
	switch (actor->aiType) {
      case AITYPE_MELEE:
         turnTime = meleeAI(actor);
         break;
      case AITYPE_RANGED:
         turnTime = rangedAI(actor);
         break;
      case AITYPE_HYDRA:
         turnTime = hydraAI(actor);
         break;
	}

   actorMan->addActorToTurnQueue(actor, turnTime);
}


int AIRunner::doWander(ActorEntity* actor) {
	PathingRoute* currentRoute = actor->getCurrentRoute();

	if (currentRoute->hasNextTile()) {
		TileCoords newTile = currentRoute->getNextTile();
		if (map->isTraversibleAt(newTile)) {
			actorMan->moveActor(actor, newTile);
			currentRoute->incrementProgress();
			return actor->stats.speed;
		}
	}

	if (randomizer.flipCoin()) {
		return actor->stats.speed;
	}

	std::vector<TileCoords>* visibleTiles = actor->getVisibleTiles();
	
	int newTileIndex;
	TileCoords newLocation;

	do {
		newTileIndex = randomizer.getRandomNumber(visibleTiles->size()-1);
		newLocation = visibleTiles->at(newTileIndex);
	} while (!map->isTraversibleAt(newLocation));

   PathingSpecs specs = PathingSpecs(PATH_ROUTE, TRAV_IGNORE_NONE);
   specs.start = actor->location;
   specs.end = visibleTiles->at(newTileIndex);

	Pathfinding::calcPath(specs, map, (*actor->getCurrentRoute()));

	return actor->stats.speed;
}



int AIRunner::doApproachAndWhack(ActorEntity* actor) {
   actor->setState(AISTATE_APPROACH_AND_WHACK);
   actor->pickTarget();
   
   if (actor->location == actor->getTargetLastKnownLocation()) {
      actor->setState(AISTATE_IDLE);
      return meleeAI(actor);
   }

   if (tryMeleeAttack(actor)) {
      return actor->stats.speed;
   }

   return doApproachTarget(actor);
}
   
int AIRunner::doShootAndApproach(ActorEntity* actor) {
   actor->setState(AISTATE_SHOOT_AND_APPROACH);
   actor->pickTarget();

   if (actor->location == actor->getTargetLastKnownLocation()) {
      actor->setState(AISTATE_IDLE);
      return rangedAI(actor);
   }

   if (actor->canSeeTarget() && targetIsInShootRange(actor)) {
      actorUtils->doRangedAttack(actor, actor->getTargetLastKnownLocation());
      return actor->stats.speed;
   }

   return doApproachTarget(actor);
 }


//approach last known location of target
int AIRunner::doApproachTarget(ActorEntity* actor) {
   auto route = actor->getCurrentRoute();
   route->clear();

   PathingSpecs specs = PathingSpecs(PATH_ROUTE, TRAV_IGNORE_NONE);
   specs.start = actor->location;
   specs.end = actor->getTargetLastKnownLocation();
   specs.routeInfo.maxAStarTiles = 300; //limited in the hopes of improving performance
	Pathfinding::calcPath(specs, map, (*route));

	if (route->hasNextTile()) {
      if (map->isTraversibleAt(route->getNextTile())) {
         actorMan->moveActor(actor, route->getNextTile());
         return actor->stats.speed;
      }
   }

   //try ignoring actors (means will line up to attack target)
   specs = PathingSpecs(PATH_ROUTE, TRAV_IGNORE_ACTORS);
   specs.start = actor->location;
   specs.end = actor->getTargetLastKnownLocation();
   specs.routeInfo.maxAStarTiles = INT_MAX;
	Pathfinding::calcPath(specs, map, (*route));

	if (route->hasNextTile()) {
      if (map->isTraversibleAt(route->getNextTile())) {
         actorMan->moveActor(actor, route->getNextTile());
         return actor->stats.speed;
      }
   }

   //stuck
   return actor->stats.speed;
}

bool AIRunner::tryMeleeAttack(ActorEntity* actor) {
   actor->pickTarget();
   auto targetActor = actor->getTarget();

   if (actor->location.isAdjacentTo(targetActor->location)) {
      actorUtils->doMeleeAttack(actor, targetActor);
      return true;
   }

   return false;
}


int AIRunner::meleeAI(ActorEntity* actor) {
   if (actor->isAggroed() || actor->canSeeHostile()) {
      return doApproachAndWhack(actor);
   }

   return doWander(actor);
}

int AIRunner::rangedAI(ActorEntity* actor) {
   if (actor->isAggroed() || actor->canSeeHostile()) {
      return doShootAndApproach(actor);
   }

   return doWander(actor);
}

int AIRunner::hydraAI(ActorEntity* actor) {
   if (actor->isAggroed() || actor->canSeeHostile()) {
      //first, always try spawining a cnidas
      if (abilityMan->doAbility(actor->ability, actor)) {
         return actor->stats.speed;
      }
      tryMeleeAttack(actor);
   }

   //whether or not attack succeeded, wait a turn
   return actor->stats.speed;
}



bool AIRunner::targetIsInShootRange(ActorEntity* actor) {
   ItemEntity* weapon = actor->inventory.getMagicWeapon();

   PathingSpecs specs = PathingSpecs(PATH_LINE, TRAV_IGNORE_NONE);
   specs.lineInfo.range = ((RangedComp*)weapon->getComponent(COMPONENT_RANGED))->range;
   specs.start = actor->location;
   specs.end = actor->getTargetLastKnownLocation();

   testingRoute.clear();
   Pathfinding::calcPath(specs, map, testingRoute);

   if (testingRoute.endTile() == actor->getTargetLastKnownLocation()) {
      return true;
   }
   return false;
}
