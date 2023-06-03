#include "Entities/Actors/ActorFactory.h"
#include "Entities/Actors/AI.h"
#include "Entities/Actors/ActorEntity.h"
#include "Entities/Items/ItemFactory.h"
#include "Enums/TurnTime.h"


void ActorFactory::initialize(EntityColiseum<ActorEntity>* actorColiseum, TurnQueue* turnQueue, LocalMap* map, ItemFactory* itemFactory) {
   this->actorColiseum = actorColiseum;
   this->map = map;
   this->turnQueue = turnQueue;
   this->itemFactory = itemFactory;
}


void ActorFactory::registerActor(ActorEntity* actor) {
	map->setActorAt(actor->location, actor);
	turnQueue->insertActor(actor, 0);
}


ActorEntity* ActorFactory::makeEvilSmileyFace(TileCoords location) {
   ActorEntity* evilSmiley = actorColiseum->makeEntity();
   ActorStatBlock* smileyStats = &evilSmiley->stats;

   smileyStats->maxHealth = 10 + randomizer.getRandomNumber(5);
   smileyStats->health = smileyStats->maxHealth;
   smileyStats->speed = FULL_TURN_TIME + FULL_TURN_TIME/4;

   evilSmiley->description.name = "Evil Smiley Face";
   evilSmiley->description.desc = "Its insipid grin unnerves you.";

   evilSmiley->display.symbol = ASYM_SMILE_THIN;
   evilSmiley->display.symbolColor = colorMap.getColor("red");

   evilSmiley->location = location;

   evilSmiley->faction = FACTION_BAD;

   evilSmiley->setNaturalWeapon(itemFactory->getNaturalWeapon(NATWEAP_POISON_FANGS));

   evilSmiley->addAttackingSubstate(AiState(AISTATE_APPROACH_AND_WHACK));
   evilSmiley->addIdleSubstate(AiState(AISTATE_WANDERING));
   
   registerActor(evilSmiley);
   return evilSmiley;
}
