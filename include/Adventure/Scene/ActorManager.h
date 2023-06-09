#pragma once

/* Intent: Have a public method you call on an actor that gets the 
 * actor's state, and runs the corresponding logic function.
 * Logic functions will be private functions of this class */

#include "Adventure/Scene/ItemManager.h"
#include "Algorithms/Randomizer.h"
#include "Entities/Actors/ActorDescriber.h"
#include "Entities/Actors/ActorEntity.h"
#include "Entities/Components.h"
#include "Entities/Damage.h"
#include "Entities/EntityColiseum.h"
#include "Adventure/Scene/TurnQueue.h"
#include "Entities/Items/ItemFactory.h"
#include "EventListener/Blabber.h"
#include "Topography/LocalMap.h"
#include "Logs/GameLog.h"


class ActorManager : public Blabber {
private:
   EntityColiseum<ActorEntity> actorColiseum;

   ItemManager* itemMan;
   TurnQueue* turnQueue;
   LocalMap* map;
   GameLog* gameLog;
   ColorLibrary colorMap;

   Randomizer randomizer;

   std::string damageTypeNames[DAMAGE_NONE];

public:
   ActorManager() : map(nullptr), gameLog(nullptr), turnQueue(nullptr),
   actorColiseum(EntityColiseum<ActorEntity>()) {
      damageTypeNames[DAMAGE_FIRE] = " </red:fire\\>";
      damageTypeNames[DAMAGE_PHYSICAL] = "";
      damageTypeNames[DAMAGE_FROST] = " </lightblue:frost\\>";
      damageTypeNames[DAMAGE_POISON] = " </green:poison\\>";
   };

   void initialize(TurnQueue* turnQueue, LocalMap* map, GameLog* gameLog, ItemManager* itemMan);

   EntityColiseum<ActorEntity>* getColiseum();

   void destroyActor(ActorEntity* actor);
   void addActorToTurnQueue(ActorEntity* actor, int turnTime);
   void moveActor(ActorEntity* actor, TileCoords newLocation);

   //When a class needs to damage an entity, it should use calcDamage
   // to decide what to print to the GameLog, then pass the amount of
   // damage into the damageActor function
   std::pair<int, std::string> calcDamage(ActorEntity* attacker, ActorEntity* recipient, Damage damage, int relevantStat = 0);
   void damageActor(ActorEntity* actor, int damage);

   void healActor(ActorEntity* actor, int health);

   void dropItem(ActorEntity* actor, ItemEntity* item);

   void sendMsgIfActorIsVisible(ActorEntity* actor, std::string message);

   ActorEntity* getActor(int id) {
      return actorColiseum.getEntity(id);
   };
};
