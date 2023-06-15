#pragma once

#include "Entities/Items/ItemEntity.h"
#include "Entities/EntityColiseum.h"
#include "Topography/LocalMap.h"
#include "GraphicsThings/ColorMap.h"
#include "Topography/TileCoordinates.h"
#include "Entities/Effects/EffectFactory.h"


enum NaturalWeaponType {
   NATWEAP_FIST,
   NATWEAP_POISON_FANGS,
   NATWEAP_FANGS,
   NATWEAP_FORCE_CANTRIP,
   NUM_NATURALWEAPONS
};


class ItemFactory {
private:
   EntityColiseum<ItemEntity>* coliseum;
   LocalMap* map;

   ColorMap colorMap;

   ItemEntity* naturalWeapons[NUM_NATURALWEAPONS];
   
   EffectFactory effectFactory;

   void registerItem(ItemEntity* item, TileCoords location);

   //Natural weapons
   ItemEntity* makeFists();
   ItemEntity* makePoisonFangs();
   ItemEntity* makeFangs();
   ItemEntity* makeForceCantrip();

public:
   ItemFactory() : coliseum(nullptr), map(nullptr) {
      for (int i=0; i<NUM_NATURALWEAPONS; i++) {
         naturalWeapons[i] = nullptr;
      }
   };

   void initialize(EntityColiseum<ItemEntity>* coliseum, LocalMap* map);

   //Weapons
   ItemEntity* makeBasicDagger(TileCoords location);
   ItemEntity* makeBasicSword(TileCoords location);
   ItemEntity* makeBasicMace(TileCoords location);
   ItemEntity* makeForceWand(TileCoords location);

   //Armor
   ItemEntity* makeGambeson(TileCoords location);
   ItemEntity* makeLeatherArmor(TileCoords location);

   //Consumables
   ItemEntity* makeIntelligenceMushroom(TileCoords location);
   ItemEntity* makeStrengthFruit(TileCoords location);

   ItemEntity* getNaturalWeapon(NaturalWeaponType type);
};
