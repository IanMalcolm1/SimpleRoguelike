#pragma once

#include <cwchar>
#include <string>
#include "Topography/TileCoordinates.h"
#include "GraphicsThings/TileDisplay.h"
#include "Entities/Effects/Effect.h"
#include "Entities/Damage.h"
#include "Enums/TurnTime.h"


/* Components inheriting from the actual Component class
 * should not contain pointers, whether directly or via
 * a class like std::string or std::vector. Otherwise
 * memory leaks will abound.
 *
 * 'Components' that require pointers should not inherit
 * from the Component struct and should be included
 * directly as part of a subclass of Entity (see
 * ActorEntity or ItemEntity).
 *
 * Neither official nor unofficial components should
 * contain their own logic. That is the domain of the
 * functions that correspond to AiStates.
 */

struct Description {
   std::string name;
   std::string desc;

   Description(std::string name = "noname", std::string desc = "indescribable")
      : name(name), desc(desc) {};
};


enum ComponentType {
   COMPONENT_DURABILITY = 0,
   COMPONENT_EFFECT = 1,
   COMPONENT_WEARABLE,
   COMPONENT_WIELDABLE,
   COMPONENT_DAMAGING,
   COMPONENT_CONSUMABLE,
   COMPONENT_RANGED,

   NUM_COMPONENT_TYPES
};
struct Component {};


struct WieldableComp : public Component {
   int handsRequired;

   WieldableComp(int handsRequired = 1) : handsRequired(handsRequired) {};
};

struct DurabilityComp : public Component {
   int total, current;

   DurabilityComp(int total, int current) : total(total), current(current) {};
};


struct WearableComp : public Component { //aka armor
   Damage damage; //for damage negated

   WearableComp(DamageType type = DAMAGE_NONE, int diceSize = 6, int dice = 0,
   int constant = 0) : damage(type, diceSize, dice, constant) {};
};


struct DamagingComp : public Component {
   Damage damage1;

   DamagingComp(Damage damage1 = Damage())
      : damage1(damage1) {};
};


struct EffectComp : public Component {
   Effect effect1;

   EffectComp(Effect effect1 = Effect())
      : effect1(effect1) {};
};


struct ConsumableComp : public Component {
   int charges;

   ConsumableComp(int charges = 1) : charges(charges) {};
};


struct RangedComp : public Component {
   int range;

   RangedComp(int range = 10) : range(range) {};
};
