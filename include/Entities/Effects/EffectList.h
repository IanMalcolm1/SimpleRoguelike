#pragma once

#include "Entities/Effects/Effect.h"
#include <vector>


class EffectList {
private:
   std::vector<Effect> effects;

public:
   void updateEffectDurations(int turnTime);
   Effect* getEffect(Effect& effect);
   void addEffect(Effect& effect);
   void removeEffect(Effect& effect);
   void replaceEffect(Effect& replacement);
   std::vector<Effect>* getAllEffects();
};
