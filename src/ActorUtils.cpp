#include "Entities/Actors/ActorUtils.h"
#include "Entities/Components.h"
#include "Entities/EntityDescriber.h"


void ActorUtils::initialize(ActorManager* actorManager, ItemManager* itemManager, EffectManager* effectManager, LocalMap* map) {
   this->actorMan = actorManager;
   this->itemMan = itemManager;
   this->effectMan = effectManager;
   this->map = map;
}



void ActorUtils::doAttack(ActorEntity* attacker, ItemEntity* weapon, ActorEntity* defender) {
   DamagingComp* damageComp = (DamagingComp*) weapon->getComponent(COMPONENT_DAMAGING);
   int relevantStat;

   if (weapon->hasComponent(COMPONENT_RANGED)) {
      relevantStat = attacker->stats.intelligence;
   }
   else {
      relevantStat = attacker->stats.strength;
   }
   
   std::string message = EntityDescriber::makeName(attacker);
   message.append(" attacks ");
   message.append(EntityDescriber::makeName(defender));
   message.append(" for ");

   auto damageAndMessage = actorMan->calcDamage(attacker, defender, damageComp->damage1, relevantStat);
   message.append(damageAndMessage.second);
   actorMan->sendMsgIfActorIsVisible(defender, message);

   actorMan->damageActor(defender, damageAndMessage.first);
   
   if (weapon->hasComponent(COMPONENT_EFFECT)) {
      EffectComp* effect = (EffectComp*) weapon->getComponent(COMPONENT_EFFECT);
      effectMan->attachEffect(effect->effect1, defender);
   }
}


void ActorUtils::doLineAttack(ActorEntity* attacker, ItemEntity* weapon, PathingRoute* route) {
   route->resetProgress();
   while (route->hasNextTile()) {
      TileCoords coords = route->getNextTile();
      if (map->thereIsAnActorAt(coords)) {
         doAttack(attacker, weapon, map->getActorAt(coords));
      }
      route->incrementProgress();
   }
}

void ActorUtils::doItemPickup(ItemEntity *item, ActorEntity* actor) {
   TileCoords currLocation = item->location;
   itemMan->moveItem(item, {-1,-1});

   if (item->hasComponent(COMPONENT_WIELDABLE)) {
      if (item->hasComponent(COMPONENT_RANGED)) {
         if (actor->hasDedicatedMagicWeapon()) {
            itemMan->moveItem(actor->getMagicWeapon(), currLocation);
         }
         actor->setMagicWeapon(item);
      }
      else {
         if (actor->hasDedicatedPhysicalWeapon()) {
            itemMan->moveItem(actor->getPhysicalWeapon(), currLocation);
         }
         actor->setPhysicalWeapon(item);
      }
   }

   else if (item->hasComponent(COMPONENT_CONSUMABLE)) {
      EffectComp* effectComp = (EffectComp*) item->getComponent(COMPONENT_EFFECT);
      effectMan->attachEffect(effectComp->effect1, actor);

      ConsumableComp* consumableComp = (ConsumableComp*) item->getComponent(COMPONENT_CONSUMABLE);
      consumableComp->charges--;
      if (consumableComp->charges <= 0) {
         itemMan->destroyItem(item);
      }
   }
}
