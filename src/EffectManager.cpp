#include "Adventure/Scene/EffectManager.h"
#include "Entities/Actors/ActorEntity.h"
#include "Entities/Actors/ActorStatBlock.h"
#include "Entities/Effects/Effect.h"
#include "Logs/DebugLogger.h"


EffectDescriber* EffectManager::getEffectDescriber() {
   return &describer;
}

void EffectManager::attachEffect(Effect effect, ActorEntity* effectee) {
   switch (effect.classification) {
      case EFFECT_CLASS_TIMED:
         addEffectToActorList(effect, effectee);
         applyEffect(effect, effectee);
         turnQueue->insertEffect(effect, effectee, effect.timedInfo.duration);
         break;
      case EFFECT_CLASS_DOT:
         addEffectToActorList(effect, effectee);
         updateDoTEffect(effect, effectee);
         break;
      default:
         break;
   }

   if (effect.description != EFFECT_DESC_NONE) {
      std::string msg = effectee->description.name;
      msg.append(describer.getMessage(effect.description));
      actorMan->sendMsgIfActorIsVisible(effectee, msg);
   }
}

void EffectManager::applyEffect(Effect effect, ActorEntity* effectee) {
   switch (effect.type) {
      case EFFECT_TYPE_STAT_MOD:
         applyStatModification(effect, effectee);
         break;
      case EFFECT_TYPE_DAMAGE:
         applyDamageEffect(effect, effectee);
         break;
      default:
         DebugLogger::log("Effect has no type.");
        return; 
   }
}

void EffectManager::addEffectToActorList(Effect effect, ActorEntity* effectee) {
   auto actorEffect = effectee->activeEffects.getEffect(effect);
   if (actorEffect == nullptr) {
      effectee->activeEffects.addEffect(effect);
   }
   else {
      if (actorEffect->first.maxStacks > actorEffect->second) {
         actorEffect->second++;
      }
      else {
         //TODO: remove oldest effect, not just the next one in line
         turnQueue->removeEffect(effect, effectee);
      }
   }
}


void EffectManager::attachTimedEffect(Effect effect, ActorEntity* effectee) {
   turnQueue->insertEffect(effect, effectee, effect.timedInfo.duration);
}

void EffectManager::updateEffect(Effect effect, ActorEntity* effectee) {
   applyEffect(effect, effectee);

   switch (effect.classification) {
      case EFFECT_CLASS_TIMED:
         endTimedEffect(effect, effectee);
         break;
      case EFFECT_CLASS_DOT:
         updateDoTEffect(effect, effectee);
         break;
      default:
         break;
   }
}



void EffectManager::applyDamageEffect(Effect effect, ActorEntity* effectee) {
   auto damageAndMsg = actorMan->calcDamage(effectee, effect.damageInfo.damage);

   std::string msg = effectee->description.name + " takes " + damageAndMsg.second;
   actorMan->sendMsgIfActorIsVisible(effectee, msg);

   actorMan->damageActor(effectee, damageAndMsg.first);
}


void EffectManager::applyStatModification(Effect effect, ActorEntity* effectee) {
   switch (effect.statModInfo.stat) {
      case STAT_MAX_HEALTH:
         effectee->stats.maxHealth += effect.statModInfo.modification;
         break;
      case STAT_STRENGTH:
         effectee->stats.strength += effect.statModInfo.modification;
         break;
      case STAT_INTELLIGENCE:
         effectee->stats.intelligence += effect.statModInfo.modification;
         break;
      case STAT_MOVE_SPEED:
         effectee->stats.baseMoveSpeed += effect.statModInfo.modification;
         break;
      case STAT_ATTACK_SPEED:
         effectee->stats.baseAttackSpeed += effect.statModInfo.modification;
         break;
      default:
         break;
   }
}


void EffectManager::removeEffect(Effect effect, ActorEntity* effectee) {
   if (effect.classification == EFFECT_CLASS_DOT || effect.classification == EFFECT_CLASS_TIMED) {
      turnQueue->removeEffect(effect, effectee);
   }

   switch (effect.type) {
      case EFFECT_TYPE_STAT_MOD:
         removeStatModification(effect, effectee);
         break;
      default:
        break; 
   }
}

void EffectManager::removeEffectFromActorList(Effect effect, ActorEntity* effectee) {
   auto actorEffect = effectee->activeEffects.getEffect(effect);
   if (actorEffect == nullptr) {
      DebugLogger::log("Attempt to remove nonexistent effect");
      return;
   }
   else {
      if (actorEffect->second > 1) {
         actorEffect->second--;
      }
      else {
         effectee->activeEffects.removeEffect(effect);
      }
   }

}

void EffectManager::removeStatModification(Effect effect, ActorEntity* effectee) {
   switch (effect.statModInfo.stat) {
      case STAT_MAX_HEALTH:
         effectee->stats.maxHealth -= effect.statModInfo.modification;
         break;
      case STAT_STRENGTH:
         effectee->stats.strength -= effect.statModInfo.modification;
         break;
      case STAT_INTELLIGENCE:
         effectee->stats.intelligence -= effect.statModInfo.modification;
         break;
      case STAT_MOVE_SPEED:
         effectee->stats.baseMoveSpeed -= effect.statModInfo.modification;
         break;
      case STAT_ATTACK_SPEED:
         effectee->stats.baseAttackSpeed -= effect.statModInfo.modification;
         break;
      default:
         break;
   }
}

void EffectManager::updateDoTEffect(Effect effect, ActorEntity* effectee) {
   effect.dotInfo.duration -= effect.dotInfo.tickTime;

   if (effect.dotInfo.duration > effect.dotInfo.tickTime) {
      turnQueue->insertEffect(effect, effectee, effect.dotInfo.tickTime);
   }
   else {
      removeEffectFromActorList(effect, effectee);
   }
}

void EffectManager::endTimedEffect(Effect effect, ActorEntity* effectee) {
   effectee->activeEffects.removeEffect(effect);
}